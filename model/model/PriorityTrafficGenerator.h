//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __BACKBONE_PRIORITYTRAFFICGENERATOR_H_
#define __BACKBONE_PRIORITYTRAFFICGENERATOR_H_

#include <omnetpp.h>
#include <MACAddress.h>
#include "cModule.h"
#include <vector>
#include <string>

#include <sstream>
#include <iterator>

#include "jDEECoModule.h"

/**
 * TODO - Generated class
 */
class PriorityTrafficGenerator : public cSimpleModule, public jDEECoModule
{
protected:
  static simtime_t *globalMaxResponseTimes;

  int periodicKind;

  long seqNum;

  // send parameters
  cPar *sendInterval;
  cPar *numPacketsPerBurst;
  cPar *packetLength;
  cPar *responseDelay;
  int etherType;
  MACAddress destMACAddress;

  // self messages
  cMessage *timerMsg;
  simtime_t stopTime;

  // receive statistics
  long packetsSent;
  long packetsReceived;
  static simsignal_t sentPkSignal;
  static simsignal_t rcvdPkSignal;

  cOutVector *responseTimes;
  simtime_t *maxResponseTimes;

  short currentPriority;
  short maxPriority;

  bool dynamicAPConnections;

  cModule *currentlyAPConnected;
  std::vector<std::string> accessPointPaths;

public:
  PriorityTrafficGenerator();
  ~PriorityTrafficGenerator();

protected:
  virtual void initialize(int stage);
  virtual int numInitStages() const {return 2;}
  virtual void handleMessage(cMessage *msg);
  virtual void finish();

  virtual MACAddress resolveDestMACAddress();

  virtual void sendBurstPackets();
  virtual void receivePacket(cPacket *msg);

  virtual void reconnect();
  virtual void sendSinglePacket(const char *msgName, short priority, simtime_t timestamp = -1.0);

  //Needs to be implemented by the module
  virtual const char * jDEECoGetModuleId();
  //Needs to be implemented by the module
  virtual void jDEECoBroadcastPacket(unsigned char * data);
};

#endif
