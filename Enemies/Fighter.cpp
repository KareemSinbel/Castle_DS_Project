#include "Fighter.h"

Fighter::Fighter(int id, double health, int arrTime, double speed, double power, double ReloadPeriod) :Enemy(id,health, FIGHTER, arrTime, speed, power, ReloadPeriod)
{
}

int Fighter::getPriority() const
{
	int x;
	if (IsFrosted())
		x = 0.5;
	else
		x = 1;
	return (Power * Health * x) / (Distance);
}

void Fighter::Move() 
{
	if (Distance <= MinDistance)
		return;
	else 
	{
		if (CurrentHealth < 0.5 * Health)
		{
			Distance -= 0.5 * GetSpeed();
		}
		else
		{
			Distance -= GetSpeed();
		}
		if (Distance < MinDistance)
			Distance = MinDistance;
	}
}

void Fighter::AttackCastle(Castle* castle)
{
	double x;
	
	if (GetRldTime() != 0)
		return;  //Reload Period must be equal zero for the fighter to be able to fight
	
	//checking if fighter is in maximum or half power
	if (CurrentHealth < 0.5 * Health)
		x = 0.5;
	else 
		x = 1.0;

	//TO-DO modifications in case of Super Solider
	double damage = (x * Power) / Distance;
	castle->Bedamaged(damage);
}

