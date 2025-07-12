#ifndef GAMELIB_H_INCLUDED
#define GAMELIB_H_INCLUDED
//implementation
#include <string>
namespace ChaseSpace
{
    constexpr int NumArgs = 5;//known at compile time
    constexpr char GameEntity[5] = {'@', '#', ' ' ,'T', 'E'};//E = door
	constexpr int ChanceMove = 70;//chance a target will move
	constexpr int NumTargets = 10;
    enum GameCodes
    {
        TERMINATE,
        ERR_CONVERT,
        ERR_VERIFY,
        ERR_CMD_ARGS,
        Victory,
        Defeat,
    };
    enum GameTHings//just an integer
    {
        PLAYER,
        OBSTACLE,
        SPACE,
        TARGET,
        DOOR,
    };
    enum Pdirection
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        UP_RIGHT,
        UP_LEFT,
        DOWN_RIGHT,
        DOWN_LEFT,
    };
    //PROTOTYPES
    void countArgs(int ArgsNum, std::string ProgramName);
    int ConvertStr(std::string str);
    void VerifyArgs(int Cols, int Rows, int Energy, int NumObstacle);
    typedef GameTHings* GameRows;
    typedef GameRows* GameArray;
	
    void countArgs(int ArgsNum, std::string ProgramName);// count the number of arguments
    int ConvertStr(std::string str);//convert a string number to a number
    void VerifyArgs(int Cols, int Rows, int Energy, int NumObstacle);
    typedef GameTHings* GameRows;
    typedef GameRows* GameArray;
	//a player must have the following
    struct PlayerRec
    {
		//to reduce the number of for-loops
        int col;
        int rows;
        int Penergy;
    };
    struct DoorRec
    {

		//Not required since we have one door
        int col;
        int row;
    };
    struct World
    {
        int cols;
        int rows;
        PlayerRec strPlayer;
        DoorRec RecDoor;
        GameArray ArrayWorld;

    };
    World initWorld(int col, int Row, int OBSTACLEnum, int Energy);
    void ShowWorld(World &RecWorld);
    void DeallocMem(World &RecWorld);
    void MovePlayer(World &recWorld, Pdirection edir);

	void MoveTarget(World& recWorld);

}


#endif // GAMELIB_H_INCLUDED
