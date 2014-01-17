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

#ifndef DROPTAILPRIORITYQUEUE_H_
#define DROPTAILPRIORITYQUEUE_H_

#include <DropTailQueue.h>
#include <cQueue.h>

class DropTailPriorityQueue: public DropTailQueue {
protected:
  virtual void initialize();

  /**
   * Redefined from PassiveQueueBase.
   */
  virtual cMessage *enqueue(cMessage *msg);

  /**
   * Redefined from PassiveQueueBase.
   */
  virtual cMessage *dequeue();

  /**
   * Redefined from PassiveQueueBase.
   */
  virtual void sendOut(cMessage *msg);

  /**
   * Redefined from IPassiveQueue.
   */
  virtual bool isEmpty();
};

#endif /* DROPTAILPRIORITYQUEUE_H_ */
