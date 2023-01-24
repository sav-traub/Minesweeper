#include <vector>
#include <iostream>
#include <fstream>
#include "Board.h"
using namespace std;

Board::Board()
{

}

void Board::GetData(string filePath, vector<int>& data)
{
    ifstream file;
    file.open(filePath, ios::in);

    if(file.is_open())
    {
        int numColumns;
        string tempNumColumns;
        getline(file, tempNumColumns);
        numColumns = stoi(tempNumColumns);
        data.push_back(numColumns);

        int numRows;
        string tempNumRows;
        getline(file, tempNumRows);
        numRows = stoi(tempNumRows);
        data.push_back(numRows);

        int numMines;
        string tempNumMines;
        getline(file, tempNumMines);
        numMines = stoi(tempNumMines);
        data.push_back(numMines);

        for (int i: data)
            cout << i << endl;
    }

    this->width = data[0] * 32;
    this->height = (data[1] * 32) + 88;
    this->numMines = data[2];
}

int Board::GetTileWidth()
{
    return this->width/32;
}

int Board::GetTileHeight()
{
    return (this->height-88)/32;
}

int Board::GetPixelHeight()
{
    return this->height;
}

int Board::GetPixelWidth()
{
    return this->width;
}

void Board::SetNumMines(int numMines)
{
    this->numMines = numMines;
}

int Board::GetNumMines()
{
    return this->numMines;
}

vector<int> Board::RevealNeighbors(int tileNum, vector<int> hiddenTileInts)
{
    for (int i = 0; i < tiles[tileNum]->neighbors.size(); i++)
    {
        if(!tiles[tileNum]->neighbors[i]->isFlagged)
            hiddenTileInts.push_back(tiles[tileNum]->neighbors[i]->GetYCoord()*GetTileWidth() + tiles[tileNum]->neighbors[i]->GetXCoord());

        //make sure tile is empty
        if (!tiles[tileNum]->neighbors[i]->hasMine && tiles[tileNum]->neighbors[i]->GetNumNeighborMines() == 0
        && !tiles[tileNum]->neighbors[i]->isRevealed && !tiles[tileNum]->neighbors[i]->isFlagged)
        {
            tiles[tileNum]->neighbors[i]->isRevealed = true;
            hiddenTileInts = RevealNeighbors((tiles[tileNum]->neighbors[i]->GetXCoord() + tiles[tileNum]->neighbors[i]->GetYCoord()*GetTileWidth()), hiddenTileInts);
        }
    }

    return hiddenTileInts;
}

bool Board::CheckBoard()
{

    for (int i = 0; i < tiles.size(); i++) //loop through all tiles
    {
        //if not revealed and does not have mine, not good
        if(!tiles[i]->isRevealed && !tiles[i]->hasMine)
            return false;
    }

    return true;
}
