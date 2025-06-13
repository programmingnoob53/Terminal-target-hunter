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
        int Number = 0;//we do not want to read carbagge
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
        int SpaceNUm = CountInWorld(SPACE, recWorld);
        assert(SpaceNUm!=0);
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
        World recWorld;
        recWorld.ArrayWorld = AllocMem(col, Row);
        recWorld.cols = col;
        recWorld.rows = Row;
        initPlayer(recWorld, Energy);
        PlaceInworld(OBSTACLE, recWorld, OBSTACLEnum);
        PlaceInworld(TARGET, recWorld, NumTargets);
        initDoor(recWorld);
        return recWorld;
    }
    void ShowWorld(World &RecWorld)
    {
        system("cls");
        assert(RecWorld.ArrayWorld!=nullptr);
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
        cout<<"Movement: w-Up, s-Down, a-Left, d-Right, q-Up_left, e-Up_right, z-Down_left, c-Down_right"<<endl;
        cout<<"Exit-X"<<endl;
    }
    bool isIngrd(World &recWorld, int Tcol,int Trow)
    {
        return(Tcol>=0&&Tcol<recWorld.cols&&Trow>=0&&Trow<recWorld.rows);
    }
    void MovePlayer(World &recWorld, Pdirection edir)
    {
        int pcol = recWorld.strPlayer.col;
        int prow = recWorld.strPlayer.rows;
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
            if(recWorld.ArrayWorld[prow][pcol] == SPACE)
            {
                recWorld.ArrayWorld[prow][pcol] = PLAYER;
                recWorld.ArrayWorld[lRow][Lcol] = SPACE;
                recWorld.strPlayer.col = pcol;
                recWorld.strPlayer.rows = prow;
                recWorld.strPlayer.Penergy--;
            }
            else if(recWorld.ArrayWorld[prow][pcol] == TARGET)
            {
                recWorld.ArrayWorld[prow][pcol] = PLAYER;
                recWorld.ArrayWorld[lRow][Lcol] = SPACE;
                recWorld.strPlayer.col = pcol;
                recWorld.strPlayer.rows = prow;
                recWorld.strPlayer.Penergy+=3;
            }
            else if(recWorld.ArrayWorld[prow][pcol] == OBSTACLE)
            {
                recWorld.strPlayer.Penergy--;
                cout<<"ouch.Player cannot go there!"<<endl;
                system("pause");
            }
            else if(recWorld.ArrayWorld[prow][pcol] == DOOR)
            {
                int TARGETnum = CountInWorld(TARGET, recWorld);
                if(TARGETnum == 0)
                {
                    system("cls");
                    cout<<"yay, u beat the game"<<endl;
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
        for(int k=0;k<RecWorld.rows;k++)
        {
            delete[] RecWorld.ArrayWorld[k];
        }
        delete[] RecWorld.ArrayWorld;
        RecWorld.ArrayWorld = nullptr;
    }
}
