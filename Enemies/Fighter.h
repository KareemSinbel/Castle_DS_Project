#pragma once
#include "Enemy.h"
#include "../Castle/Castle.h"
class Fighter:public Enemy
{
private:
	int priority;
public:
	Fighter(int id, double health, int arrTime, double speed, double power, double ReloadPeriod);
	
	int getPriority() const;
	void Move() override;
	void AttackCastle(Castle*);
	
};
