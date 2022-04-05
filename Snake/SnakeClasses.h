#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;

class Block
{
protected:
    Vector2f pos;
    Vector2f end;
    Vector2f sectionSize;
public:
    Block(Vector2f _pos = { 40, 40 }, Vector2f _sectionSize = { 40, 40 });

    Vector2f getPos();

    Vector2f getEnd();

    Vector2f getSize();

    virtual void setPosition(Vector2f _pos);

    virtual void setSize(Vector2f _sectionSize);
};

class SnakeBlock : public Block
{
protected:
    int direction;
public:
    SnakeBlock(Vector2f _pos = { 40, 40 }, int _direction = 3);

    void move();

    int getDirection();

    void setDirection(int _direction);

    friend class Snake;
};

class HedgehogBlock : public Block
{
protected:
    RectangleShape hedgehogRect;
    Texture hedgehogImg;

    bool intersectWithSnakes(vector<SnakeBlock>& snakeBody1, vector<SnakeBlock>& snakeBody2);
public:
    HedgehogBlock();

    void resetPos(RenderWindow& window, vector<SnakeBlock>& snakeBody1, vector<SnakeBlock>& snakeBody2);

    RectangleShape getRect();

    void draw(RenderWindow& window);

    virtual void setSize(Vector2f _sectionSize);

    virtual void setPosition(Vector2f _pos);
};

class Snake
{
protected:
    vector<SnakeBlock> snakeBody;
    RectangleShape snakeRect;
    Texture snakeImg;
    int stepCounter = 0;

    void rotateImg(const SnakeBlock& snakeBlock);
    
    bool eating(HedgehogBlock& hedgehog);

    bool selfEating();
    
    int snakesCrash(Snake& enemySnake);

    bool abroad(RenderWindow& window);

public:
    Snake(Color blockColor = Color::Green, Vector2f spawnPoint = Vector2f(40, 40));

    int move(HedgehogBlock& hedgehog, Snake& enemySnake, RenderWindow& window);

    void draw(RenderWindow& window);

    void setHeadDir(int _direction);

    SnakeBlock getHead();

    vector<SnakeBlock> getBody();

    RectangleShape getRect();
};

class Game
{
private:
    ContextSettings settings;
    vector<VideoMode> screenSizes;
    RenderWindow window;
    RectangleShape background;
    Texture backgroundTexture;
    HedgehogBlock hedgehog;
    Font font;
    Text pointCounter1;
    Text pointCounter2;
    wstring winner;
    int refreshTime;
    bool gameOver;

    void refreshCounter(Snake& snake1, Snake& snake2);

    void aboutPage(RenderWindow& window);

    bool preStart(RenderWindow& window);

    void endGame();
   
public:
    Game();

    Game(int _refreshTime);

    void play();
};
