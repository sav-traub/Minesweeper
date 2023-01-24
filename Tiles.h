#include <SFML/Graphics.hpp>
using namespace std;

class Tile
{
    int xCoord;
    int yCoord;
public:
    bool isFlagged;
    bool hasMine;
    bool isRevealed;
    bool victoryFlag;
    vector<Tile*> neighbors;

    Tile();

    void SetXCoord(int tileNum, int numCol);
    void SetYCoord(int tileNum, int numCol);
    int GetXCoord();
    int GetYCoord();

    int GetNumNeighborMines();
    int GetNumNeighborFlags();

    void MarkMine(bool b);
    void MarkFlag(bool b);
    void MarkRevealed(bool b);
};
