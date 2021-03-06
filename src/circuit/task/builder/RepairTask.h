/*
 * RepairTask.h
 *
 *  Created on: Jan 29, 2015
 *      Author: rlcevg
 */

#ifndef SRC_CIRCUIT_TASK_REPAIRTASK_H_
#define SRC_CIRCUIT_TASK_REPAIRTASK_H_

#include "task/common/RepairTask.h"

namespace circuit {

class CEconomyManager;

class CBRepairTask: public IRepairTask {
public:
	CBRepairTask(ITaskManager* mgr, Priority priority, CAllyUnit* target, int timeout = 0);
	virtual ~CBRepairTask();

	virtual void Update();

	virtual void OnUnitIdle(CCircuitUnit* unit);
	virtual void OnUnitDamaged(CCircuitUnit* unit, CEnemyUnit* attacker);
};

} // namespace circuit

#endif // SRC_CIRCUIT_TASK_REPAIRTASK_H_
