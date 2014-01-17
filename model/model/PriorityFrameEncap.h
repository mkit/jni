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

#ifndef __BACKBONE_PRIORITYFRAMEENCAP_H_
#define __BACKBONE_PRIORITYFRAMEENCAP_H_

#include <omnetpp.h>
#include "EtherFrame_m.h"

#define PRIORITY_ETHER_MAC_FRAME_BYTES 1

class PriorityFrameEncap : public cSimpleModule
{
protected:
  int seqNum;
  // statistics
  long totalFromHigherLayer;  // total number of packets received from higher layer
  long totalFromMAC;          // total number of frames received from MAC
  static simsignal_t encapPkSignal;
  static simsignal_t decapPkSignal;

protected:
  virtual void initialize();
  virtual void handleMessage(cMessage *msg);

  virtual void processPacketFromHigherLayer(cPacket *msg);
  virtual void processFrameFromMAC(EtherFrame *msg);

  virtual void updateDisplayString();
};

#endif
