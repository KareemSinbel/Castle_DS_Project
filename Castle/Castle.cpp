#include "Castle.h"


Castle::Castle() 
{
}

Castle::Castle(double h, int n, double CP):enmyNum(n),Power(CP) 
{
	SetHealth(h);
	maximumhealth = h;
}

void Castle::SetHealth(double h)
{
	if(h > 0)
		Health = h;
	else
		Health = 0; // killed
}

void Castle::SetPower(double pwr)
{
	Power = pwr;
}

void Castle::SetEnemyNumber(int n)
{
	enmyNum = n;
}

double Castle::GetHealth() const
{
	return Health;
}

int Castle::gettotaldmg()
{
	return castletotaldmg;
}

void Castle::settime(int x)
{
	currenttimestep = x;
}


bool Castle::attack(Enemy* enemy)
{
		//srand(time(NULL));
		//int icefire = rand() % 100;
		//if (icefire <= 20)
		//{
		//	if (enemy->GetStatus() != FRST)
		//	{
		//		/*if (enemy->gettotalice() >= 5)
		//		{
		//			enemy->SetStatus(FRST);
		//		}
		//		else
		//		{
		//			enemy->settotalice(1);
		//		}*/
		//		double frost = Power / enemy->GetDistance();
		//		return enemy->BeFrosted(frost); // returns true if frosted
		//	}
		//}
		//else
		//{
			Healer* healer = dynamic_cast<Healer*>(enemy);
			int K = 0;
			if (Frozen == true)
			{
				return false;
			}

			if (healer != nullptr)
			{
				K = 2;
			}
			else
			{
				K = 1;
			}
			double dmg = Power / (enemy->GetDistance() * K);
			castletotaldmg += dmg;
			return enemy->BeDamaged(dmg,currenttimestep); //returns true if killed
		//}
}


bool Castle::freezing(Enemy* enemy)
{
	if (Frozen == true)
	{
		return false;
	}
	if (enemy->GetStatus() != FRST)
	{
		/*if (enemy->gettotalice() >= 5)
		{
			enemy->SetStatus(FRST);
		}
		else
		{
			enemy->settotalice(1);
		}*/
		double frost = Power / enemy->GetDistance();
		return enemy->BeFrosted(frost, currenttimestep); // returns true if frosted
	}
}


void Castle::usehealertools(Enemy* enemy)
{
	Healer* healer = dynamic_cast<Healer*>(enemy);
	if (healer == nullptr)
	{
		return;
	}
	else
	{
		if (healer->IsDead() == true && healer->GetDistance() <= 5)
		{
			Health = 0.03 * maximumhealth;
		}
	}
}


bool Castle::isfrosted()
{
	return frosted;
}

bool Castle::getkilled()
{
	return iskilled;
}


void Castle::Bedamaged(double dmg)
{
	totaldmgtaken += dmg;
	Health -= dmg;
	if (Health <= 0)
	{
		Health = 0;
		iskilled = true;
	}
}


void Castle::Befrosted(double dmg)
{
	totalfrosttaken += dmg;
	if (totalfrosttaken >= castlethreshhold)
	{
		totalfrosttaken = 0;
		frosted = true;;
	}
}

int Castle::getnumtoattack()
{
	return enmyNum;
}