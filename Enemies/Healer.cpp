#include "Healer.h"


Healer::Healer(int id, double health, int arrTime, double speed, double power, double ReloadPeriod) :Enemy(id, health, HEALER, arrTime, speed, power, ReloadPeriod)
{
	Direction = true;
}
 
void Healer::Move() 
{

	if (CurrentHealth < 0.5 * Health)
	{
		if((Distance+Speed)>MaxDistance)
		{
			Distance = MaxDistance;
			Direction = true;
		}
		else if ((Distance - Speed) < MinDistance)
		{
			Distance = MinDistance;
			Direction = false;
			
		}

		if (Direction)
		{
			Distance -= 0.5 * GetSpeed();
		}
		else
		{
			Distance += 0.5 * GetSpeed();
		}
	}
	
	else
	{
		if ((Distance + Speed) > MaxDistance)
		{
			Distance = MaxDistance;
			Direction = true;
		}
		else if ((Distance - Speed) < MinDistance)
		{
			Distance = MinDistance;
			Direction = false;

		}

		if (Direction)
		{
			Distance -= GetSpeed();
		}
		else
		{
			Distance +=  GetSpeed();
		}
	}
}

void Healer::HealEnemy(Enemy* enemy)
{
	if (GetRldTime() != 0) //must equal zero
		return;

	if (enemy->GetStatus() == ACTV)

	{
		int difference = enemy->GetDistance() - Distance; //distance between healer and enemy
		
		int k = 0;

		if (difference == 0)
		{
			k = 1;
		}
		else if (difference == 1)
		{
			k = 0.5;
		}
		else if (difference == 2)
		{
			k = 0.25;
		}

		double heal = 0.05 * Health + 0.01 * enemy->GetHealth() * k;


	    if (difference >= -2 && difference <= 0 && Direction == true)
		{
			enemy->BeHealed(heal);
		}
		else if (difference <= 2 && difference >= 0 && Direction == false)
		{
			enemy->BeHealed(heal);
		}
	}
}

bool Healer::getDirection()
{
	return Direction;
}
