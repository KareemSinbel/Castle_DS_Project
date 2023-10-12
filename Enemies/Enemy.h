#pragma once

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"


class GUI;
// Enemy is the base class of each type of enemy
// Enemy should be an abstract class in next phases
class Enemy
{

protected:
	 const int ID;         //Each enemy has a unique ID (sequence number)
	 const int ArrvTime;	//arrival time (time it starts to join battle)

	ENMY_STATUS status;	    //status of the enemy (inactive, inactive, frosted, killed)
	ENMY_TYPE Type;
	
	int Distance;	//Horizontal distance between enemy & the tower of its region
	                //Always positive (ranges from 2 to 60)
	const double Health;	//Enemy health
	const double FrostThreshold;
	const double FrostedTime;
	
	double CurrentHealth;
	double CurrentFrost;
	double TimeToUnfrost;
	 double Speed;
	 double Power;

	 int FirstShot = 0; //Time of first attacking by castle
	 int KilledTime; //Time of killing
	 int RldPeriod;
	 int EndReload;
	 //int totalice = 0;
public:
	//Enemy();
	Enemy(int id, double health,ENMY_TYPE type, int arrTime,double Speed,double Power, double RldPeriod, int d = MaxDistance);
	virtual ~Enemy();

	//Getters
	int GetID() const;
	ENMY_STATUS GetStatus() const;
	ENMY_TYPE GetType() const;
	int GetDistance() const;
	int GetArrvTime() const;
	int GetHealth() const;
	int GetRldTime() const;
	int GetSpeed() const;
	int GetKldTime() const;
	int GetPower() const;
	int GetfrstShotTime() const;
	bool IsFrosted() const;
	bool IsDead() const;

	//Setters
	void SetStatus(ENMY_STATUS);
	void SetDistance(int);
	//void setType(ENMY_TYPE);
	void setRldTime();

	//functions
	bool BeFrosted(double Frost, int); //Returns true when frosted
	bool BeDamaged(double Damage, int); //Returns true in case of enemy's death
	void BeHealed(double heal);
	void DecrementDist();
	bool ReduceFrostedTime();
	void Melt();
	void Reducereloadtime(); //reduce the time to end reloading;
	

	/*void settotalice(int x);
	int gettotalice();*/


	// Virtual Functions: ----------------

	virtual void Move() = 0;	//All enemies can move
	

};