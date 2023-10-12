#pragma once
#include "Enemy.h"
#include "../Castle/Castle.h"
class Freezer:public Enemy
{
public:
	Freezer(int id, double health, int arrTime, double speed, double power, double ReloadPeriod);

	void Move() override;
	void FreezeCastle(Castle*);
};

