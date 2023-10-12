#pragma once
#include "Enemy.h"
class Healer:public Enemy
{
private:
	bool Direction;
public:

	Healer(int id, double health, int arrTime, double speed, double power, double ReloadPeriod);

	void Move() override;
	void HealEnemy(Enemy* enemy);
	 
	bool getDirection();
};

