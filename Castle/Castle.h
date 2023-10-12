#pragma once
#include "..\Defs.h"
#include "../Enemies/Healer.h"


class Castle
{
	double Health,Power;
	double  maximumhealth;
	double castlethreshhold = 0.04*Health; // limit for freezing threshold
	bool frosted; // check if the casltle is frosted or not
	bool iskilled; // check if the castle is dead or not
	int enmyNum;
	double totaldmgtaken = 0; // total dmg taken by the enemy
	double totalfrosttaken = 0; // total amount of ice dealt by enemy
	int currenttimestep = 0;
	
	//
	// TODO: Add More Data Members As Needed
	//
	bool Frozen = false;
	int castletotaldmg = 0; // total dmg dealt to the enemy

public:
	//bool isForsted = 0;
	Castle();
	Castle(double h, int n, double CP);
	void SetHealth(double h);
	void SetPower(double pwr);
	void SetEnemyNumber(int n);
	double GetHealth() const;
	int gettotaldmg();
	void settime(int);


	

	//
	// TODO: Add More Member Functions As Needed
	//

	bool attack(Enemy* enemy);
	bool freezing(Enemy* enemy);
	void usehealertools(Enemy* enemy);
	bool isfrosted();
	void Bedamaged(double dmg); // damage taken by fighters
	void Befrosted(double dmg); // dmg taken by freezers
	bool getkilled();
	int getnumtoattack();
};

