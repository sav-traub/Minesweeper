#include "Tiles.h"

Tile::Tile()
{
    hasMine = false;
    isFlagged = false;
    isRevealed = false;
}

void Tile::MarkMine(bool b)
{
    hasMine = b;
}

void Tile::MarkFlag(bool b)
{
    isFlagged = b;
}

void Tile::MarkRevealed(bool b)
{
    isRevealed = b;
}

int Tile::GetNumNeighborMines()
{
    if(hasMine)
        return -1;
    int count = 0;
    for(int j = 0; j < neighbors.size(); j++)
    {
        if(neighbors[j]->hasMine)
            count++;
    }
    return count;
}

int Tile::GetNumNeighborFlags()
{
    if(isFlagged)
        return -1;
    int count = 0;
    for(int j = 0; j < neighbors.size(); j++)
    {
        if(neighbors[j]->isFlagged)
            count++;
    }
    return count;
}

void Tile::SetXCoord(int tileNum, int numCol)
{
    int val = tileNum % numCol;
    xCoord = val;
}

void Tile::SetYCoord(int tileNum, int numCol)
{
    int val = (tileNum - xCoord)/numCol;
    yCoord = val;
}

int Tile::GetXCoord()
{
    return xCoord;
}

int Tile::GetYCoord()
{
    return yCoord;
}
