#include "Freezer.h"

Freezer :: Freezer(int id, double health, int arrTime, double speed, double power, double ReloadPeriod) :Enemy(id, health, FREEZER, arrTime, speed, power, ReloadPeriod)
{
}

void Freezer::Move() 
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

void Freezer::FreezeCastle(Castle* castle) 
{
	if (GetRldTime() != 0) //must equal zero
		return;

	double k;
	if (CurrentHealth < 0.5 * Health)
		k = 0.5;
	else
		k = 1.0;

	double frost = (k * Power) / Distance; //CHECK
	castle->Befrosted(frost);
}