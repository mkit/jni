/*
 * jDEECoModule.h
 *
 *  Created on: 24 Dec 2013
 *      Author: Michal Kit <kit@d3s.mff.cuni.cz>
 */

#ifndef JDEECOMODULE_H_
#define JDEECOMODULE_H_

#include "jDEECoModuleHelper.h"

class DLLEXPORT jDEECoModule {

public:
	void *host;
	void *env;

	jDEECoModule() {host = NULL; env = NULL;};
	virtual ~jDEECoModule() {};

	//Needs to be implemented by the module
	virtual const char * jDEECoGetModuleId() {return NULL;};
	//Needs to be implemented by the module
	virtual void jDEECoBroadcastPacket(unsigned char * data) {};

protected:
	void jDEECoPacketReceived(unsigned char * data) {
		packetReceived(data, this);
	}

	//Needs to be called at the module initialisation
	void jDEECoInitialize() {
		registerModule(this);
	}
};

#endif /* JDEECOMODULE_H_ */
