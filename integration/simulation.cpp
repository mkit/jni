#include "simulation.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <algorithm>

#include "jDEECoModule.h"

#include "opp_ctype.h"
#include "args.h"
#include "distrib.h"
#include "cconfigoption.h"
#include "inifilereader.h"
#include "sectionbasedconfig.h"
#include "appreg.h"
#include "cmodule.h"
#include "fsutils.h"
#include "fnamelisttokenizer.h"
#include "stringutil.h"
#include "fileutil.h"
#include "intxtypes.h"
#include "startup.h"

USING_NAMESPACE;

Register_GlobalConfigOption(CFGID_LOAD_LIBS, "load-libs", CFG_FILENAMES, "", "A space-separated list of dynamic libraries to be loaded on startup. The libraries should be given without the `.dll' or `.so' suffix -- that will be automatically appended.");
Register_GlobalConfigOption(CFGID_CONFIGURATION_CLASS, "configuration-class", CFG_STRING, "", "Part of the Envir plugin mechanism: selects the class from which all configuration information will be obtained. This option lets you replace omnetpp.ini with some other implementation, e.g. database input. The simulation program still has to bootstrap from an omnetpp.ini (which contains the configuration-class setting). The class should implement the cConfigurationEx interface.");
Register_GlobalConfigOption(CFGID_USER_INTERFACE, "user-interface", CFG_STRING, "", "Selects the user interface to be started. Possible values are Cmdenv and Tkenv. This option is normally left empty, as it is more convenient to specify the user interface via a command-line option or the IDE's Run and Debug dialogs. New user interfaces can be defined by subclassing cRunnableEnvir.");

class jDEECoRuntime {
public:
	void *host;
	void *env;
	const char * id;

	jDEECoRuntime(void *host, void *env, const char *id) {
		this->env = env;
		this->host = host;
		this->id = id;
	}
};

std::vector<jDEECoRuntime *> jDEECoRuntimes;
std::vector<jDEECoModule *> jDEECoModules;

// helper macro
#define CREATE_BY_CLASSNAME(var,classname,baseclass,description) \
     baseclass *var ## _tmp = (baseclass *) createOne(classname); \
     var = dynamic_cast<baseclass *>(var ## _tmp); \
     if (!var) \
         throw cRuntimeError("Class \"%s\" is not subclassed from " #baseclass, (const char *)classname);


static void verifyIntTypes()
{
#define VERIFY(t,size) if (sizeof(t)!=size) {printf("INTERNAL ERROR: sizeof(%s)!=%d, please check typedefs in include/inttypes.h, and report this bug!\n\n", #t, size); abort();}
    VERIFY(int8,  1);
    VERIFY(int16, 2);
    VERIFY(int32, 4);
    VERIFY(int64, 8);

    VERIFY(uint8, 1);
    VERIFY(uint16,2);
    VERIFY(uint32,4);
    VERIFY(uint64,8);
#undef VERIFY

#define LL  INT64_PRINTF_FORMAT
    char buf[32];
    int64 a=1, b=2;
    sprintf(buf, "%"LL"d %"LL"d", a, b);
    if (strcmp(buf, "1 2")!=0) {printf("INTERNAL ERROR: INT64_PRINTF_FORMAT incorrectly defined in include/inttypes.h, please report this bug!\n\n"); abort();}
#undef LL
}

void simulate(const char * envName)
{
	cStaticFlag dummy;
	//
    // SETUP
    //
    cSimulation *simulationobject = NULL;
    cRunnableEnvir *app = NULL;
    SectionBasedConfiguration *bootconfig = NULL;
    cConfigurationEx *configobject = NULL;
    try
    {
        // construct global lists
        ExecuteOnStartup::executeAll();

        // verify definitions of int64, int32, etc.
        verifyIntTypes();

        //
        // First, load the ini file. It might contain the name of the user interface
        // to instantiate.
        //
        const char *fname = "omnetpp.ini"; //default filename

        InifileReader *inifile = new InifileReader();
        if (fileExists(fname))
            inifile->readFile(fname);


        // activate [General] section so that we can read global settings from it
        bootconfig = new SectionBasedConfiguration();
        bootconfig->setConfigurationReader(inifile);
        bootconfig->activateConfig("General", 0);

        //load libs
        std::vector<std::string> libs = bootconfig->getAsFilenames(CFGID_LOAD_LIBS);
        for (int k=0; k<(int)libs.size(); k++) {
        	::printf("Loading %s ...\n", libs[k].c_str());
        	loadExtensionLibrary(libs[k].c_str());
        }

        //
        // Create custom configuration object, if needed.
        //
        std::string configclass = bootconfig->getAsString(CFGID_CONFIGURATION_CLASS);
	    if (configclass.empty())
	    {
	    	configobject = bootconfig;
	    }
	    else
	    {
	    	// create custom configuration object
	        CREATE_BY_CLASSNAME(configobject, configclass.c_str(), cConfigurationEx, "configuration");
	        configobject->initializeFrom(bootconfig);
	        configobject->activateConfig("General", 0);
	        delete bootconfig;
	        bootconfig = NULL;

	        // load libs from this config as well
	        std::vector<std::string> libs = configobject->getAsFilenames(CFGID_LOAD_LIBS);
	        for (int k=0; k<(int)libs.size(); k++)
	        	loadExtensionLibrary(libs[k].c_str());
	    }


        // validate the configuration, but make sure we don't report cmdenv-* keys
        // as errors if Cmdenv is absent; same for Tkenv.
        std::string ignorablekeys;
        if (omnetapps.getInstance()->lookup("Cmdenv")==NULL)
            ignorablekeys += " cmdenv-*";
        if (omnetapps.getInstance()->lookup("Tkenv")==NULL)
            ignorablekeys += " tkenv-*";
        configobject->validate(ignorablekeys.c_str());

        //
        // Choose and set up user interface (EnvirBase subclass). Everything else
        // will be done by the user interface class.
        //
        const char * appname = envName;
        if (appname == NULL || opp_strcmp(appname, "") == 0)
        	appname = configobject->getAsString(CFGID_USER_INTERFACE).c_str();
        cOmnetAppRegistration *appreg = NULL;
        if (!(appname == NULL || opp_strcmp(appname, "") == 0))
        {
        	// look up specified user interface
            appreg = static_cast<cOmnetAppRegistration *>(omnetapps.getInstance()->lookup(appname));
            if (!appreg)
            {
            	::printf("\n"
                	"User interface '%s' not found (not linked in or loaded dynamically).\n"
                    "Available ones are:\n", appname);
                cRegistrationList *a = omnetapps.getInstance();
                for (int i=0; i<a->size(); i++)
                	::printf("  %s : %s\n", a->get(i)->getName(), a->get(i)->info().c_str());

                throw cRuntimeError("Could not start user interface '%s'", appname);
            }
        }
        else
        {
        	// user interface not explicitly selected: pick one from what we have
            appreg = cOmnetAppRegistration::chooseBest();
            if (!appreg)
            	throw cRuntimeError("No user interface (Cmdenv, Tkenv, etc.) found");
        }

        //
        // Create interface object.
        //
        ::printf("Setting up %s...\n", appreg->getName());
        app = appreg->createOne();
    }
    catch (std::exception& e)
    {
        ::fprintf(stderr, "\n<!> Error during startup: %s.\n", e.what());
        if (app)
        {
            delete app;
            app = NULL;
        }
        else
        {
            delete bootconfig;
        }
    }

    //
    // RUN
    //
    try
    {
        if (app)
        {
            simulationobject = new cSimulation("simulation", app);
            cSimulation::setActiveSimulation(simulationobject);
            app->run(0, NULL, configobject);
        }
    }
    catch (std::exception& e)
    {
        ::fprintf(stderr, "\n<!> %s.\n", e.what());
    }

    //
    // SHUTDOWN
    //
    cSimulation::setActiveSimulation(NULL);
    delete simulationobject;  // will delete app as well

    componentTypes.clear();
    nedFunctions.clear();
    classes.clear();
    enums.clear();
    classDescriptors.clear();
    configOptions.clear();
    omnetapps.clear();
    cSimulation::clearLoadedNedFiles();
}

JNIEXPORT jint JNICALL _JNI_OnLoad(JavaVM *vm, void *reserved) {
	return JNI_VERSION_1_6;
}

JNIEXPORT jdouble JNICALL _Java_cz_cuni_mff_d3s_deeco_simulation_Simulation_getCurrentTime(JNIEnv *env, jobject jsimulation) {
	if (cSimulation::getActiveSimulation() == NULL)
		return -1;
	else
		return cSimulation::getActiveSimulation()->getSimTime().dbl();
}

JNIEXPORT void JNICALL _Java_cz_cuni_mff_d3s_deeco_simulation_Simulation_initializeHost(JNIEnv *env, jobject jsimulation, jobject object, jstring id) {
	const char * cstring = env->GetStringUTFChars(id,0);
	for (std::vector<jDEECoRuntime *>::iterator it = jDEECoRuntimes.begin(); it != jDEECoRuntimes.end(); ++it) {
		if (opp_strcmp((*it)->id, cstring) == 0) {
			return;
		}
	}
	jDEECoRuntimes.push_back(new jDEECoRuntime(&object, env, cstring));
}

JNIEXPORT void JNICALL _Java_cz_cuni_mff_d3s_deeco_simulation_Simulation_broadcastPacket(JNIEnv *env, jobject jsimulation, jstring id, jbyteArray packet) {
	const char * cstring = env->GetStringUTFChars(id,0);
	for (std::vector<jDEECoModule *>::iterator it = jDEECoModules.begin(); it != jDEECoModules.end(); ++it) {
		if (opp_strcmp((*it)->jDEECoGetModuleId(), cstring) == 0) {
			int length = env->GetArrayLength(packet);
		    unsigned char* buffer = new unsigned char[length];
		    env->GetByteArrayRegion(packet, 0, length, reinterpret_cast<jbyte*>(buffer));
		    (*it)->jDEECoBroadcastPacket(buffer);
		    break;
		 }
	}
	env->ReleaseStringUTFChars(id, cstring);
}

JNIEXPORT void JNICALL _Java_cz_cuni_mff_d3s_deeco_simulation_Simulation_run(JNIEnv *env, jobject jsimulation, jstring environment) {
	const char * cEnv = env->GetStringUTFChars(environment,0);
	simulate(cEnv);
	jDEECoModules.clear();
	jDEECoRuntimes.clear();
	env->ReleaseStringUTFChars(environment, cEnv);
}

DLLEXPORT void packetReceived(unsigned char * data, void * module) {
	jDEECoModule *jModule = (jDEECoModule *) module;
	if (jModule->env != NULL && jModule->host != NULL) {
		jclass cls = ((JNIEnv *) jModule->env)->GetObjectClass(*((jobject *) jModule->host));
		jmethodID mid = ((JNIEnv *) jModule->env)->GetMethodID(cls, "packetReceived", "(I)V");
		if (mid == 0)
			return;
		jbyteArray packet = ((JNIEnv *) jModule->env)->NewByteArray(sizeof(data));
		((JNIEnv *) jModule->env)->SetByteArrayRegion(packet, 0, sizeof(data), (jbyte*)data);
		((JNIEnv *) jModule->env)->CallVoidMethod(*((jobject *) jModule->host), mid, packet);
	}
}

DLLEXPORT void registerModule(void * module) {
	jDEECoModule *jModule = (jDEECoModule *) module;
	printf("Initializing jDEECo module: %s\n", jModule->jDEECoGetModuleId());
	if(std::find(jDEECoModules.begin(), jDEECoModules.end(), jModule) == jDEECoModules.end()) {
		for (std::vector<jDEECoRuntime *>::iterator it = jDEECoRuntimes.begin(); it != jDEECoRuntimes.end(); ++it) {
			if (opp_strcmp((*it)->id, jModule->jDEECoGetModuleId()) == 0) {
				jModule->host = (*it)->host;
				jModule->env = (*it)->env;
				jDEECoModules.push_back(jModule);
				printf("Module registered\n");
				break;
			}
		}
	}
}
