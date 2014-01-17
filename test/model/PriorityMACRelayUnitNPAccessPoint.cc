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

#include "PriorityMACRelayUnitNPAccessPoint.h"
#include "EtherFrame_m.h"

Define_Module(PriorityMACRelayUnitNPAccessPoint);

void PriorityMACRelayUnitNPAccessPoint::processFrame(cMessage *msg)
{
    int cpu = msg->getKind();
    EtherFrame *frame = (EtherFrame *) msg->getContextPointer();
    ASSERT(frame);
    msg->setContextPointer(NULL);
    long length = frame->getByteLength();
    int inputport = frame->getArrivalGate()->getIndex();

    EV << "CPU-" << cpu << " completed processing of frame " << frame << endl;

    if (inputport == 0)
        broadcastFrame(frame, inputport);
    else
        handleAndDispatchFrame(frame, 0);
    printAddressTable();

    bufferUsed -= length;
    bufferLevel.record(bufferUsed);

    numProcessedFrames++;

    // Process next frame in queue if they are pending
    if (!queue.empty())
    {
        EtherFrame *newframe = (EtherFrame *) queue.pop();
        msg->setContextPointer(newframe);
        EV << "CPU-" << cpu << " starting processing of frame " << newframe << endl;
        scheduleAt(simTime()+processingTime, msg);
    }
    else
    {
        EV << "CPU-" << cpu << " idle\n";
    }
}
