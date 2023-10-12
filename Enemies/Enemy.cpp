#include "Enemy.h"
#include<iostream>


using namespace std;
//Enemy::Enemy()
//{
//	SetDistance(MaxDistance);
//	SetStatus(INAC);
//}

Enemy::Enemy(int id, double health,ENMY_TYPE type, int arrTime, double speed, double power, double ReloadPeriod, int distance)
	:ID(id),Health(health),ArrvTime(arrTime),Type(type),Speed(speed),Power(power),RldPeriod(ReloadPeriod),Distance(distance)
	, FrostThreshold(0.3*Health), FrostedTime(RldPeriod)
{
	SetStatus(INAC);
	EndReload = 0;
	CurrentFrost = 0;
	TimeToUnfrost = 0;
	CurrentHealth =health;
}


Enemy::~Enemy()
{
}

int Enemy::GetID() const
{
	return ID;
}


void Enemy::SetStatus(ENMY_STATUS s)
{
	status = s;
}


ENMY_STATUS Enemy::GetStatus() const
{
	return status;
}


void Enemy::DecrementDist()
{
	if (Distance - Speed > MinDistance)
		Distance -= Speed;
	else if (Distance > MinDistance) {
		if (Type == HEALER)Speed = -Speed;
		Distance = MinDistance;
	}
	if (Distance > MaxDistance) {
		Distance = MaxDistance;
		Speed = -Speed;
	}
}

bool Enemy::ReduceFrostedTime()
{
	if (TimeToUnfrost == 0)
	{
		if (!IsDead())
			SetStatus(ACTV);
		return true;
	}
	else
	{
		TimeToUnfrost--;
		return false;
	}
}

void Enemy::Melt()
{
	if (CurrentFrost == 0)
		return;
	CurrentFrost -= 0.1 * (CurrentHealth + Power);
}

void Enemy::SetDistance(int d)
{
	if (d >= MinDistance && d <= MaxDistance)
		Distance = d;
	else
		Distance = MaxDistance;
}

void Enemy::setRldTime()
{
	EndReload = RldPeriod;
}

bool Enemy::BeFrosted(double frost, int currenttime)
{
	if (FirstShot == 0)
		FirstShot = currenttime;
	
	else if (CurrentFrost + frost >= FrostThreshold)
	{
		CurrentFrost = 0;
		SetStatus(FRST);
		return true; //frosted enemy
	}
	else
	{
		CurrentFrost += frost;
		return false; //still not frosted
	}
}

bool Enemy::BeDamaged(double Damage,int currenttime)
{
	CurrentHealth -= Damage;
	
	if (FirstShot == 0)
		FirstShot = currenttime; //detecting time of first shot
	
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		SetStatus(KILD);
		KilledTime = currenttime;
		return true; //Dead enemy
	}
	else
		return false; //Still alive
}

void Enemy::BeHealed(double Heal)
{
	CurrentHealth += Heal;
}



int Enemy::GetDistance() const
{
	return Distance;
}

int Enemy::GetHealth() const
{
	return CurrentHealth;
}

int Enemy::GetRldTime() const
{
	return EndReload;
}

int Enemy::GetSpeed() const
{
	if (CurrentFrost >= FrostThreshold)
		return 0.7 * Speed;
	else
		return Speed;
}

int Enemy::GetKldTime() const
{
	return KilledTime;
}

int Enemy::GetPower() const
{
	return Power;
}

int Enemy::GetfrstShotTime() const
{
	return FirstShot;
}

bool Enemy::IsFrosted() const
{
	if (GetStatus() == FRST)
		return true;
	else
		return false;
}

bool Enemy::IsDead() const
{
	if (GetStatus() == KILD)
		return true;
	else
		return false;
}



int Enemy::GetArrvTime() const
{
	return ArrvTime;
}

ENMY_TYPE Enemy::GetType() const
{
	return Type;
}


//void Enemy::settotalice(int x)
//{
//	totalice += x;
//}
//
//int Enemy::gettotalice()
//{
//	return totalice;
//}

void Enemy::Reducereloadtime()
{
	if (EndReload == 0)
	{
		return;
	}
	else
	{
		EndReload--;
	}
}