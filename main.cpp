#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include <unordered_map>
#include "TextureManager.h"
#include "Board.h"
#include "Random.h"
using namespace std;

int main()
{
    vector<int> data;
    auto* b = new Board();
    b->GetData("boards/config.cfg", data);
    b->SetNumMines(data[2]);

    int numCol = b->GetTileWidth();
    int numRow = b->GetTileHeight();
    int numTiles = numCol * numRow;

    bool debugModeOn = false;
    bool victory = false;
    bool defeat = false;
    bool norm = true;

    bool negDig = false;
    int onesDig = b->GetNumMines() % 10; //0
    int tensDig = (b->GetNumMines() / 10) % 10; //5
    int hundredsDig = (b->GetNumMines() / 100) % 10;

    vector<sf::Sprite> mines;
    vector<sf::Sprite> minesDebugMode;
    vector<sf::Sprite> hiddenTiles;
    vector<sf::Sprite> victoryHiddenTiles;
    vector<sf::Sprite> revealedTiles;
    vector<sf::Sprite> flags;
    vector<sf::Sprite> victoryFlags;
    vector<sf::Sprite> tempFlags;
    vector<sf::Sprite> nums;
    vector<sf::Sprite> revealMines;

    sf::RenderWindow window(sf::VideoMode(b->GetPixelWidth(), b->GetPixelHeight()), "Board Setup");

    //Creating Sprites
    sf::Sprite hiddenTile(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite victoryTile(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite revealed(TextureManager::GetTexture("tile_revealed"));
    sf::Sprite mine(TextureManager::GetTexture("mine"));
    sf::Sprite flag(TextureManager::GetTexture("flag"));
    sf::Sprite victoryFlag(TextureManager::GetTexture("flag"));
    sf::Sprite num1(TextureManager::GetTexture("number_1"));
    sf::Sprite num2(TextureManager::GetTexture("number_2"));
    sf::Sprite num3(TextureManager::GetTexture("number_3"));
    sf::Sprite num4(TextureManager::GetTexture("number_4"));
    sf::Sprite num5(TextureManager::GetTexture("number_5"));
    sf::Sprite num6(TextureManager::GetTexture("number_6"));
    sf::Sprite num7(TextureManager::GetTexture("number_7"));
    sf::Sprite num8(TextureManager::GetTexture("number_8"));
    sf::Sprite smiley(TextureManager::GetTexture("face_happy"));
    smiley.setPosition((window.getSize().x/2.0f)-64.0f,window.getSize().y-64.0f);

    sf::Sprite test1(TextureManager::GetTexture("test_1"));
    test1.setPosition(window.getSize().x-192.0f,window.getSize().y-64.0f);

    sf::Sprite test2(TextureManager::GetTexture("test_2"));
    test2.setPosition(window.getSize().x-128.0f,window.getSize().y-64.0f);

    sf::Sprite test3(TextureManager::GetTexture("test_3"));
    test3.setPosition(window.getSize().x-64.0f,window.getSize().y-64.0f);

    sf::Sprite debug(TextureManager::GetTexture("debug"));
    debug.setPosition(window.getSize().x-256.0f,window.getSize().y-64.0f);

    sf::Sprite sunglasses(TextureManager::GetTexture("face_win"));
    sunglasses.setPosition((window.getSize().x/2.0f)-64.0f,window.getSize().y-64.0f);

    sf::Sprite deadFace(TextureManager::GetTexture("face_lose"));
    deadFace.setPosition((window.getSize().x/2.0f)-64.0f,window.getSize().y-64.0f);

    //SCORE
    sf::Sprite digits(TextureManager::GetTexture("digits"));
    sf::Sprite digits1(TextureManager::GetTexture("digits"));
    sf::Sprite digits2(TextureManager::GetTexture("digits"));
    sf::Sprite digits3(TextureManager::GetTexture("digits"));
    digits.setPosition((window.getSize().x/8.0f)-64.0f,window.getSize().y-64.0f);
    digits1.setPosition((window.getSize().x/8.0f)-85.0f,window.getSize().y-64.0f);
    digits2.setPosition((window.getSize().x/8.0f)-43.0f,window.getSize().y-64.0f);
    digits3.setPosition((window.getSize().x/8.0f)-22.0f,window.getSize().y-64.0f);
    digits1.setTextureRect(sf::IntRect(-21, 0, 21, 32));
    digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));
    digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
    digits3.setTextureRect(sf::IntRect(21 * onesDig, 1, 21, 32));

    for(int i  = 0; i < (b->GetTileHeight())*(b->GetTileWidth()); i++)
    {
        Tile* t = new Tile();
        t->MarkRevealed(false);
        t->MarkFlag(false);
        t->MarkMine(false);
        b->tiles.push_back(t);
    }

    //Random mines set up
    int* temp = new int[b->GetNumMines()];
    for(int z = 0; z < b->GetNumMines(); z++)
    {
        int randomNum = Random::Int(0, numTiles-1);
        bool check = false;
        for(int i = 0; i < sizeof(temp); i++)
        {
            if(temp[i] == randomNum) //check for duplicates
                check = true;
        }

        if(!check)
        {
            temp[z] = randomNum;
            b->tiles[randomNum]->SetXCoord(randomNum, numCol);
            b->tiles[randomNum]->SetYCoord(randomNum, numCol);
            b->tiles[randomNum]->MarkMine(true);

            mine.setPosition((float)b->tiles[randomNum]->GetXCoord()*32, (float)b->tiles[randomNum]->GetYCoord()*32);
            mines.push_back(mine);
            revealMines.push_back(mine);

            victoryFlag.setPosition((float)b->tiles[randomNum]->GetXCoord()*32, (float)b->tiles[randomNum]->GetYCoord()*32);
            tempFlags.push_back(victoryFlag);
        }
        else
            z--;
    }
    delete[] temp;

    for (int j = 0; j < b->GetTileHeight(); j++)
    {
        for (int i = 0; i < b->GetTileWidth(); i++)
        {
            hiddenTile.setPosition((float) i * 32, (float) j * 32);
            revealed.setPosition((float) i * 32, (float) j * 32);
            hiddenTiles.push_back(hiddenTile);
            revealedTiles.push_back(revealed);
            if(!(b->tiles[i+j*numCol]->hasMine))
            {
                b->tiles[i+j*numCol]->SetXCoord(i+j*numCol, numCol);
                b->tiles[i+j*numCol]->SetYCoord(i+j*numCol, numCol);
            }

            if ((i + 1) >= 0 && j >= 0 && (i + 1) < numCol && j < numRow)
                b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + (j * numCol)]); //right tile

            if ((i - 1) >= 0 && j >= 0 && (i - 1) < numCol && j < numRow)
                b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + (j * numCol)]); //left tile

            if (i >= 0 && (j + 1) >= 0 && i < numCol && (j + 1) < numRow)
                b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i) + ((j + 1) * numCol)]); //bottom tile

            if (i >= 0 && (j - 1) >= 0 && i < numCol && (j - 1) < numRow)
                b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i) + ((j - 1) * numCol)]); //top tile

            if ((i + 1) >= 0 && (j - 1) >= 0 && (i + 1) < numCol && (j - 1) < numRow)
                b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + ((j - 1) * numCol)]); //top right tile

            if ((i - 1) >= 0 && (j - 1) >= 0 && (i - 1) < numCol && (j - 1) < numRow)
                b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + ((j - 1) * numCol)]); //top left tile

            if ((i - 1) >= 0 && (j + 1) >= 0 && (i - 1) < numCol && (j + 1) < numRow)
                b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + ((j + 1) * numCol)]); //bottom left tile

            if ((i + 1) >= 0 && (j + 1) >= 0 && (i + 1) < numCol && (j + 1) < numRow)
                b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + ((j + 1) * numCol)]); //bottom right tile

            int numNeighbors = b->tiles[i + j*numCol]->GetNumNeighborMines();
            if (numNeighbors == 1) {
                num1.setPosition((float) i * 32, (float) j * 32);
                nums.push_back(num1);
            }
            if (numNeighbors == 2) {
                num2.setPosition((float) i * 32, (float) j * 32);
                nums.push_back(num2);
            }
            if (numNeighbors == 3) {
                num3.setPosition((float) i * 32, (float) j * 32);
                nums.push_back(num3);
            }
            if (numNeighbors == 4) {
                num4.setPosition((float) i * 32, (float) j * 32);
                nums.push_back(num4);
            }
            if (numNeighbors == 5) {
                num5.setPosition((float) i * 32, (float) j * 32);
                nums.push_back(num5);
            }
            if (numNeighbors == 6) {
                num6.setPosition((float) i * 32, (float) j * 32);
                nums.push_back(num6);
            }
            if (numNeighbors == 7) {
                num7.setPosition((float) i * 32, (float) j * 32);
                nums.push_back(num7);
            }
            if (numNeighbors == 8) {
                num8.setPosition((float) i * 32, (float) j * 32);
                nums.push_back(num8);
            }
        }
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed) //if a button was pressed
            {
                if(event.mouseButton.button == sf::Mouse::Right)
                {
                    auto mousePosition = sf::Mouse::getPosition();
                    mousePosition = sf::Mouse::getPosition(window);
                    //cout << "Mouse Position: " << event.mouseButton.x << ", " << event.mouseButton.y << endl;
                    sf::Vector2f mousePosFloat(mousePosition.x, mousePosition.y);

                    int xCoord = mousePosition.x / 32;
                    int yCoord = mousePosition.y / 32;
                    //cout << "coords: " << xCoord << ", " << yCoord << endl;

                    if(!b->tiles[xCoord + yCoord*numCol]->isFlagged && norm && !b->tiles[xCoord + yCoord*numCol]->isRevealed)
                    {
                        flag.setPosition((float) ((xCoord) * 32), (float) ((yCoord) * 32));
                        flags.push_back(flag);
                        b->tiles[xCoord + yCoord*numCol]->isFlagged = true;
                        //cout << "coords: " << xCoord << ", " << yCoord << endl;

                        if(!negDig) //if positive count while adding flags
                        {
                            if (onesDig == 0)
                            {
                                onesDig = 10;
                                if (tensDig == 0)
                                {
                                    tensDig = 10;
                                    hundredsDig--;
                                    digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));

                                    if (hundredsDig == 0)
                                    {
                                        tensDig = 10;
                                        digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));
                                    }
                                }
                                tensDig--;
                                digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                            }
                            onesDig--;
                            digits3.setTextureRect(sf::IntRect(21 * onesDig, 1, 21, 32));

                            if (hundredsDig == 0 && tensDig == 0 && onesDig == 0)
                            {
                                negDig = true;

                                //make negative sign appear
                                digits1.setTextureRect(sf::IntRect(21 * 10, 1, 21, 32));
                            }
                        }

                        else //if negative count while adding flags
                        {
                            if(onesDig == 9)
                            {
                                onesDig = -1;
                                tensDig++;
                                digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                            }
                            onesDig++;
                            digits3.setTextureRect(sf::IntRect(21 * onesDig, 1, 21, 32));

                            if(tensDig == 9)
                            {
                                tensDig = -1;
                                hundredsDig--;
                                digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));
                                tensDig++;
                                digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                            }
                        }
                    }

                    else if(norm)
                    {
                        b->tiles[xCoord + yCoord*numCol]->isFlagged = false;
                        for (int j = 0; j < flags.size(); j++)
                        {
                            sf::FloatRect flag = flags[j].getGlobalBounds();
                            if (flag.contains(mousePosFloat))
                            {
                                flags.erase(flags.begin()+j);

                                if(!negDig) //if positive count while removing flags
                                {
                                    if (onesDig == 9)
                                    {
                                        onesDig = -1;
                                        digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                                        if (tensDig == 9)
                                        {
                                            tensDig = -1;
                                            hundredsDig++;
                                            digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));

                                            if (hundredsDig == 9)
                                            {
                                                tensDig = -1;
                                                digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));
                                            }
                                        }
                                        tensDig++;
                                        digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                                    }
                                    onesDig++;
                                    digits3.setTextureRect(sf::IntRect(21 * onesDig, 1, 21, 32));
                                }

                                else //if negative count while removing flags
                                {
                                    if (onesDig == 0)
                                    {
                                        onesDig = 10;
                                        tensDig--;
                                        digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                                    }
                                    onesDig--;
                                    digits3.setTextureRect(sf::IntRect(21 * onesDig, 1, 21, 32));

                                    if (tensDig == -1)
                                    {
                                        tensDig = 9;
                                        hundredsDig--;
                                        digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));
                                        digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                                    }

                                    if (tensDig == 0 && onesDig == 0)
                                    {
                                        negDig = false;

                                        //make negative sign appear
                                        digits1.setTextureRect(sf::IntRect(-21, 1, 21, 32));
                                    }
                                }
                            }
                        }
                    }
                }

                //check WHICH button was pressed
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    //print location of cursor
                    auto mousePosition = sf::Mouse::getPosition();
                    mousePosition = sf::Mouse::getPosition(window);
                    //cout << "Mouse Position: " << event.mouseButton.x << ", " << event.mouseButton.y << endl;

                    //check to see if you clicked on the button
                    sf::FloatRect smile = smiley.getGlobalBounds();
                    sf::FloatRect test1Square = test1.getGlobalBounds();
                    sf::FloatRect test2Square = test2.getGlobalBounds();
                    sf::FloatRect test3Square = test3.getGlobalBounds();
                    sf::FloatRect debugger = debug.getGlobalBounds();

                    sf::Vector2f mousePosFloat(mousePosition.x, mousePosition.y);

                    int xCoord = mousePosition.x / 32;
                    int yCoord = mousePosition.y / 32;
                    //cout << "coords: " << xCoord << ", " << yCoord << endl;

                    if(debugger.contains(mousePosFloat)) //if you clicked debug btn
                    {
                        if(!debugModeOn && norm) //can't go into debug mode after winning
                        {
                            //copy over all mines
                            for(int i = 0; i < mines.size(); i++)
                                minesDebugMode.push_back(mines[i]);
                            debugModeOn = true;
                        }
                        else if(debugModeOn && norm)
                        {
                            //erase all revealed mines
                            minesDebugMode.clear();
                            debugModeOn = false;
                        }

                    }

                    else if (smile.contains(mousePosFloat))//if you clicked n smiley btn
                    {
                        norm = true;
                        defeat = false;
                        victory = false;

                        victoryFlags.clear();
                        revealMines.clear();
                        tempFlags.clear();
                        nums.clear();
                        flags.clear();
                        mines.clear();
                        b->tiles.clear();
                        minesDebugMode.clear();
                        hiddenTiles.clear();
                        victoryHiddenTiles.clear();

                        negDig = false;
                        onesDig = b->GetNumMines() % 10; //0
                        tensDig = (b->GetNumMines() / 10) % 10; //5
                        hundredsDig = (b->GetNumMines() / 100) % 10;

                        digits1.setTextureRect(sf::IntRect(-21, 0, 21, 32));
                        digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));
                        digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                        digits3.setTextureRect(sf::IntRect(21 * onesDig, 1, 21, 32));

                        for(int i  = 0; i < (b->GetTileHeight())*(b->GetTileWidth()); i++)
                        {
                            Tile* t = new Tile();
                            t->MarkRevealed(false);
                            t->MarkFlag(false);
                            t->MarkMine(false);
                            b->tiles.push_back(t);
                        }

                        //Random mines set up
                        int* temp = new int[b->GetNumMines()];
                        for(int z = 0; z < b->GetNumMines(); z++)
                        {
                            int randomNum = Random::Int(0, numTiles-1);
                            bool check = false;
                            for(int i = 0; i < sizeof(temp); i++)
                            {
                                if(temp[i] == randomNum) //check for duplicates
                                    check = true;
                            }

                            if(!check)
                            {
                                temp[z] = randomNum;
                                b->tiles[randomNum]->SetXCoord(randomNum, numCol);
                                b->tiles[randomNum]->SetYCoord(randomNum, numCol);
                                b->tiles[randomNum]->MarkMine(true);


                                mine.setPosition((float)b->tiles[randomNum]->GetXCoord()*32, (float)b->tiles[randomNum]->GetYCoord()*32);
                                mines.push_back(mine);
                                revealMines.push_back(mine);

                                victoryFlag.setPosition((float)b->tiles[randomNum]->GetXCoord()*32, (float)b->tiles[randomNum]->GetYCoord()*32);
                                tempFlags.push_back(victoryFlag);

                                victoryTile.setPosition((float)b->tiles[randomNum]->GetXCoord()*32, (float)b->tiles[randomNum]->GetYCoord()*32);
                                victoryHiddenTiles.push_back(victoryTile);
                            }
                            else
                                z--;
                        }
                        delete[] temp;

                        for (int j = 0; j < b->GetTileHeight(); j++)
                        {
                            for (int i = 0; i < b->GetTileWidth(); i++)
                            {
                                hiddenTile.setPosition((float) i * 32, (float) j * 32);
                                revealed.setPosition((float) i * 32, (float) j * 32);
                                hiddenTiles.push_back(hiddenTile);
                                revealedTiles.push_back(revealed);
                                if(!(b->tiles[i+j*numCol]->hasMine))
                                {
                                    b->tiles[i+j*numCol]->SetXCoord(i+j*numCol, numCol);
                                    b->tiles[i+j*numCol]->SetYCoord(i+j*numCol, numCol);
                                }

                                if ((i + 1) >= 0 && j >= 0 && (i + 1) < numCol && j < numRow)
                                    b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + (j * numCol)]); //right tile

                                if ((i - 1) >= 0 && j >= 0 && (i - 1) < numCol && j < numRow)
                                    b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + (j * numCol)]); //left tile

                                if (i >= 0 && (j + 1) >= 0 && i < numCol && (j + 1) < numRow)
                                    b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i) + ((j + 1) * numCol)]); //bottom tile

                                if (i >= 0 && (j - 1) >= 0 && i < numCol && (j - 1) < numRow)
                                    b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i) + ((j - 1) * numCol)]); //top tile

                                if ((i + 1) >= 0 && (j - 1) >= 0 && (i + 1) < numCol && (j - 1) < numRow)
                                    b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + ((j - 1) * numCol)]); //top right tile

                                if ((i - 1) >= 0 && (j - 1) >= 0 && (i - 1) < numCol && (j - 1) < numRow)
                                    b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + ((j - 1) * numCol)]); //top left tile

                                if ((i - 1) >= 0 && (j + 1) >= 0 && (i - 1) < numCol && (j + 1) < numRow)
                                    b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + ((j + 1) * numCol)]); //bottom left tile

                                if ((i + 1) >= 0 && (j + 1) >= 0 && (i + 1) < numCol && (j + 1) < numRow)
                                    b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + ((j + 1) * numCol)]); //bottom right tile

                                int numNeighbors = b->tiles[i + j*numCol]->GetNumNeighborMines();
                                if (numNeighbors == 1) {
                                    num1.setPosition((float) i * 32, (float) j * 32);
                                    nums.push_back(num1);
                                }
                                if (numNeighbors == 2) {
                                    num2.setPosition((float) i * 32, (float) j * 32);
                                    nums.push_back(num2);
                                }
                                if (numNeighbors == 3) {
                                    num3.setPosition((float) i * 32, (float) j * 32);
                                    nums.push_back(num3);
                                }
                                if (numNeighbors == 4) {
                                    num4.setPosition((float) i * 32, (float) j * 32);
                                    nums.push_back(num4);
                                }
                                if (numNeighbors == 5) {
                                    num5.setPosition((float) i * 32, (float) j * 32);
                                    nums.push_back(num5);
                                }
                                if (numNeighbors == 6) {
                                    num6.setPosition((float) i * 32, (float) j * 32);
                                    nums.push_back(num6);
                                }
                                if (numNeighbors == 7) {
                                    num7.setPosition((float) i * 32, (float) j * 32);
                                    nums.push_back(num7);
                                }
                                if (numNeighbors == 8) {
                                    num8.setPosition((float) i * 32, (float) j * 32);
                                    nums.push_back(num8);
                                }
                            }
                        }

                    }

                    else if(test1Square.contains(mousePosFloat)) //if you clicked test1
                    {
                        norm = true;
                        defeat = false;
                        victory = false;

                        mines.clear();
                        nums.clear();
                        victoryFlags.clear();
                        revealMines.clear();
                        flags.clear();
                        hiddenTiles.clear();
                        victoryHiddenTiles.clear();
                        minesDebugMode.clear();
                        tempFlags.clear();
                        b->tiles.clear();

                        for(int i  = 0; i < (b->GetTileHeight())*(b->GetTileWidth()); i++)
                        {
                            Tile* t = new Tile();
                            t->MarkRevealed(false);
                            t->MarkFlag(false);
                            t->MarkMine(false);
                            b->tiles.push_back(t);
                        }

                        vector<string> temp;
                        ifstream file("boards/testboard1.brd");
                        if(file.is_open())
                        {
                            string line;
                            while(getline(file, line))
                                temp.push_back(line);

                            for(int j = 0; j < temp.size(); j++) //go through each column of testboard file
                            {
                                for (int i = 0; i < temp[j].size(); i++) //go through each row of 0s and 1s
                                {
                                    if (temp[j].at(i) == '1') {
                                        mine.setPosition((float) i * 32, (float) j * 32);
                                        mines.push_back(mine);
                                        revealMines.push_back(mine);
                                        victoryFlag.setPosition((float) i * 32, (float) j * 32);
                                        tempFlags.push_back(victoryFlag);
                                        hiddenTile.setPosition((float) i * 32, (float) j * 32);
                                        hiddenTiles.push_back(hiddenTile);
                                        victoryTile.setPosition((float) i * 32, (float) j * 32);
                                        victoryHiddenTiles.push_back(victoryTile);
                                        b->tiles[i + j * numCol]->MarkMine(true);
                                        b->tiles[i+j*numCol]->SetXCoord(i+j*numCol, numCol);
                                        b->tiles[i+j*numCol]->SetYCoord(i+j*numCol, numCol);
                                    } else if (temp[j].at(i) == '0') {
                                        hiddenTile.setPosition((float) i * 32, (float) j * 32);
                                        hiddenTiles.push_back(hiddenTile);
                                        b->tiles[i+j*numCol]->SetXCoord(i+j*numCol, numCol);
                                        b->tiles[i+j*numCol]->SetYCoord(i+j*numCol, numCol);
                                    }
                                }
                            }

                            for (int j = 0; j < b->GetTileHeight(); j++)
                            {
                                for (int i = 0; i < b->GetTileWidth(); i++)
                                {
                                    if ((i + 1) >= 0 && j >= 0 && (i + 1) < numCol && j < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + (j * numCol)]); //right tile

                                    if ((i - 1) >= 0 && j >= 0 && (i - 1) < numCol && j < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + (j * numCol)]); //left tile

                                    if (i >= 0 && (j + 1) >= 0 && i < numCol && (j + 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i) + ((j + 1) * numCol)]); //bottom tile

                                    if (i >= 0 && (j - 1) >= 0 && i < numCol && (j - 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i) + ((j - 1) * numCol)]); //top tile

                                    if ((i + 1) >= 0 && (j - 1) >= 0 && (i + 1) < numCol && (j - 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + ((j - 1) * numCol)]); //top right tile

                                    if ((i - 1) >= 0 && (j - 1) >= 0 && (i - 1) < numCol && (j - 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + ((j - 1) * numCol)]); //top left tile

                                    if ((i - 1) >= 0 && (j + 1) >= 0 && (i - 1) < numCol && (j + 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + ((j + 1) * numCol)]); //bottom left tile

                                    if ((i + 1) >= 0 && (j + 1) >= 0 && (i + 1) < numCol && (j + 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + ((j + 1) * numCol)]); //bottom right tile

                                    int numNeighbors = b->tiles[i + j*numCol]->GetNumNeighborMines();
                                    if(numNeighbors == 1)
                                    {
                                        num1.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num1);
                                    }
                                    if(numNeighbors == 2)
                                    {
                                        num2.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num2);
                                    }
                                    if(numNeighbors == 3)
                                    {
                                        num3.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num3);
                                    }
                                    if(numNeighbors == 4)
                                    {
                                        num4.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num4);
                                    }
                                    if(numNeighbors == 5)
                                    {
                                        num5.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num5);
                                    }
                                    if(numNeighbors == 6)
                                    {
                                        num6.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num6);
                                    }
                                    if(numNeighbors == 7)
                                    {
                                        num7.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num7);
                                    }
                                    if(numNeighbors == 8)
                                    {
                                        num8.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num8);
                                    }
                                }
                            }
                        }
                        else
                            cout << "File could not be opened." << endl;


                        negDig = false;
                        onesDig = mines.size() % 10; //1
                        tensDig = (mines.size() / 10) % 10; //0
                        hundredsDig = (mines.size() / 100) % 10; //0

                        digits1.setTextureRect(sf::IntRect(-21, 0, 21, 32));
                        digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));
                        digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                        digits3.setTextureRect(sf::IntRect(21 * onesDig, 1, 21, 32));
                    }

                    else if(test2Square.contains(mousePosFloat)) //if you clicked test2
                    {
                        norm = true;
                        defeat = false;
                        victory = false;

                        mines.clear();
                        nums.clear();
                        victoryFlags.clear();
                        revealMines.clear();
                        flags.clear();
                        hiddenTiles.clear();
                        victoryHiddenTiles.clear();
                        minesDebugMode.clear();
                        tempFlags.clear();
                        b->tiles.clear();

                        for(int i  = 0; i < (b->GetTileHeight())*(b->GetTileWidth()); i++)
                        {
                            Tile* t = new Tile();
                            t->MarkRevealed(false);
                            t->MarkFlag(false);
                            t->MarkMine(false);
                            b->tiles.push_back(t);
                        }

                        vector<string> temp;
                        ifstream file("boards/testboard2.brd");
                        if(file.is_open())
                        {
                            string line;
                            while(getline(file, line))
                                temp.push_back(line);

                            for(int j = 0; j < temp.size(); j++) //go through each column of testboard file
                            {
                                for (int i = 0; i < temp[j].size(); i++) //go through each row of 0s and 1s
                                {
                                    if (temp[j].at(i) == '1') {
                                        mine.setPosition((float) i * 32, (float) j * 32);
                                        mines.push_back(mine);
                                        revealMines.push_back(mine);
                                        victoryFlag.setPosition((float) i * 32, (float) j * 32);
                                        tempFlags.push_back(victoryFlag);
                                        hiddenTile.setPosition((float) i * 32, (float) j * 32);
                                        hiddenTiles.push_back(hiddenTile);
                                        victoryTile.setPosition((float) i * 32, (float) j * 32);
                                        victoryHiddenTiles.push_back(victoryTile);
                                        b->tiles[i + j * numCol]->MarkMine(true);
                                        b->tiles[i+j*numCol]->SetXCoord(i+j*numCol, numCol);
                                        b->tiles[i+j*numCol]->SetYCoord(i+j*numCol, numCol);
                                    } else if (temp[j].at(i) == '0') {
                                        hiddenTile.setPosition((float) i * 32, (float) j * 32);
                                        hiddenTiles.push_back(hiddenTile);
                                        b->tiles[i+j*numCol]->SetXCoord(i+j*numCol, numCol);
                                        b->tiles[i+j*numCol]->SetYCoord(i+j*numCol, numCol);
                                    }
                                }
                            }

                            for (int j = 0; j < b->GetTileHeight(); j++)
                            {
                                for (int i = 0; i < b->GetTileWidth(); i++)
                                {
                                    if ((i + 1) >= 0 && j >= 0 && (i + 1) < numCol && j < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + (j * numCol)]); //right tile

                                    if ((i - 1) >= 0 && j >= 0 && (i - 1) < numCol && j < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + (j * numCol)]); //left tile

                                    if (i >= 0 && (j + 1) >= 0 && i < numCol && (j + 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i) + ((j + 1) * numCol)]); //bottom tile

                                    if (i >= 0 && (j - 1) >= 0 && i < numCol && (j - 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i) + ((j - 1) * numCol)]); //top tile

                                    if ((i + 1) >= 0 && (j - 1) >= 0 && (i + 1) < numCol && (j - 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + ((j - 1) * numCol)]); //top right tile

                                    if ((i - 1) >= 0 && (j - 1) >= 0 && (i - 1) < numCol && (j - 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + ((j - 1) * numCol)]); //top left tile

                                    if ((i - 1) >= 0 && (j + 1) >= 0 && (i - 1) < numCol && (j + 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + ((j + 1) * numCol)]); //bottom left tile

                                    if ((i + 1) >= 0 && (j + 1) >= 0 && (i + 1) < numCol && (j + 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + ((j + 1) * numCol)]); //bottom right tile

                                    int numNeighbors = b->tiles[i + j*numCol]->GetNumNeighborMines();
                                    if(numNeighbors == 1)
                                    {
                                        num1.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num1);
                                    }
                                    if(numNeighbors == 2)
                                    {
                                        num2.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num2);
                                    }
                                    if(numNeighbors == 3)
                                    {
                                        num3.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num3);
                                    }
                                    if(numNeighbors == 4)
                                    {
                                        num4.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num4);
                                    }
                                    if(numNeighbors == 5)
                                    {
                                        num5.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num5);
                                    }
                                    if(numNeighbors == 6)
                                    {
                                        num6.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num6);
                                    }
                                    if(numNeighbors == 7)
                                    {
                                        num7.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num7);
                                    }
                                    if(numNeighbors == 8)
                                    {
                                        num8.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num8);
                                    }
                                }
                            }
                        }
                        else
                            cout << "File could not be opened." << endl;


                        negDig = false;
                        onesDig = mines.size() % 10; //1
                        tensDig = (mines.size() / 10) % 10; //0
                        hundredsDig = (mines.size() / 100) % 10; //0

                        digits1.setTextureRect(sf::IntRect(-21, 0, 21, 32));
                        digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));
                        digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                        digits3.setTextureRect(sf::IntRect(21 * onesDig, 1, 21, 32));
                    }

                    else if(test3Square.contains(mousePosFloat)) //if you clicked test3
                    {
                        norm = true;
                        defeat = false;
                        victory = false;

                        mines.clear();
                        nums.clear();
                        victoryFlags.clear();
                        revealMines.clear();
                        flags.clear();
                        hiddenTiles.clear();
                        victoryHiddenTiles.clear();
                        minesDebugMode.clear();
                        tempFlags.clear();
                        b->tiles.clear();

                        for(int i  = 0; i < (b->GetTileHeight())*(b->GetTileWidth()); i++)
                        {
                            Tile* t = new Tile();
                            t->MarkRevealed(false);
                            t->MarkFlag(false);
                            t->MarkMine(false);
                            b->tiles.push_back(t);
                        }

                        vector<string> temp;
                        ifstream file("boards/testboard3.brd");
                        if(file.is_open())
                        {
                            string line;
                            while(getline(file, line))
                                temp.push_back(line);

                            for(int j = 0; j < temp.size(); j++) //go through each column of testboard file
                            {
                                for (int i = 0; i < temp[j].size(); i++) //go through each row of 0s and 1s
                                {
                                    if (temp[j].at(i) == '1') {
                                        mine.setPosition((float) i * 32, (float) j * 32);
                                        mines.push_back(mine);
                                        revealMines.push_back(mine);
                                        victoryFlag.setPosition((float) i * 32, (float) j * 32);
                                        tempFlags.push_back(victoryFlag);
                                        hiddenTile.setPosition((float) i * 32, (float) j * 32);
                                        hiddenTiles.push_back(hiddenTile);
                                        victoryTile.setPosition((float) i * 32, (float) j * 32);
                                        victoryHiddenTiles.push_back(victoryTile);
                                        b->tiles[i + j * numCol]->MarkMine(true);
                                        b->tiles[i+j*numCol]->SetXCoord(i+j*numCol, numCol);
                                        b->tiles[i+j*numCol]->SetYCoord(i+j*numCol, numCol);
                                    } else if (temp[j].at(i) == '0') {
                                        hiddenTile.setPosition((float) i * 32, (float) j * 32);
                                        hiddenTiles.push_back(hiddenTile);
                                        b->tiles[i+j*numCol]->SetXCoord(i+j*numCol, numCol);
                                        b->tiles[i+j*numCol]->SetYCoord(i+j*numCol, numCol);
                                    }
                                }
                            }

                            for (int j = 0; j < b->GetTileHeight(); j++)
                            {
                                for (int i = 0; i < b->GetTileWidth(); i++)
                                {
                                    if ((i + 1) >= 0 && j >= 0 && (i + 1) < numCol && j < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + (j * numCol)]); //right tile

                                    if ((i - 1) >= 0 && j >= 0 && (i - 1) < numCol && j < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + (j * numCol)]); //left tile

                                    if (i >= 0 && (j + 1) >= 0 && i < numCol && (j + 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i) + ((j + 1) * numCol)]); //bottom tile

                                    if (i >= 0 && (j - 1) >= 0 && i < numCol && (j - 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i) + ((j - 1) * numCol)]); //top tile

                                    if ((i + 1) >= 0 && (j - 1) >= 0 && (i + 1) < numCol && (j - 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + ((j - 1) * numCol)]); //top right tile

                                    if ((i - 1) >= 0 && (j - 1) >= 0 && (i - 1) < numCol && (j - 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + ((j - 1) * numCol)]); //top left tile

                                    if ((i - 1) >= 0 && (j + 1) >= 0 && (i - 1) < numCol && (j + 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i - 1) + ((j + 1) * numCol)]); //bottom left tile

                                    if ((i + 1) >= 0 && (j + 1) >= 0 && (i + 1) < numCol && (j + 1) < numRow)
                                        b->tiles[i + j*numCol]->neighbors.push_back(b->tiles[(i + 1) + ((j + 1) * numCol)]); //bottom right tile

                                    int numNeighbors = b->tiles[i + j*numCol]->GetNumNeighborMines();
                                    if(numNeighbors == 1)
                                    {
                                        num1.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num1);
                                    }
                                    if(numNeighbors == 2)
                                    {
                                        num2.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num2);
                                    }
                                    if(numNeighbors == 3)
                                    {
                                        num3.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num3);
                                    }
                                    if(numNeighbors == 4)
                                    {
                                        num4.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num4);
                                    }
                                    if(numNeighbors == 5)
                                    {
                                        num5.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num5);
                                    }
                                    if(numNeighbors == 6)
                                    {
                                        num6.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num6);
                                    }
                                    if(numNeighbors == 7)
                                    {
                                        num7.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num7);
                                    }
                                    if(numNeighbors == 8)
                                    {
                                        num8.setPosition((float) i * 32, (float) j * 32);
                                        nums.push_back(num8);
                                    }
                                }
                            }
                        }
                        else
                            cout << "File could not be opened." << endl;


                        negDig = false;
                        onesDig = mines.size() % 10; //1
                        tensDig = (mines.size() / 10) % 10; //0
                        hundredsDig = (mines.size() / 100) % 10; //0

                        digits1.setTextureRect(sf::IntRect(-21, 0, 21, 32));
                        digits.setTextureRect(sf::IntRect(21 * hundredsDig, 1, 21, 32));
                        digits2.setTextureRect(sf::IntRect(21 * tensDig, 1, 21, 32));
                        digits3.setTextureRect(sf::IntRect(21 * onesDig, 1, 21, 32));
                    }

                    else
                    {
                        vector<int> tileInts;
                        for (int i = 0; i < hiddenTiles.size(); i++) //loop through all tiles
                        {
                            sf::FloatRect tile = hiddenTiles[i].getGlobalBounds();

                            //if you click on a tile w/ no flag and no mine
                            if (tile.contains(mousePosFloat) && !b->tiles[xCoord + yCoord * numCol]->isFlagged &&
                                !b->tiles[xCoord + yCoord * numCol]->hasMine && norm)
                            {
                                //if tile has no neighboring mines
                                if (b->tiles[xCoord + yCoord * numCol]->GetNumNeighborMines() == 0)
                                {
                                    tileInts = b->RevealNeighbors((xCoord + yCoord * numCol),tileInts); //reveal tile's neighbors

                                    for (int i = 0; i < tileInts.size(); i++)
                                    {
                                        hiddenTiles[tileInts[i]].setColor(sf::Color::Transparent);
                                        b->tiles[tileInts[i]]->MarkRevealed(true);
                                    }
                                }
                                b->tiles[xCoord + yCoord * numCol]->MarkRevealed(true);


                                hiddenTiles[i].setColor(sf::Color::Transparent);
                                //cout << "tile num: " << xCoord + yCoord * numCol << endl;
                                //cout << "num mines: " << b->tiles[xCoord + yCoord * numCol]->GetNumNeighborMines() << endl;
                            }

                            //if you click on a tile w/ a mine and no flag (DEFEAT)
                            if (tile.contains(mousePosFloat) && b->tiles[xCoord + yCoord * numCol]->hasMine &&
                                !b->tiles[xCoord + yCoord * numCol]->isFlagged) {
                                //reveal all mines
                                for (int j = 0; j < b->tiles.size(); j++) {
                                    if (b->tiles[j]->hasMine)
                                        hiddenTiles[j].setColor(sf::Color::Transparent);

                                    for (int i = 0; i < mines.size(); i++)
                                        minesDebugMode.push_back(mines[i]);
                                }
                                norm = false;
                                victory = false;
                                defeat = true; //reveal dead face

                                mines.clear();

                                break; //end loop?
                            }

                            if (tile.contains(mousePosFloat) && b->tiles[xCoord + yCoord * numCol]->victoryFlag) {
                                flags.erase(flags.begin() + (xCoord + yCoord * numCol));
                            }
                        }

                        if(b->CheckBoard()) //check for victory
                        {
                            //cout << "victory" << endl;

                            //set smiley to sunglasses
                            victory = true;
                            norm = false;
                            defeat = false;

                            //flag remaining hidden mines (all tiles left)
                            for(int i = 0; i < tempFlags.size(); i++)
                                victoryFlags.push_back(tempFlags[i]);

                            //counter should be 0
                            digits1.setTextureRect(sf::IntRect(-21, 0, 21, 32));
                            digits.setTextureRect(sf::IntRect(21 * 0, 1, 21, 32));
                            digits2.setTextureRect(sf::IntRect(21 * 0, 1, 21, 32));
                            digits3.setTextureRect(sf::IntRect(21 * 0, 1, 21, 32));

                            mines.clear();
                            minesDebugMode.clear();
                        }
                    }
                }
            }
        }

        window.clear(); //erase the screen

        for(unsigned int i = 0; i < revealedTiles.size(); i++)
            window.draw(revealedTiles[i]);

        for(unsigned int i = 0; i < mines.size(); i++)
            window.draw(mines[i]);

        for(unsigned int i = 0; i < nums.size(); i++)
            window.draw(nums[i]);

        for(unsigned int i = 0; i < hiddenTiles.size(); i++)
            window.draw(hiddenTiles[i]);

        for(unsigned int i = 0; i < flags.size(); i++)
            window.draw(flags[i]);

        for(unsigned int i = 0; i < minesDebugMode.size(); i++)
            window.draw(minesDebugMode[i]);

        if(victory)
        {
            for(unsigned int i = 0; i < victoryHiddenTiles.size(); i++)
                window.draw(victoryHiddenTiles[i]);

            for(unsigned int i = 0; i < victoryFlags.size(); i++)
                window.draw(victoryFlags[i]);

            window.draw(sunglasses);
        }

        window.draw(test1);
        window.draw(test2);
        window.draw(test3);
        window.draw(debug);
        window.draw(digits);
        window.draw(digits1);
        window.draw(digits2);
        window.draw(digits3);

        if(norm)
            window.draw(smiley);
        if(defeat)
            window.draw(deadFace);

        window.display();
    }

    TextureManager::Clear();

    return 0;
}
