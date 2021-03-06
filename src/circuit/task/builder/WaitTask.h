/*
 * WaitTask.h
 *
 *  Created on: May 29, 2017
 *      Author: rlcevg
 */

#ifndef SRC_CIRCUIT_TASK_BUILDER_WAITTASK_H_
#define SRC_CIRCUIT_TASK_BUILDER_WAITTASK_H_

#include "task/common/WaitTask.h"

namespace circuit {

class CBWaitTask: public IWaitTask {
public:
	CBWaitTask(ITaskManager* mgr, int timeout);
	virtual ~CBWaitTask();

	virtual void OnUnitDamaged(CCircuitUnit* unit, CEnemyUnit* attacker);
};

} // namespace circuit

#endif // SRC_CIRCUIT_TASK_BUILDER_WAITTASK_H_
