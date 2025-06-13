#include <iostream>
#include "GameLib.h"
#include <cctype>// to use the tollower function
#include <ctime>
#include <cstdlib>
//This is my first c++ console game
//!!in this game the player must collect all the targets to exit the do

/**
         @brief Terminal-based grid game where the player collects targets while managing energy.

         i want to clarify that this is NOT my original idea.
         This idea was inspired by my lectures at uj.

        i want to give a shoutout to lectures:
                - Sithungu,
                - Ogwok
                - & Coulter
**/
using namespace ChaseSpace;
using namespace std;

int main(int argc, char** argv)
{
    srand(time(nullptr));//seed the random number generator


    countArgs(argc, argv[0]);
    int cols = ConvertStr(argv[1]);
    int Rows = ConvertStr(argv[2]);
    int ObstacleNum = ConvertStr(argv[3]);
    int Penergy = ConvertStr(argv[4]);
    //verify args
    VerifyArgs(cols, Rows, Penergy, ObstacleNum);
    World RecWorld;
    RecWorld = initWorld(cols, Rows, ObstacleNum, Penergy);

    auto blnShow = true;
    do
    {
        ShowWorld(RecWorld);
        if(RecWorld.strPlayer.Penergy == 0)
        {
            cout<<"Player has no energy"<<endl;
            DeallocMem(RecWorld);
            exit(Defeat);
        }
        char chInput = '\0';
        cin>>chInput;
        switch(tolower(chInput))
        {
        case 'a':
            MovePlayer(RecWorld, LEFT);
            break;
        case 's':
            MovePlayer(RecWorld, DOWN);
            break;
        case 'd':
            MovePlayer(RecWorld, RIGHT);
            break;
        case 'w':
            MovePlayer(RecWorld, UP);
            break;
        case 'q':
            MovePlayer(RecWorld, UP_LEFT);
            break;
        case 'e':
            MovePlayer(RecWorld, UP_RIGHT);
            break;
        case 'z':
            MovePlayer(RecWorld, DOWN_LEFT);
            break;
        case 'c':
            MovePlayer(RecWorld, DOWN_RIGHT);
            break;
        case 'x':
            cout<<"Exiting...."<<endl;
            blnShow = false;
            break;
        default:
            cerr<<"incorrect input..."<<endl;
            system("pause");
            break;
        }

    }while(blnShow);

    //!!free memory
    DeallocMem(RecWorld);
    return TERMINATE;
}
