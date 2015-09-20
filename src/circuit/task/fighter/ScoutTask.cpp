/*
 * ScoutTask.cpp
 *
 *  Created on: Jan 28, 2015
 *      Author: rlcevg
 */

#include "task/fighter/ScoutTask.h"
#include "task/TaskManager.h"
#include "module/MilitaryManager.h"
#include "terrain/TerrainManager.h"
#include "terrain/ThreatMap.h"
#include "terrain/PathFinder.h"
#include "resource/MetalManager.h"
#include "unit/EnemyUnit.h"
#include "unit/action/MoveAction.h"
#include "CircuitAI.h"
#include "util/utils.h"

#include "AISCommands.h"

namespace circuit {

using namespace springai;

CScoutTask::CScoutTask(ITaskManager* mgr)
		: IFighterTask(mgr, FightType::SCOUT)
		, isUpdating(false)
		, updCount(0)
{
}

CScoutTask::~CScoutTask()
{
	PRINT_DEBUG("Execute: %s\n", __PRETTY_FUNCTION__);
}

void CScoutTask::AssignTo(CCircuitUnit* unit)
{
	IFighterTask::AssignTo(unit);

	CMoveAction* moveAction = new CMoveAction(unit);
	unit->PushBack(moveAction);
	moveAction->SetActive(false);
}

void CScoutTask::Execute(CCircuitUnit* unit)
{
	IUnitAction* act = static_cast<IUnitAction*>(unit->End());
	if (act->GetType() != IUnitAction::Type::MOVE) {
		return;
	}
	CMoveAction* moveAction = static_cast<CMoveAction*>(act);

	CCircuitAI* circuit = manager->GetCircuit();
	CTerrainManager* terrainManager = circuit->GetTerrainManager();
	CThreatMap* threatMap = circuit->GetThreatMap();
	Unit* u = unit->GetUnit();

	F3Vec path;
	CEnemyUnit* bestTarget = FindBestTarget(unit, path);

	if (bestTarget != nullptr) {
		position = bestTarget->GetPos();
		u->Attack(bestTarget->GetUnit(), UNIT_COMMAND_OPTION_INTERNAL_ORDER, FRAMES_PER_SEC * 300);
		moveAction->SetActive(false);
		return;
	} else if (!path.empty()) {
		position = path.back();
		moveAction->SetPath(path);
		moveAction->Update(circuit);
		moveAction->SetActive(true);
		return;
	}

	if (isUpdating && (threatMap->GetThreatAt(position) < threatMap->GetUnitThreat(unit))) {
		return;
	}

	CMetalManager* metalManager = circuit->GetMetalManager();
	const CMetalData::Clusters& clusters = metalManager->GetClusters();
	if (!clusters.empty()) {
		int index = circuit->GetMilitaryManager()->GetScoutIndex();
		position = clusters[index].geoCentr;
		const CMetalData::Metals& spots = metalManager->GetSpots();
		auto it = clusters[index].idxSpots.begin();
		u->Fight(spots[*it].position, UNIT_COMMAND_OPTION_INTERNAL_ORDER, FRAMES_PER_SEC * 300);
		auto end = clusters[index].idxSpots.end();
		while (++it != end) {
			u->Fight(spots[*it].position, UNIT_COMMAND_OPTION_SHIFT_KEY, FRAMES_PER_SEC * 300);
		}
		moveAction->SetActive(false);
		return;
	}

	float x = rand() % (terrainManager->GetTerrainWidth() + 1);
	float z = rand() % (terrainManager->GetTerrainHeight() + 1);
	position = AIFloat3(x, circuit->GetMap()->GetElevationAt(x, z), z);
	u->Fight(position, UNIT_COMMAND_OPTION_INTERNAL_ORDER, FRAMES_PER_SEC * 300);
	moveAction->SetActive(false);
}

void CScoutTask::Update()
{
	if (updCount++ % 4 == 0) {
		// FIXME: Update group target
		isUpdating = true;
		for (CCircuitUnit* unit : units) {
			Execute(unit);
		}
		isUpdating = false;
	} else {
		IFighterTask::Update();
	}
}

CEnemyUnit* CScoutTask::FindBestTarget(CCircuitUnit* unit, F3Vec& path)
{
	CCircuitAI* circuit = manager->GetCircuit();
	CTerrainManager* terrainManager = circuit->GetTerrainManager();
	CThreatMap* threatMap = circuit->GetThreatMap();
	AIFloat3 pos = unit->GetUnit()->GetPos();
	STerrainMapArea* area = unit->GetArea();
	float power = threatMap->GetUnitThreat(unit) * 0.5f;

	CEnemyUnit* bestTarget = nullptr;
	F3Vec enemyPositions;
	float range = unit->GetUnit()->GetMaxRange() + terrainManager->GetConvertStoP() * 2;
	float minSqDist = range * range;
	const CCircuitAI::EnemyUnits& enemies = circuit->GetEnemyUnits();
	for (auto& kv : enemies) {
		CEnemyUnit* enemy = kv.second;
		if (enemy->IsHidden() || (threatMap->GetThreatAt(enemy->GetPos()) >= power) ||
			!terrainManager->CanMoveToPos(area, enemy->GetPos()))
		{
			continue;
		}
		float sqDist = pos.SqDistance2D(enemy->GetPos());
		if (sqDist < minSqDist) {
			bestTarget = enemy;
			minSqDist = sqDist;
		} else {
			enemyPositions.push_back(enemy->GetPos());
		}
	}

	path.clear();
	if ((bestTarget != nullptr) || enemyPositions.empty()) {
		return bestTarget;
	}

	circuit->GetPathfinder()->SetMapData(unit->GetCircuitDef()->GetMobileId());
	circuit->GetPathfinder()->FindBestPath(path, pos, range * 0.5f, enemyPositions);

	return nullptr;
}

} // namespace circuit
