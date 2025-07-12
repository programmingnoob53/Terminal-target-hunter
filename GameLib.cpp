#include <iostream>
#include "GameLib.h"
#include <sstream>
#include <cstdlib>
#include <cassert>

//it is safe to use a namespace in a source file
using namespace std;


namespace ChaseSpace
{
    int GenerateRand(int MinNum, int MaxNum)
    {
        int RangeNum = MaxNum - MinNum +1;
        return(rand()%RangeNum + MinNum);

    }
    void countArgs(int ArgsNum, std::string ProgramName)
    {
        if(ArgsNum != NumArgs)
        {
            cerr<<ProgramName<<" Usage: <Cols><Rows><ObstacleNum><PlayerEnergy>"<<endl;
            exit(ERR_CMD_ARGS);
        }
    }
    int ConvertStr(std::string str)
    {
        int Number;
        stringstream ssconvert;
        ssconvert<<str;
        ssconvert>>Number;
        if(ssconvert.fail())
        {
            cerr<<str<<" is not a number"<<endl;
            exit(ERR_CONVERT);
        }
        return Number;
    }
    void VerifyArgs(int Cols, int Rows, int Energy, int NumObstacle)
    {
        //avoid hardcoding values, create a const/constexpre instead
        if(Cols <5 || Rows<5)
        {
            cerr<<"<Cols><Rows> must be greater than 5"<<endl;
            exit(ERR_VERIFY);
        }
        if(Energy<10)
        {
            cerr<<"<Energy> must be atleast 10 units"<<endl;
            exit(ERR_VERIFY);
        }
        if(NumObstacle<4)
        {
            cerr<<"Number of obstacle must be 5 and Up"<<endl;
            exit(ERR_VERIFY);
        }
    }
    GameArray AllocMem(int cols, int rows)
    {
        GameArray ArrWorld;
        ArrWorld = new GameRows[rows];
        for(int k=0;k<rows;k++)
        {
            ArrWorld[k] = new GameTHings[cols];
            for(int i=0;i<cols;i++)
            {
                ArrWorld[k][i] = SPACE;
            }
        }
        return ArrWorld;
    }
    //place player
    void initPlayer(World &recWorld, int Energy)
    {
        int Col = GenerateRand(0,recWorld.cols-1);
        int Row = GenerateRand(0, recWorld.rows-1);
        recWorld.ArrayWorld[Row][Col] = PLAYER;
        recWorld.strPlayer.col = Col;
        recWorld.strPlayer.rows = Row;
        recWorld.strPlayer.Penergy = Energy;
    }
    int CountInWorld(GameTHings Thing, World &RecWorld)
    {
        int Number = 0;//we do not want to read carbagge(set to 0)
        for(int k=0;k<RecWorld.rows;k++)
        {
            for(int j=0;j<RecWorld.cols;j++)
            {
                if(RecWorld.ArrayWorld[k][j] == Thing)
                {
                    Number++;
                }
            }
        }
        return Number;
    }
    void PlaceInworld(GameTHings Thing, World &recWorld, int ThingNum)
    {
		//Place a Number of thing in the world(SPACE)
        int SpaceNUm = CountInWorld(SPACE, recWorld);//count the number of space in world 
        assert(SpaceNUm!=0);//SAFETY
        do
        {
            int col = GenerateRand(0, recWorld.cols-1);
            int row = GenerateRand(0, recWorld.rows-1);
            if(recWorld.ArrayWorld[row][col] == SPACE)
            {
                recWorld.ArrayWorld[row][col] = Thing;
                ThingNum--;
            }
        }while(ThingNum>0);
    }
    void initDoor(World &recWorld)
    {
		//this function Places a door in a 2-d World
        auto blnPlaced = false;
        do
        {
            int col = GenerateRand(0, recWorld.cols-1);
            int row = GenerateRand(0, recWorld.rows-1);
            if(recWorld.ArrayWorld[row][col] == SPACE)
            {
                blnPlaced = true;
                recWorld.RecDoor.col = col;
                recWorld.RecDoor.row = row;
                recWorld.ArrayWorld[row][col] = DOOR;
            }
        }while(!blnPlaced);
    }

    World initWorld(int col, int Row, int OBSTACLEnum, int Energy)
    {
		//initialize the world
        World recWorld;
        recWorld.ArrayWorld = AllocMem(col, Row);
        recWorld.cols = col;
        recWorld.rows = Row;
		//place player
        initPlayer(recWorld, Energy);
		//place obstacle
        PlaceInworld(OBSTACLE, recWorld, OBSTACLEnum);
		//place targets and door
        PlaceInworld(TARGET, recWorld, NumTargets);
        initDoor(recWorld);
        return recWorld;
    }
    void ShowWorld(World &RecWorld)
    {
        system("cls");//clear the screen
        assert(RecWorld.ArrayWorld!=nullptr);//safety check
        cout<<"Player Energy: "<<RecWorld.strPlayer.Penergy<<endl;
        for(int k=0;k<RecWorld.rows;k++)
        {
            for(int i=0;i<RecWorld.cols;i++)
            {
                cout<<GameEntity[RecWorld.ArrayWorld[k][i]];
            }
            cout<<endl;
        }
        cout<<endl;
        cout<<"Movement: w-Up, s-Down, a-Left, d-Right, q-Up_left, e-Up_right, z-Down_left, c-Down_right"<<endl;//controlls
        cout<<"Exit-X"<<endl;
    }
    bool isIngrd(World &recWorld, int Tcol,int Trow)
    {
		// to check if an object is in grd
        return(Tcol>=0&&Tcol<recWorld.cols&&Trow>=0&&Trow<recWorld.rows);
    }
	GameArray CopyArray(GameArray ArrayWorld, int cols, int rows)
	{
		assert(ArrayWorld!=nullptr);//if memory was allocated
		GameArray NewArr = AllocMem(cols, rows);//everything is space
		//copy word to word
		for(int k=0;k<rows;k++)
		{
			for(int j=0;j<cols;j++)
			{
				NewArr[k][j] = ArrayWorld[k][j];
			}
		}
		return NewArr;
	}
	//allowed if the functions do not have the same parameters
	void DeallocMem(GameArray& ArrWorld, int rows)
	{
		assert(ArrWorld!=nullptr);//check
		for(int k=0;k<rows;k++)
		{
			delete[] ArrWorld[k];
		}
		//the outer array
		delete[] ArrWorld;
		ArrWorld = nullptr;
	}
	void MoveTarget(World& recWorld)
	{
		GameArray ArrWorld = CopyArray(recWorld.ArrayWorld, recWorld.cols, recWorld.rows);
		for(int k=0;k<recWorld.rows;k++)
		{
			for(int j=0;j<recWorld.cols;j++)
			{
				//if there is a target at that position
				if(recWorld.ArrayWorld[k][j] == TARGET)
				{
					int chance = GenerateRand(1, 100);//numbers between 1 and 100
					if(chance <= ChanceMove)//each target has a chance(%) of moving
					{
						int Movedir = GenerateRand(UP, RIGHT);//using the enumeration called pDirection, UP->0, Right->3
						//it is not good to hardcode values
						int Tcol = j;
						int Trow  = k;
						switch(Movedir)
						{
							case UP:
							Trow--;
							break;
							case DOWN:
							Trow++;
							break;
							case LEFT:
							Tcol--;
							break;
							case RIGHT:
							Tcol++;
							break;
							default:
							break;
						}
						//check if a target is moving in bound
						if(isIngrd(recWorld, Tcol, Trow))
						{
							if(ArrWorld[Trow][Tcol] == SPACE)//can only move into a space
							{
								ArrWorld[Trow][Tcol] = TARGET;
								ArrWorld[k][j] = SPACE;
							}
						}
					}
				}
			}
		}
		recWorld.ArrayWorld = CopyArray(ArrWorld, recWorld.cols, recWorld.rows);//copy the world back
		DeallocMem(ArrWorld, recWorld.rows);//deallocate memory for temporary world
	}
    void MovePlayer(World &recWorld, Pdirection edir)
    {
        int pcol = recWorld.strPlayer.col;//avoid typing recWorld.strPlayer.col
        int prow = recWorld.strPlayer.rows;
		//player can move in all 8 direction
		MoveTarget(recWorld);
        switch(edir)
        {
        case UP:
            prow--;
            break;
        case DOWN:
            prow++;
            break;
        case LEFT:
            pcol--;
            break;
        case RIGHT:
            pcol++;
            break;
        case DOWN_LEFT:
            prow++;
            pcol--;
            break;
        case DOWN_RIGHT:
            prow++;
            pcol++;
            break;
        case UP_LEFT:
            prow--;
            pcol--;
            break;
        case UP_RIGHT:
            prow--;
            pcol++;
            break;
        }
        if(isIngrd(recWorld, pcol, prow))
        {
            int lRow = recWorld.strPlayer.rows;//previous col and row
            int Lcol = recWorld.strPlayer.col;
            if(recWorld.ArrayWorld[prow][pcol] == SPACE)//move in space
            {
                recWorld.ArrayWorld[prow][pcol] = PLAYER;
                recWorld.ArrayWorld[lRow][Lcol] = SPACE;
                recWorld.strPlayer.col = pcol;
                recWorld.strPlayer.rows = prow;
                recWorld.strPlayer.Penergy--;
            }
            else if(recWorld.ArrayWorld[prow][pcol] == TARGET)//move in target
            {
                recWorld.ArrayWorld[prow][pcol] = PLAYER;
                recWorld.ArrayWorld[lRow][Lcol] = SPACE;
                recWorld.strPlayer.col = pcol;
                recWorld.strPlayer.rows = prow;
                recWorld.strPlayer.Penergy+=GenerateRand(2, 5);//allocate random energy
            }
            else if(recWorld.ArrayWorld[prow][pcol] == OBSTACLE)
            {
                recWorld.strPlayer.Penergy--;
                cout<<"Ouch.Player cannot go there!"<<endl;
                system("pause");
            }
            else if(recWorld.ArrayWorld[prow][pcol] == DOOR)
            {
                int TARGETnum = CountInWorld(TARGET, recWorld);
                if(TARGETnum == 0)
                {
                    system("cls");
                    cout<<"yay, u beat the game!!"<<endl;
                    DeallocMem(recWorld);
                    exit(Victory);
                }
                else
                {
                    cout<<"Collect all the targets to open the door"<<endl;
                    system("pause");
                }
            }
        }
        else
        {
            cerr<<"Player cannot go out of world"<<endl;
            system("pause");
        }
		
    }
    void DeallocMem(World &RecWorld)
    {
		//work for record structures
        for(int k=0;k<RecWorld.rows;k++)
        {
            delete[] RecWorld.ArrayWorld[k];
        }
        delete[] RecWorld.ArrayWorld;
        RecWorld.ArrayWorld = nullptr;
    }
}
