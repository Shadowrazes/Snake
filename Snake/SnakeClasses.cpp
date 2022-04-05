#include "SnakeClasses.h"

using namespace sf;
using namespace std;

Block::Block(Vector2f _pos, Vector2f _sectionSize)
    : pos(_pos), sectionSize(_sectionSize)
{
    end = Vector2f(pos.x + sectionSize.x, pos.y + sectionSize.y);
}

Vector2f Block::getPos() { return pos; }

Vector2f Block::getEnd() { return end; }

Vector2f Block::getSize() { return sectionSize; }

void Block::setPosition(Vector2f _pos)
{
    pos = _pos;
    end = Vector2f(pos.x + sectionSize.x, pos.y + sectionSize.y);
}

void Block::setSize(Vector2f _sectionSize) { sectionSize = _sectionSize; }

SnakeBlock::SnakeBlock(Vector2f _pos, int _direction)
    : Block(_pos), direction(_direction)
{
}

void SnakeBlock::move()
{
    switch (direction)
    {
    case 0:
    {
        pos.y -= sectionSize.y / 20;
        end.y -= sectionSize.y / 20;
        break;
    }
    case 1:
    {
        pos.x += sectionSize.x / 20;
        end.x += sectionSize.x / 20;
        break;
    }
    case 2:
    {
        pos.y += sectionSize.y / 20;
        end.y += sectionSize.y / 20;
        break;
    }
    case 3:
    {
        pos.x -= sectionSize.x / 20;
        end.x -= sectionSize.x / 20;
        break;
    }
    }
}

int SnakeBlock::getDirection() { return direction; }

void SnakeBlock::setDirection(int _direction) { direction = _direction; }

bool HedgehogBlock::intersectWithSnakes(vector<SnakeBlock>& snakeBody1, vector<SnakeBlock>& snakeBody2)
{
    for (auto& it : snakeBody1)
    {
        if (it.getPos() == pos)
            return true;
    }
    for (auto& it : snakeBody2)
    {
        if (it.getPos() == pos)
            return true;
    }
    return false;
}

HedgehogBlock::HedgehogBlock() : Block()
{
    hedgehogImg.loadFromFile("hedgehog.png");
    hedgehogRect.setSize(sectionSize);
    hedgehogRect.setPosition(pos);
    hedgehogRect.setTexture(&hedgehogImg);
}

void HedgehogBlock::resetPos(RenderWindow& window, vector<SnakeBlock>& snakeBody1, vector<SnakeBlock>& snakeBody2)
{
    do
    {
        pos.x = sectionSize.x +
            rand() % static_cast<int>(window.getSize().x - 2 * sectionSize.x);
        pos.x -= static_cast<int>(pos.x) % static_cast<int>(sectionSize.x);

        pos.y = sectionSize.y +
            rand() % static_cast<int>(window.getSize().y - 2 * sectionSize.y);
        pos.y -= static_cast<int>(pos.y) % static_cast<int>(sectionSize.y);
    } while (intersectWithSnakes(snakeBody1, snakeBody2));

    end = Vector2f(pos.x + sectionSize.x, pos.y + sectionSize.y);
    hedgehogRect.setPosition(pos);
}

RectangleShape HedgehogBlock::getRect() { return hedgehogRect; }

void HedgehogBlock::draw(RenderWindow& window)
{
    window.draw(hedgehogRect);
    window.display();
}

void HedgehogBlock::setSize(Vector2f _sectionSize)
{
    sectionSize = _sectionSize;
    hedgehogRect.setSize(sectionSize);
}

void HedgehogBlock::setPosition(Vector2f _pos)
{
    pos = _pos;
    end = Vector2f(pos.x + sectionSize.x, pos.y + sectionSize.y);
    hedgehogRect.setPosition(pos);
}

void Snake::rotateImg(const SnakeBlock& snakeBlock)
{
    switch (snakeBlock.direction)
    {
    case 0:
    {
        snakeRect.setRotation(90);
        break;
    }
    case 1:
    {
        snakeRect.setRotation(180);
        break;
    }
    case 2:
    {
        snakeRect.setRotation(270);
        break;
    }
    case 3:
    {
        snakeRect.setRotation(0);
        break;
    }
    }
}

bool Snake::eating(HedgehogBlock& hedgehog)
{
    Vector2f snakeHeadCenter(
        snakeBody[0].pos.x + snakeBody[0].sectionSize.x / 2,
        snakeBody[0].pos.y + snakeBody[0].sectionSize.y / 2);
    if (hedgehog.getRect().getGlobalBounds().contains(snakeHeadCenter))
    {
        SnakeBlock forPush;
        switch (snakeBody[snakeBody.size() - 1].direction)
        {
        case 0:
        {
            forPush.pos = Vector2f(snakeBody[snakeBody.size() - 1].end.x -
                hedgehog.getSize().x,
                snakeBody[snakeBody.size() - 1].end.y);
            break;
        }
        case 1:
        {
            forPush.pos = Vector2f(snakeBody[snakeBody.size() - 1].pos.x -
                hedgehog.getSize().x,
                snakeBody[snakeBody.size() - 1].pos.y);
            break;
        }
        case 2:
        {
            forPush.pos = Vector2f(snakeBody[snakeBody.size() - 1].pos.x,
                snakeBody[snakeBody.size() - 1].pos.y -
                hedgehog.getSize().y);
            break;
        }
        case 3:
        {
            forPush.pos = Vector2f(snakeBody[snakeBody.size() - 1].end.x,
                snakeBody[snakeBody.size() - 1].end.y -
                hedgehog.getSize().y);
            break;
        }
        }
        forPush.end = Vector2f(forPush.pos.x + forPush.sectionSize.x,
            forPush.pos.y + forPush.sectionSize.y);
        forPush.direction = snakeBody[snakeBody.size() - 1].direction;
        snakeBody.push_back(forPush);
        return true;
    }
    return false;
}

bool Snake::selfEating()
{
    Vector2f headPos(snakeBody[0].pos);
    for (int i = 4; i < snakeBody.size(); i++)
    {
        if (headPos.x == snakeBody[i].pos.x && headPos.y == snakeBody[i].pos.y)
            return true;
    }
    return false;
}

int Snake::snakesCrash(Snake& enemySnake)
{
    Vector2f snakeHeadCenter(snakeBody[0].pos.x + snakeBody[0].sectionSize.x / 2, snakeBody[0].pos.y + snakeBody[0].sectionSize.y / 2);
    for (int i = 0; i < enemySnake.snakeBody.size(); i++)
    {
        if (snakeHeadCenter.x > enemySnake.snakeBody[i].pos.x && snakeHeadCenter.x < enemySnake.snakeBody[i].end.x
            && snakeHeadCenter.y > enemySnake.snakeBody[i].pos.y && snakeHeadCenter.y < enemySnake.snakeBody[i].end.y)
        {
            if (i == 0)
                return 3;
            else
                return 2;
        }
    }
    return 0;
}

bool Snake::abroad(RenderWindow& window)
{
    Vector2f snakeHeadCenter(snakeBody[0].pos.x + snakeBody[0].sectionSize.x / 2, snakeBody[0].pos.y + snakeBody[0].sectionSize.y / 2);
    if (snakeHeadCenter.x > window.getSize().x || snakeHeadCenter.x < 0)
        return true;
    else if (snakeHeadCenter.y > window.getSize().y || snakeHeadCenter.y < 0)
        return true;
    return false;
}

Snake::Snake(Color blockColor, Vector2f spawnPoint)
{
    snakeRect.setFillColor(blockColor);
    snakeBody.push_back(SnakeBlock(spawnPoint, 0));
    snakeImg.loadFromFile("hedgehog.png");
    snakeRect.setSize(snakeBody[0].sectionSize);
    snakeRect.setTexture(&snakeImg);
    snakeRect.setOrigin(snakeRect.getGlobalBounds().width / 2,
    snakeRect.getGlobalBounds().height / 2);
}

int Snake::move(HedgehogBlock& hedgehog, Snake& enemySnake, RenderWindow& window)
{
    if (stepCounter == 10)
    {
        if (selfEating()) return 1;
        else
        {
            int status = snakesCrash(enemySnake);
            if (status)
                return status;
        }
    }
    for (auto& it : snakeBody)
    {
        it.move();
    }

    stepCounter++;
    if (stepCounter == 20)
    {
        stepCounter = 0;
        for (int i = snakeBody.size() - 1; i > 0; i--)
        {
            snakeBody[i].direction = snakeBody[i - 1].direction;
        }

        if (eating(hedgehog))
            hedgehog.resetPos(window, snakeBody, enemySnake.snakeBody);
        if (selfEating()) return 1;
        else return snakesCrash(enemySnake);
    }
    return abroad(window);
}

void Snake::draw(RenderWindow& window)
{
    for (const auto& it : snakeBody)
    {
        rotateImg(it);
        snakeRect.setPosition(it.pos.x + it.sectionSize.x / 2, it.pos.y + it.sectionSize.y / 2);
        window.draw(snakeRect);
    }
}

void Snake::setHeadDir(int _direction) { snakeBody[0].direction = _direction; }

SnakeBlock Snake::getHead() { return snakeBody[0]; }

vector<SnakeBlock> Snake::getBody() { return snakeBody; }

RectangleShape Snake::getRect() { return snakeRect; }

void Game::refreshCounter(Snake& snake1, Snake& snake2)
{
    pointCounter1.setString(to_string(snake1.getBody().size() - 1));
    pointCounter2.setString(to_string(snake2.getBody().size() - 1));
    while (pointCounter2.getPosition().x + pointCounter2.getGlobalBounds().width >= screenSizes[0].width)
        pointCounter2.move(-1, 0);
}

void Game::aboutPage(RenderWindow& window)
{
    Texture background;
    background.loadFromFile("about.jpeg");
    RectangleShape about(Vector2f(screenSizes[0].width, screenSizes[0].height));
    about.setTexture(&background);
    while (1)
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    return;
                }
            }
        }
        window.clear(Color::White);
        window.draw(about);
        window.display();
        sleep(milliseconds(1000 / 60));
    }
}

bool Game::preStart(RenderWindow& window)
{
    Vector2f sectionSize(40, 40);
    wstring strokes[3] = { L"Начать игру" , L"О программе" , L"Выход" };
    vector <RectangleShape> buttons;
    vector <Text> buttonStorkes;
    int indent = 0;
    for (int i = 0; i < 3; i++)
    {
        RectangleShape button(Vector2f(sectionSize.x * 10, sectionSize.y * 3));
        button.setPosition(screenSizes[0].width / 2 - button.getGlobalBounds().width / 2,
            sectionSize.y * (8 + i * 3) + sectionSize.y * indent);
        button.setFillColor(Color(1, 121, 111));
        button.setOutlineThickness(2);
        button.setOutlineColor(Color::Black);
        indent++;

        Text stroke(strokes[i], font, 40);
        stroke.setPosition(screenSizes[0].width / 2 - stroke.getGlobalBounds().width / 2,
            button.getPosition().y + button.getGlobalBounds().height / 2 - stroke.getGlobalBounds().height);
        stroke.setFillColor(Color(192, 192, 192));
        buttons.push_back(button);
        buttonStorkes.push_back(stroke);
    }
    while (1)
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    Vector2f mousePos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
                    for (int i = 0; i < 3; i++)
                    {
                        if (buttons[i].getGlobalBounds().contains(mousePos))
                        {
                            switch (i)
                            {
                            case 0:
                            {
                                return 0;
                                break;
                            }
                            case 1:
                            {
                                aboutPage(window);
                                break;
                            }
                            case 2:
                                return 1;
                            }
                        }
                    }
                }
            }
            if (event.type == Event::MouseMoved)
            {
                Vector2f mousePos(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
                for (int i = 0; i < 3; i++)
                {
                    if (buttons[i].getGlobalBounds().contains(mousePos))
                        buttons[i].setFillColor(Color(0, 166, 147));
                    else
                        buttons[i].setFillColor(Color(1, 121, 111));
                }
            }
        }
        window.clear();
        window.draw(background);
        for (int i = 0; i < 3; i++)
        {
            window.draw(buttons[i]);
            window.draw(buttonStorkes[i]);
        }
        window.display();
        sleep(milliseconds(1000 / 60));
    }
}

void Game::endGame()
{
    Vector2f centerPos(screenSizes[0].width / 2, screenSizes[0].height / 2);
    Text endStroke(L"Игра окончена: " + winner, font, 40);
    endStroke.setFillColor(Color::Cyan);
    endStroke.setPosition(centerPos.x - endStroke.getGlobalBounds().width / 2,
        centerPos.y - endStroke.getGlobalBounds().height / 2);

    Text exitStroke(L"Вернуться в меню", font, 25);
    exitStroke.setFillColor(Color(192, 192, 192));

    RectangleShape exitRect(Vector2f(240, 40));
    exitRect.setPosition(centerPos.x - exitRect.getGlobalBounds().width / 2,
        endStroke.getPosition().y + 100);
    exitRect.setFillColor(Color(1, 121, 111));
    exitRect.setOutlineThickness(2);
    exitRect.setOutlineColor(Color::Black);
    exitStroke.setPosition(centerPos.x - exitStroke.getGlobalBounds().width / 2,
        exitRect.getPosition().y +
        exitRect.getGlobalBounds().height / 2 -
        exitStroke.getGlobalBounds().height);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            Vector2f mousePos(Mouse::getPosition(window).x,
                Mouse::getPosition(window).y);
            if (event.type == Event::Closed ||
                (Keyboard::isKeyPressed(Keyboard::Escape) &&
                    Keyboard::isKeyPressed(Keyboard::LShift)))
                window.close();
            if (event.type == Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == Mouse::Left)
                {
                    if (exitRect.getGlobalBounds().contains(mousePos))
                    {
                        return;
                    }

                }
            }
            if (event.type == Event::MouseMoved)
            {
                if (exitRect.getGlobalBounds().contains(mousePos))
                    exitRect.setFillColor(Color(0, 166, 147));
                else
                    exitRect.setFillColor(Color(1, 121, 111));
            }
        }
        window.draw(endStroke);
        window.draw(exitRect);
        window.draw(exitStroke);
        window.draw(pointCounter1);
        window.draw(pointCounter2);
        window.display();
        sleep(milliseconds(1000 / 60));
    }
}

Game::Game()
{
    settings.antialiasingLevel = 8;
    screenSizes = VideoMode::getFullscreenModes();
    window.create(screenSizes[0], "Snake", Style::Fullscreen, settings);
    backgroundTexture.loadFromFile("weed.jpg");
    background.setSize(Vector2f(screenSizes[0].width, screenSizes[0].height));
    background.setTexture(&backgroundTexture);
    font.loadFromFile("times-new-roman.ttf");
    pointCounter1.setFont(font);
    pointCounter1.setString("0");
    pointCounter1.setFillColor(Color::Red);
    pointCounter1.setPosition(0, 0);
    pointCounter2 = pointCounter1;
    pointCounter2.setPosition(screenSizes[0].width, 0);
    pointCounter2.setFillColor(Color::Cyan);
    refreshTime = 6;
    gameOver = false;
}

Game::Game(int _refreshTime) : refreshTime(_refreshTime)
{
    settings.antialiasingLevel = 8;
    screenSizes = VideoMode::getFullscreenModes();
    window.create(screenSizes[0], "Snake", Style::Fullscreen, settings);
    backgroundTexture.loadFromFile("weed.jpg");
    background.setSize(Vector2f(screenSizes[0].width, screenSizes[0].height));
    background.setTexture(&backgroundTexture);
    font.loadFromFile("times-new-roman.ttf");
    pointCounter1.setFont(font);
    pointCounter1.setString("0");
    pointCounter1.setFillColor(Color::Red);
    pointCounter1.setPosition(0, 0);
    pointCounter2 = pointCounter1;
    pointCounter2.setPosition(screenSizes[0].width, 0);
    pointCounter2.setFillColor(Color::Cyan);
    gameOver = false;
}

void Game::play()
{
    while (1)
    {
        if (gameOver)
        {
            endGame();
            gameOver = false;
        }
        if (preStart(window))
            return;

        Snake snake1(Color::Red, Vector2f(40 * 5, screenSizes[0].height - 40 * 2));
        Snake snake2(Color::Cyan, Vector2f(40 * 40, screenSizes[0].height - 40 * 2));

        vector<SnakeBlock> snakeBody1 = snake1.getBody();
        vector<SnakeBlock> snakeBody2 = snake2.getBody();
        hedgehog.resetPos(window, snakeBody1, snakeBody2);
        snakeBody1.clear(); snakeBody2.clear();
        while (window.isOpen() && !gameOver)
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::KeyPressed)
                {
                    if (Keyboard::isKeyPressed(Keyboard::Escape) &&
                        Keyboard::isKeyPressed(Keyboard::LShift))
                        window.close();
                    if (event.key.code == Keyboard::Up &&
                        snake2.getHead().getDirection() != 2)
                    {
                        snake2.setHeadDir(0);
                        break;
                    }
                    if (event.key.code == Keyboard::Right &&
                        snake2.getHead().getDirection() != 3)
                    {
                        snake2.setHeadDir(1);
                        break;
                    }
                    if (event.key.code == Keyboard::Down &&
                        snake2.getHead().getDirection() != 0)
                    {
                        snake2.setHeadDir(2);
                        break;
                    }
                    if (event.key.code == Keyboard::Left &&
                        snake2.getHead().getDirection() != 1)
                    {
                        snake2.setHeadDir(3);
                        break;
                    }
                    if (event.key.code == Keyboard::W &&
                        snake1.getHead().getDirection() != 2)
                    {
                        snake1.setHeadDir(0);
                        break;
                    }
                    if (event.key.code == Keyboard::D &&
                        snake1.getHead().getDirection() != 3)
                    {
                        snake1.setHeadDir(1);
                        break;
                    }
                    if (event.key.code == Keyboard::S &&
                        snake1.getHead().getDirection() != 0)
                    {
                        snake1.setHeadDir(2);
                        break;
                    }
                    if (event.key.code == Keyboard::A &&
                        snake1.getHead().getDirection() != 1)
                    {
                        snake1.setHeadDir(3);
                        break;
                    }
                }
            }
            for (int i = 0; i < 20; i++)
            {
                window.clear();
                window.draw(background);
                refreshCounter(snake1, snake2);
                window.draw(pointCounter1);
                window.draw(pointCounter2);
                snake1.draw(window);
                snake2.draw(window);
                hedgehog.draw(window);

                int status = snake1.move(hedgehog, snake2, window);
                if (status)
                {
                    gameOver = true;
                    if (status == 3)
                        winner = L"Ничья";
                    else
                        winner = L"Бирюзовый победил";
                    break;
                }

                status = snake2.move(hedgehog, snake1, window);
                if (status)
                {
                    gameOver = true;
                    if (status == 3)
                        winner = L"Ничья";
                    else
                        winner = L"Красный победил";
                    break;
                }
                window.display();
                sleep(milliseconds(refreshTime));
            }
        }
    }
}