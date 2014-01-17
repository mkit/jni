jni
===
Project description:
integration - the main dll library project providing JNI funictionalities
model - a dll project containing simulation specific classes
embedding - a c++ exe project for testing functionality of the integration dll (requires modification of the simulation.cpp)
test - a Java project for testing functionality of the integration dll

All c++ dll projects require setting up includes, libraries and libraries path (can be done in Properties > C/C++ General > Paths and Symbols) libraries required are all dll from the OMNET++\bin directory that names ends with letter "d" (oppcmdenvd, oppsimd...) plus "oppmain" library from the OMNET++\lib directory. Bare in mind that libraries names given there are without the "lib" prefix and the extension. In the locations please povide the paths where those libraries can be found.
Additionally the model project requires the library and the path to inet (debug version) of the library. This can be found in the (inet project)\out\gcc-debug\src directory after building the inet project.

Both model and integration require appropriate includes, which are OMnet++\include directory and inet relevant directories. Those are:

"D:/Workspaces/OMNeT++/Workspace/inet/src/linklayer/ethernet/switch"
"D:/Workspaces/OMNeT++/Workspace/inet/src/linklayer/ethernet"
"D:/Workspaces/OMNeT++/Workspace/inet/src/linklayer/queue"
"D:/Workspaces/OMNeT++/Workspace/inet/src/base"
"D:/Workspaces/OMNeT++/Workspace/inet/src/linklayer/contract"

In order to run the test project you need JRE-32 bit version and you also need to specify an additional argument for library searching paths:

'-Djava.library.path="${project_loc};C:\Windows\System32"'

Moreover you need both dlls (libmodel.dll and libintegration.dll) in the application directory (test) together with the aforementioned *d.dll libraries (from OMNet++\bin) directory and inet.dll.

The embedding (exe project requires) all the aforementioned libraries (plus libmodel.dll and libintegration.dll) to be present in the application root directory. Additionally in order to run it you need to export the simulation method from the integration/simulation.cpp file and used that compiled version. To do that put the "extern "C" __declspec(dllexport)" before the simulate method.
