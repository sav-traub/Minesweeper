#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Tiles.h"
using namespace std;

class Board {
    int width;
    int height;
    int numMines;
public:
    bool check;
    vector<Tile*> tiles;
    Board();
    int GetTileWidth();
    int GetTileHeight();
    int GetPixelWidth();
    int GetPixelHeight();
    void SetNumMines(int numMines);
    int GetNumMines();
    void GetData(string filePath, vector<int>& data);
    vector<int> RevealNeighbors(int tileNum, vector<int> hiddenTileInts);

    bool CheckBoard();
};