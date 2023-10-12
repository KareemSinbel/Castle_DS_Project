#include "Battle.h"
#include <time.h>
#include <fstream>
#include <sstream>
#include<iostream>

using namespace std;


int Battle::GetTimeStep()
{
	return CurrentTimeStep;
}

Battle::Battle()
{	
	EnemyCount = 0;
	KilledCount = 0;
	ActiveCount = 0;
	FrostedCount = 0;
	CurrentTimeStep = 0;
	pGUI = NULL;
}

Castle * Battle::GetCastle()
{
	return &BCastle;
}


void Battle::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
	GAME_STATUS status = DRAWN;
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		status = InteractiveMode();
		break;
	case MODE_STEP:
		status = StepByStepMode();
		break;
	case MODE_SLNT:
		status = SilentMode();
		break;
	};
	ExportData(status);
	delete pGUI;
	
}

GAME_STATUS Battle::InteractiveMode()
{
	int ActiveNumbers, 
		FrozenNumbers;
	pGUI->PrintMessage("Start the game");
	pGUI->waitForClick();
	CurrentTimeStep = 0;
	ImportEnemies();
	AddAllListsToDrawingList();
	pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.isfrosted());	
	GAME_STATUS gameStatus = IN_PROGRESS;
	while (gameStatus == IN_PROGRESS)
	{
		pGUI->waitForClick();
		CurrentTimeStep++;
		BCastle.settime(CurrentTimeStep);
		gameStatus = Battleaction();
		pGUI->ResetDrawingList();
		AddAllListsToDrawingList();
		FrozenNumbers = FrostedFighter + FrostedFreezer + FrostedHealer;
		ActiveNumbers = Q_Active.getCount() - FrozenNumbers;
		pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.isfrosted(), Q_Killed.getCount(),
			ActiveNumbers, FrozenNumbers, ActiveFighter, ActiveFreezer, ActiveHealer,
			FrostedFighter, FrostedHealer, FrostedFreezer, KilledFighter, KilledFreezer, KilledHealer);
	}
	pGUI->waitForClick();
	return gameStatus;
}


GAME_STATUS Battle::StepByStepMode()
{

	int ActiveNumbers,
		FrozenNumbers;
	CurrentTimeStep = 0;
	ImportEnemies();
	AddAllListsToDrawingList();
    pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.isfrosted());	
	GAME_STATUS gameStatus = IN_PROGRESS;
	while (gameStatus == IN_PROGRESS)
	{
		Sleep(250);
		CurrentTimeStep++;
		BCastle.settime(CurrentTimeStep);
		gameStatus = Battleaction();
		pGUI->ResetDrawingList();
		AddAllListsToDrawingList();
		FrozenNumbers = FrostedFighter + FrostedFreezer + FrostedHealer;
		ActiveNumbers = Q_Active.getCount() - FrostedCount;
		pGUI->UpdateInterface(CurrentTimeStep, BCastle.GetHealth(), BCastle.isfrosted(), Q_Killed.getCount(),
			ActiveNumbers, FrozenNumbers, ActiveFighter, ActiveFreezer, ActiveHealer,
			FrostedFighter, FrostedHealer, FrostedFreezer, KilledFighter, KilledFreezer, KilledHealer);
	}

	pGUI->waitForClick();
	return gameStatus;
}


GAME_STATUS Battle::SilentMode()
{
	CurrentTimeStep = 0;

	ImportEnemies();
	GAME_STATUS gameStatus = IN_PROGRESS;
	while (gameStatus == IN_PROGRESS)
	{
		CurrentTimeStep++;
		BCastle.settime(CurrentTimeStep);
		gameStatus = Battleaction();
	}

	pGUI->waitForClick();
	return gameStatus;
}


bool Battle::ImportEnemies() 
{
	ifstream inFile;
	inFile.open("Enemy2.txt");

	if (inFile.fail())
	{
		return false;
	}
	else
	{
		char* pch;
		char* context = nullptr;
		const int size = 300;
		char line[size];
		
		//variables to store data in
		int ID, ArrvTime, Dist;
		double Health, Speed, Power, RldPeriod;
		ENMY_TYPE type;

		//getting castle info from first line
		inFile.getline(line, size);

		pch = strtok(line, " ");
		BCastle.SetHealth(atoi(pch));
						
		pch = strtok(nullptr, " ");
		BCastle.SetEnemyNumber(atoi(pch));
		pch = strtok(nullptr, " ");
		BCastle.SetPower(atoi(pch));
		
		//getting number of enemies
		inFile.getline(line, size);
		pch = strtok(line, " ");
		NumEnmy = atoi(pch);

			
		
		//getting enemies info
		while (inFile.getline(line, size)) 
		{
			Enemy* pE;
				pch = strtok(line, " ");
				ID = atoi(pch);
				pch = strtok(nullptr, " ");
				type = (ENMY_TYPE)atoi(pch);
				pch = strtok(nullptr, " ");
				ArrvTime = atoi(pch);
				pch = strtok(nullptr, " ");
				Health = atoi(pch);
				pch = strtok(nullptr, " ");
				Power = atoi(pch);
				pch = strtok(nullptr, " ");
				RldPeriod = atoi(pch);
				pch = strtok(nullptr, " ");
				Speed = atoi(pch);

				//initiating enemies according to their types then adding them to inactive queue
		switch (type)
		{
		case FIGHTER:
			pE = new Fighter(ID,Health, ArrvTime, Speed, Power, RldPeriod);
			fighterNum++;
			EnemyCount++;
			break;
		case HEALER:
			pE = new Healer(ID,Health, ArrvTime, Speed, Power, RldPeriod);
			healerNum++;
			EnemyCount++;
			break;
		case FREEZER:
			pE = new Freezer(ID,Health, ArrvTime, Speed, Power, RldPeriod);
			freezerNum++;
			EnemyCount++;
		};
		Q_Inactive.enqueue(pE);
		}
		inFile.close();
	}
	return true;
}

void Battle::ExportData(GAME_STATUS gameStatus)
{
	ofstream fout("Play results.txt");
	Enemy* EnemyPointer;
	int TotalKills = Q_Killed.getCount();
	int FirstShotDelay = 0;
	int KillTimeDelay = 0;

	fout << "Game is:-  ";
	if (gameStatus == WIN)
	{
		fout << "WIN\n";
	}
	else if (gameStatus == LOSS)
	{
		fout << "LOSS\n";
	}
	else if (gameStatus == DRAWN)
	{
		fout << "DRAWN\n";
	}

	fout << "KTS  ID   FD   KD   LT\n";
	for (int i = 0; i < TotalKills; i++)
	{

		Q_Killed.dequeue(EnemyPointer);

		fout << EnemyPointer->GetKldTime() << "    "; //just to adjust the spaces in the required format 
		//outputting KTS   killed at time step
		if (EnemyPointer->GetKldTime() < 10)
		{
			fout << EnemyPointer->GetKldTime() << "    ";// 4 spaces for adjusting 
		}
		else if (EnemyPointer->GetKldTime() < 100)
		{
			fout << EnemyPointer->GetKldTime() << "   ";// 3 spaces for adjusting
		}
		else if (EnemyPointer->GetKldTime() < 1000)
		{
			fout << EnemyPointer->GetKldTime() << "  ";
		}
		fout << EnemyPointer->GetID() << "    ";
		//outputting ID  sequence number
		if (EnemyPointer->GetID() < 10)
		{
			fout << EnemyPointer->GetID() << "    ";// 4 spaces for adjusting 
		}
		else if (EnemyPointer->GetID() < 100)
		{
			fout << EnemyPointer->GetID() << "   "; // 3 spaces for adjusting
		}
		else if (EnemyPointer->GetID() < 1000)
		{
			fout << EnemyPointer->GetID() << "  ";// 2   space for adjusting
		}

		//outputting FD  first-shot delay
		FirstShotDelay += EnemyPointer->GetfrstShotTime() - EnemyPointer->GetArrvTime();
		if (EnemyPointer->GetfrstShotTime() - EnemyPointer->GetArrvTime() < 10)
		{
			fout << EnemyPointer->GetfrstShotTime() - EnemyPointer->GetArrvTime() << "    ";// 4 spaces for adjusting 
		}
		else if (EnemyPointer->GetfrstShotTime() - EnemyPointer->GetArrvTime() < 100)
		{
			fout << EnemyPointer->GetfrstShotTime() - EnemyPointer->GetArrvTime() << "   ";// 3 spaces for adjusting
		}
		else if (EnemyPointer->GetfrstShotTime() - EnemyPointer->GetArrvTime() < 1000)
		{
			fout << EnemyPointer->GetfrstShotTime() - EnemyPointer->GetArrvTime() << "  ";// 2   space for adjusting
		}

		//outputting KD  kill delay
		KillTimeDelay += EnemyPointer->GetKldTime() - EnemyPointer->GetfrstShotTime();
		if (EnemyPointer->GetKldTime() - EnemyPointer->GetfrstShotTime() < 10)
		{
			fout << EnemyPointer->GetKldTime() - EnemyPointer->GetfrstShotTime() << "    ";// 4 spaces for adjusting 
		}
		else if (EnemyPointer->GetKldTime() - EnemyPointer->GetfrstShotTime() < 100)
		{
			fout << EnemyPointer->GetKldTime() - EnemyPointer->GetfrstShotTime() << "   ";// 3 spaces for adjusting
		}
		else if (EnemyPointer->GetKldTime() - EnemyPointer->GetfrstShotTime() < 1000)
		{
			fout << EnemyPointer->GetKldTime() - EnemyPointer->GetfrstShotTime() << "  ";// 2   space for adjusting
		}

		//outputting LT  total enemy lifetime
		if (EnemyPointer->GetKldTime() - EnemyPointer->GetArrvTime() < 10)
		{
			fout << EnemyPointer->GetKldTime() - EnemyPointer->GetArrvTime() << "    "; // 4 spaces for adjusting 
		}
		else if (EnemyPointer->GetKldTime() - EnemyPointer->GetArrvTime() < 100)
		{
			fout << EnemyPointer->GetKldTime() - EnemyPointer->GetArrvTime() << "   ";  // 3 spaces for adjusting
		}
		else if (EnemyPointer->GetKldTime() - EnemyPointer->GetArrvTime() < 1000)
		{
			fout << EnemyPointer->GetKldTime() - EnemyPointer->GetArrvTime() << "  "; // 2   space for adjusting
		}

		fout << endl; //the end of the third line
	}
	fout << "THE CASTLE TOTAL DAMAGE IS:- ";
	fout << BCastle.gettotaldmg() << endl;

	if (gameStatus == WIN)
	{
		fout << "Total Number of Enemies = " << TotalKills << endl;
		fout << "Average of First-Shot Delay = " << (double)FirstShotDelay / TotalKills << endl;
		fout << "Average of Kill Delay = " << (double)KillTimeDelay / TotalKills << endl;
	}
	else
	{
		fout << "Number of Killed Enemies = " << TotalKills << endl;
		fout << "Number of Alive Enemies = " << EnemyCount - TotalKills << endl;
		fout << "Average First-Shot Delay for killed = " << (double)FirstShotDelay / TotalKills << endl;
		fout << "Average Kill Delay for killed = " << (double)KillTimeDelay / TotalKills << endl;
	}
}

void Battle::AddAllListsToDrawingList()
{
	//Add inactive queue to drawing list
	int c1, c2, c3, c4;
	Enemy* const* EnemyList = Q_Inactive.toArray(c1);
	Enemy* const* Activelist = Q_Active.toArray(c2);
	Enemy* const* Killedlist = Q_Killed.toArray(c3);
	/*Healer* const* HealerList = S_Healer.toArray(c4);*/
	for (int i = 0; i < c1; i++)
		pGUI->AddToDrawingList(EnemyList[i]);
	for (int i = 0; i < c2; i++)
		pGUI->AddToDrawingList(Activelist[i]);
	for (int i = 0; i < c3; i++)
		pGUI->AddToDrawingList(Killedlist[i]);
	/*for (int i = 0; i < c4; i++)
		pGUI->AddToDrawingList(HealerList[i]);*/
}



//check the inactive list and activate all enemies that has arrived
void Battle::ActivateEnemies()
{
	Enemy *pE;
	while( Q_Inactive.peekFront(pE) )	//as long as there are more inactive enemies
	{
		if(pE->GetArrvTime() > CurrentTimeStep )	//no more arrivals at current time
			return;
				
		Q_Inactive.dequeue(pE);	//remove enemy from the queue
		pE->SetStatus(ACTV);	//make status active
		Q_Active.enqueue(pE);
		ActiveCount++;
		
		/*switch (pE->GetType())
		{
		case HEALER:
			S_Healer.push(dynamic_cast<Healer*> (pE));
			healerNum++;
			break;
		case FIGHTER:
			Q_Fighter.insert(dynamic_cast<Fighter*> (pE), dynamic_cast<Fighter*> (pE)->getPriority());
			fighterNum++;
			break;
		case FREEZER:
			Q_Freezer.enqueue(dynamic_cast<Freezer*> (pE));
			freezerNum++;
			break;
		};*/
			
	}
}



GAME_STATUS Battle::Battleaction()
{
	ActivateEnemies();
	Enemy* activeenemy;
	Fighter* fighter;
	Healer* healer;
	Freezer* freezer;
	Queue<Enemy*> tmp; //Temporarly Queue have all enemy types
	ActiveCount = Q_Active.getCount();
	//Filling the other Queues,stacks .. etc depend on the enemy type
	for (int i = 0; i < ActiveCount; i++)
	{
		Q_Active.dequeue(activeenemy);
		if (activeenemy->IsFrosted() == false)
		{
			activeenemy->Move();
			activeenemy->Melt();
		}
		fighter = dynamic_cast<Fighter*>(activeenemy);
		healer = dynamic_cast<Healer*>(activeenemy);
		freezer = dynamic_cast<Freezer*>(activeenemy);
		if (fighter != nullptr)
		{
			tmp.enqueue(fighter);
			Q_Fighter.insert(fighter, fighter->getPriority());
			if (fighter->IsFrosted() == true)
			{
				if (fighter->ReduceFrostedTime() == false)
				{
					//FrostedFighter++;
					FrostedCount++;
					//ActiveCount--;
				}
				else if (fighter->GetRldTime() == 0)
				{
					fighter->AttackCastle(&BCastle);
					fighter->setRldTime();
				}
				else
				{
					fighter->Reducereloadtime();
				}
			}
		}
		else if (freezer != nullptr)
		{
			tmp.enqueue(freezer);
			Q_Freezer.enqueue(freezer);
			if (freezer->IsFrosted() == true)
			{
				if (freezer->ReduceFrostedTime() == false)
				{
					//FrostedCount++;
					FrostedFreezer++;
					//ActiveCount--;
				}
				else if (freezer->GetRldTime() == 0)
				{
					freezer->FreezeCastle(&BCastle);
					freezer->setRldTime();
				}
				else
				{
					freezer->Reducereloadtime();
				}
			}
		}
		else if (healer != nullptr)
		{
			tmp.enqueue(healer);
			S_Healer.push(healer);
		}
		Q_Active.enqueue(activeenemy);
	}


	//getting number of activated fighters ..etc
	 ActiveHealer = S_Healer.getCount();
	 ActiveFighter = Q_Fighter.size();
	 ActiveFreezer = Q_Freezer.getCount();

	//looping on the healers and checks if his conditions true or not to heal the others
	for (int i = 0; i < ActiveHealer; i++)
	{
		S_Healer.pop(healer);
		// Checks if the healer is frosted or not return true if frosted
		if (healer->IsFrosted())
		{
			if (!healer->ReduceFrostedTime())
			{
				//FrostedCount++;
				FrostedHealer++;
				//ActiveCount--;
			}
		}
		else
		{
			int count = tmp.getCount();
			Enemy* temp;
			for (int i = 0; i < count; i++)
			{
				tmp.dequeue(temp);

				//checks the distance between the healer and the other enemy
				if (temp->GetDistance() == healer->GetDistance() || abs(temp->GetDistance() - healer->GetDistance()) == 2 || abs(temp->GetDistance() - healer->GetDistance()) == 1)
				{
					healer->HealEnemy(temp);
				}
				tmp.enqueue(temp);
			}
		}
	}

	int N = BCastle.getnumtoattack();
	srand(time(NULL));
	int iceorbullet = (rand() % 100) + 1; //probability for attacking with bullet or ice by castle
	//attacking with bullets
	if (iceorbullet > 20)
	{
		if (ActiveFighter >= N)
		{
			for (int i = 0; i < N; i++)
			{
				if (Q_Fighter.size() == 0)
				{
					break;
				}
				Q_Fighter.dequeueMax(fighter);
				if (BCastle.attack(fighter) == true)
				{
					if (fighter->GetStatus() == FRST)
					{
						FrostedFighter--;
					}
					Q_Killed.enqueue(fighter);
					KilledFighter++;
				}
			}
		}
		else if (ActiveFighter < N)
		{
			for (int i = 0; i < N - ActiveFighter; i++)
			{
				if (S_Healer.getCount() == 0)
				{
					break;
				}
				S_Healer.pop(healer);
				if (BCastle.attack(healer) == true)
				{
					if (healer->GetStatus() == FRST)
					{
						FrostedHealer--;
					}
					Q_Killed.enqueue(healer);
					BCastle.usehealertools(healer);
					KilledHealer++;
				}
			}
		}
		else if (ActiveFighter + ActiveHealer < N)
		{
			for (int i = 0; i < N- (ActiveFighter + ActiveHealer); i++)
			{
				if (Q_Freezer.getCount() == 0)
				{
					break;
				}
				Q_Freezer.dequeue(freezer);
				if (BCastle.attack(freezer) == true)
				{
					if (freezer->GetStatus() == FRST)
					{
						FrostedFreezer--;
					}
					Q_Killed.enqueue(freezer);
					KilledFreezer++;
				}
			}
		}
		else
		{
			for (int i = 0; i < N; i++)
			{
				if (Q_Fighter.size() == 0)
				{
					break;
				}
				Q_Fighter.dequeueMax(fighter);
				if (BCastle.attack(fighter) == true)
				{
					if (fighter->GetStatus() == FRST)
					{
						FrostedFighter--;
					}
					Q_Killed.enqueue(fighter);
					KilledFighter++;
				}
			}
			
			if (ActiveFighter < N)
			{
				for (int i = 0; i < N - ActiveFighter; i++)
				{
					if (S_Healer.getCount() == 0)
					{
						break;
					}
					S_Healer.pop(healer);
					if (BCastle.attack(healer) == true)
					{
						if (healer->GetStatus() == FRST)
						{
							FrostedHealer--;
						}
						Q_Killed.enqueue(healer);
						BCastle.usehealertools(healer);
						KilledHealer++;
					}
				}
			}
			if (ActiveFighter + ActiveHealer < N)
			{
				for (int i = 0; i < N - (ActiveFighter + ActiveHealer); i++)
				{
					if (Q_Freezer.getCount() == 0)
					{
						break;
					}
					Q_Freezer.dequeue(freezer);
					if (BCastle.attack(freezer) == true)
					{
						if (freezer->GetStatus() == FRST)
						{
							FrostedFreezer--;
						}
						Q_Killed.enqueue(freezer);
						KilledFreezer++;
					}
				}
			}
		}
	}
	//attacking with ice
	else if (iceorbullet <= 20)
	{
		//freezing the fighter
		if (ActiveFighter >= N)
		{
			for (int i = 0; i < N; i++)
			{
				if (Q_Fighter.size() == 0)
				{
					break;
				}
				Q_Fighter.dequeueMax(fighter);
				if (fighter->IsFrosted() == false)
				{
					if (BCastle.freezing(fighter) == true)
					{
						FrostedFighter++;
					}
				}
			}
		}
		//freezing the healer
		else if (ActiveFighter < N)
		{
			for (int i = 0; i < N - ActiveFighter; i++)
			{
				if (S_Healer.getCount() == 0)
				{
					break;
				}
				S_Healer.pop(healer);
				if (healer->IsFrosted() == false)
				{
					if (BCastle.freezing(healer) == true)
					{
						FrostedHealer++;
					}
				}
			}
		}
		//freezing the freezers
		else if (ActiveFighter + ActiveHealer < N)
		{
			for (int i = 0; i < N - (ActiveFighter + ActiveHealer); i++)
			{
				if (Q_Freezer.getCount() == 0)
				{
					break;
				}
				Q_Freezer.dequeue(freezer);
				if (freezer->IsFrosted() == false)
				{
					if (BCastle.freezing(freezer) == true)
					{
						FrostedFreezer++;
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < N; i++)
			{
				if (Q_Fighter.size() == 0)
				{
					break;
				}
				Q_Fighter.dequeueMax(fighter);
				if (fighter->IsFrosted() == false)
				{
					if (BCastle.freezing(fighter) == true)
					{
						FrostedFighter++;
					}
				}
			}

			if (ActiveFighter < N)
			{
				for (int i = 0; i < N - ActiveFighter; i++)
				{
					if (S_Healer.getCount() == 0)
					{
						break;
					}
					S_Healer.pop(healer);
					if (healer->IsFrosted() == false)
					{
						if (BCastle.freezing(healer) == true)
						{
							FrostedHealer++;
						}
					}
				}
			}
			//freezing the freezers
			if (ActiveFighter + ActiveHealer < N)
			{
				for (int i = 0; i < N - (ActiveFighter + ActiveHealer); i++)
				{
					if (Q_Freezer.getCount() == 0)
					{
						break;
					}
					Q_Freezer.dequeue(freezer);
					if (freezer->IsFrosted() == false)
					{
						if (BCastle.freezing(freezer) == true)
						{
							FrostedFreezer++;
						}
					}
				}
			}
		}
	}
	ActiveFighter = ActiveFighter - FrostedFighter - KilledFighter;
	ActiveHealer = ActiveHealer - FrostedHealer - KilledHealer;
	ActiveFreezer = ActiveFreezer - FrostedFreezer - KilledFreezer;

	for (int i = 0; i < ActiveCount; i++)
	{
		Q_Active.dequeue(activeenemy);
		if (!activeenemy->IsDead())
		{
			Q_Active.enqueue(activeenemy);
		}
		else
		{
			Q_Killed.enqueue(activeenemy);
		}
	}

	//checks the game status
	if (BCastle.GetHealth() == 0 && EnemyCount == Q_Killed.getCount())
	{
		return DRAWN;
	}

	else if (BCastle.GetHealth() == 0)
	{
		return LOSS;
	}

	else if (EnemyCount == Q_Killed.getCount())
	{
		return WIN;
	}
	else
	{
		return IN_PROGRESS;
	}
}