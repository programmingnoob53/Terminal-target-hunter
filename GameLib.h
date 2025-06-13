#ifndef GAMELIB_H_INCLUDED
#define GAMELIB_H_INCLUDED
//implementation
#include <string>
namespace ChaseSpace
{
    constexpr int NumArgs = 5;//known at compile time
    constexpr int NumTargets = 9;
    constexpr char GameEntity[5] = {'@', '#', ' ' ,'T', 'E'};//E = door
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
    struct PlayerRec
    {
        int col;
        int rows;
        int Penergy;
    };
    struct DoorRec
    {
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

}


#endif // GAMELIB_H_INCLUDED
