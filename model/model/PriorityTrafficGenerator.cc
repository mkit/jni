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
#include <ctimestampedvalue.h>
#include "PriorityTrafficGenerator.h"
#include "Ieee802Ctrl_m.h"

Define_Module(PriorityTrafficGenerator)

simsignal_t PriorityTrafficGenerator::sentPkSignal = SIMSIGNAL_NULL;
simsignal_t PriorityTrafficGenerator::rcvdPkSignal = SIMSIGNAL_NULL;
simtime_t *PriorityTrafficGenerator::globalMaxResponseTimes = NULL;

using namespace std;

PriorityTrafficGenerator::PriorityTrafficGenerator() {
    timerMsg = NULL;
}

PriorityTrafficGenerator::~PriorityTrafficGenerator() {
    delete[] responseTimes;
    cancelAndDelete(timerMsg);
}

const char * PriorityTrafficGenerator::jDEECoGetModuleId() {
    return getName();
}

void PriorityTrafficGenerator::jDEECoBroadcastPacket(unsigned char * data)  {
    //TODO
}

void PriorityTrafficGenerator::initialize(int stage) {
    // we can only initialize in the 2nd stage (stage==1), because
    // assignment of "auto" MAC addresses takes place in stage 0
    if (stage == 1) {
        sendInterval = &par("sendInterval");
        numPacketsPerBurst = &par("numPacketsPerBurst");
        packetLength = &par("packetLength");
        etherType = par("etherType");
        responseDelay = &par("responseDelay");

        maxPriority = par("maxPriority");

        seqNum = 0;
        //WATCH(seqNum);

        // statistics
        packetsSent = packetsReceived = 0;
        sentPkSignal = registerSignal("sentPk");
        rcvdPkSignal = registerSignal("rcvdPk");
        //WATCH(packetsSent);
        //WATCH(packetsReceived);

        currentPriority = 0;

        if (globalMaxResponseTimes == NULL) {
            globalMaxResponseTimes = new simtime_t[maxPriority];
            for (int i = 0; i < maxPriority; i++) {
                globalMaxResponseTimes[i] = simTime() - simTime();
            }
        }

        maxResponseTimes = new simtime_t[maxPriority];
        for (int i = 0; i < maxPriority; i++) {
            maxResponseTimes[i] = simTime() - simTime();
        }

        destMACAddress = resolveDestMACAddress();

        responseTimes = new cOutVector[maxPriority];
        for (int i = 0; i < maxPriority; i++) {
            char buffer[40];
            sprintf(buffer, "responseTimes[%d]", i);
            responseTimes[i].setName(buffer);
        }

        // if no dest address given, nothing to do
        if (destMACAddress.isUnspecified())
            return;

        simtime_t startTime = par("startTime");
        stopTime = par("stopTime");
        if (stopTime != 0 && stopTime <= startTime)
            error("Invalid startTime/stopTime parameters");

        periodicKind = maxPriority + 1;
        timerMsg = new cMessage("generateNextPacket", periodicKind);
        scheduleAt(startTime, timerMsg);

        //find access points
        currentlyAPConnected = NULL;
        dynamicAPConnections = par("dynamicAPConnections");
        if (dynamicAPConnections) {
            cModule *network = simulation.getSystemModule();
            for (SubmoduleIterator iter(network); !iter.end(); iter++)
            {
                if (iter()->hasPar("relayUnitType")
                        && opp_strcmp(iter()->par("relayUnitType"),"PriorityMACRelayUnitNPAccessPoint") == 0) {
                           accessPointPaths.push_back(iter()->getFullPath());
                }
            }
        }
        }
    	//jDEECo Initialization
    	jDEECoInitialize();
    }

MACAddress PriorityTrafficGenerator::resolveDestMACAddress() {
    MACAddress destMACAddress;
    const char *destAddress = par("destAddress");
    if (destAddress[0]) {
        // try as mac address first, then as a module
        if (!destMACAddress.tryParse(destAddress)) {
            cModule *destStation = simulation.getModuleByPath(destAddress);
            if (!destStation)
            error("cannot resolve MAC address '%s': not a 12-hex-digit MAC address or a valid module path name", destAddress);

            cModule *destMAC = destStation->getSubmodule("mac");
            if (!destMAC)
            error("module '%s' has no 'mac' submodule", destAddress);

            destMACAddress.setAddress(destMAC->par("address"));
        }
    }
    return destMACAddress;
}

void PriorityTrafficGenerator::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        if (msg->getKind() == periodicKind) {
            //Send messages
            sendBurstPackets();
            simtime_t d = simTime() + sendInterval->doubleValue();
            if (stopTime == 0 || d < stopTime)
                scheduleAt(d, msg);
        } else {
            //Send response
            sendSinglePacket(msg->getName(), msg->getKind(),
                    msg->getCreationTime());
            delete msg;
        }
    } else {
        receivePacket(check_and_cast<cPacket*>(msg));
    }
}

void PriorityTrafficGenerator::reconnect() {
    if (accessPointPaths.size() > 0) {
        int connectToIndex = -1;
        if (currentlyAPConnected != NULL) {
            if (currentlyAPConnected->getIndex()
                    < (int) (accessPointPaths.size() - 1)) {
                connectToIndex = currentlyAPConnected->getIndex() + 1;
            }
        } else {
            connectToIndex = 0;
        }
        cModule *parent = getParentModule();
        if (currentlyAPConnected != NULL) {
            //disconnect
            currentlyAPConnected->gate("ethg$o", parent->getIndex() + 1)->disconnect();
            parent->gate("ethg$o")->disconnect();
        }
        if (connectToIndex >= 0) {
            string connectToPath = accessPointPaths[connectToIndex];
            cModule *network = simulation.getSystemModule();
            cModule *connectTo = network->getModuleByPath(
                    connectToPath.c_str());

            //connect to AP
            currentlyAPConnected = connectTo;
            cChannelType *channelType = cChannelType::get("backbone.Network.C");
            cChannel *channel = channelType->create("channelAP");
            currentlyAPConnected->gate("ethg$o", parent->getIndex() + 1)->connectTo(
                    parent->gate("ethg$i"), channel);
            channel = channelType->create("channelV");
            parent->gate("ethg$o")->connectTo(
                    currentlyAPConnected->gate("ethg$i",
                            parent->getIndex() + 1), channel);

            EV<< parent->getName() << " connecting to " << connectToPath << endl;
            if (connectToIndex <= 0)
                currentPriority = 0;
            else
                currentPriority = (connectToIndex * 1.0
                        / accessPointPaths.size()) * maxPriority;
        } else {
            currentlyAPConnected = NULL;
        }
    }

}

void PriorityTrafficGenerator::sendBurstPackets() {
    int n = numPacketsPerBurst->longValue();
    for (int i = 0; i < n; i++) {
        char msgname[40];
        sprintf(msgname, "pk-%d-%ld", getId(), seqNum);
        EV<< "Generating packet `" << msgname << "'\n";
        sendSinglePacket(msgname, currentPriority);
    }
}

void PriorityTrafficGenerator::sendSinglePacket(const char *msgName,
        short priority, simtime_t timestamp) {
    seqNum++;

    cPacket *datapacket = new cPacket(msgName, IEEE802CTRL_DATA);

    long len = packetLength->longValue();
    datapacket->setByteLength(len);

    if (timestamp > -1.0)
        datapacket->setTimestamp(timestamp);

    Ieee802Ctrl *etherctrl = new Ieee802Ctrl();
    etherctrl->setEtherType(etherType);
    etherctrl->setDest(destMACAddress);
    datapacket->setControlInfo(etherctrl);

    datapacket->setKind(priority);

    //emit(sentPkSignal, datapacket);
    send(datapacket, "out");
    packetsSent++;

    if (dynamicAPConnections) {
        if (intrand(2) == 0) {
            // reconnect
            reconnect();
        }
    } else {
        currentPriority = intrand(maxPriority);
    }
}

void PriorityTrafficGenerator::receivePacket(cPacket *msg) {
    string strName(msg->getName());
    bool scheduled = false;
    //Is response
    if (strName.find("-R") != string::npos) {
        char *token;
        char buffer[40];
        opp_strcpy(buffer, msg->getName());
        token = strtok(buffer, "-");
        if (token != NULL) {
            token = strtok(NULL, "-");
            if (token != NULL) {
                EV<< "Token: " << token << endl;
                if (getId() == atoi(token)) {
                    EV << "match" << endl;
                    simtime_t diff = simTime() - msg->getTimestamp();
                    responseTimes[msg->getKind()].record(diff);
//                    if (diff > maxResponseTimes[msg->getKind()])
//                    maxResponseTimes[msg->getKind()] = diff;
//                    if (diff > globalMaxResponseTimes[msg->getKind()])
//                    globalMaxResponseTimes[msg->getKind()] = diff;
                }
            }
        }
    } else if (responseDelay->doubleValue() >= 0) {
        EV<< "Not found" << endl;
        char msgname[40];
        sprintf(msgname, "%s-R", msg->getName());
        msg->setName(msgname);
        simtime_t d = simTime() + responseDelay->doubleValue();
        scheduleAt(d, msg);
        scheduled = true;
    }
    packetsReceived++;
    //emit(rcvdPkSignal, msg);
    if (!scheduled)
        dropAndDelete(msg);
}

void PriorityTrafficGenerator::finish() {
    cancelAndDelete(timerMsg);
    timerMsg = NULL;
    for (int i = 0; i < maxPriority; i++) {
        char buffer[40];
        sprintf(buffer, "maxResponseTime[%d]", i);
        recordScalar(buffer, maxResponseTimes[i]);
    }
    delete[] maxResponseTimes;
    if (globalMaxResponseTimes != NULL) {
        for (int i = 0; i < maxPriority; i++) {
            char buffer[40];
            sprintf(buffer, "globalMaxResponseTimes[%d]", i);
            recordScalar(buffer, globalMaxResponseTimes[i]);
        }
        delete[] globalMaxResponseTimes;
        globalMaxResponseTimes = NULL;
    }
}
