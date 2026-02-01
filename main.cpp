#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;


class GameConstants
{
public:
    // Window dimensions
    static const int WINDOW_WIDTH = 1000;
    static const int WINDOW_HEIGHT = 700;

    // Game settings
    static const int MAX_SCORE = 10;
    static const int PADDLE_WIDTH = 20;
    static const int PADDLE_HEIGHT = 120;
    static const int BALL_RADIUS = 15;
    static const float BALL_SPEED;
    static const float PADDLE_SPEED;

    // Colors
    static const Color BACKGROUND_COLOR;
    static const Color PADDLE_COLOR;
    static const Color BALL_COLOR;
    static const Color TEXT_COLOR;
    static const Color LINE_COLOR;
    static const Color PLAYER1_COLOR;
    static const Color PLAYER2_COLOR;
    static const Color INPUT_COLOR;
};

const float GameConstants::BALL_SPEED = 6.0f;
const float GameConstants::PADDLE_SPEED = 8.0f;
const Color GameConstants::BACKGROUND_COLOR = Color(10, 20, 40);
const Color GameConstants::PADDLE_COLOR = Color::Green;
const Color GameConstants::BALL_COLOR = Color::Red;
const Color GameConstants::TEXT_COLOR = Color::White;
const Color GameConstants::LINE_COLOR = Color(100, 100, 100, 100);
const Color GameConstants::PLAYER1_COLOR = Color::Blue;
const Color GameConstants::PLAYER2_COLOR = Color::Magenta;
const Color GameConstants::INPUT_COLOR = Color::Yellow;


class GameObject
{
protected:
    float xPosition;
    float yPosition;
    float width;
    float height;
    Color objectColor;

public:

    GameObject(float x, float y, float w, float h, Color c)
    {
        xPosition = x;
        yPosition = y;
        width = w;
        height = h;
        objectColor = c;
    }

    // Virtual destructor
    virtual ~GameObject()
    {

    }


    virtual void draw(RenderWindow& window) = 0;

    // Getters
    float getX() const
    {
        return xPosition;
    }

    float getY() const
    {
        return yPosition;
    }

    float getWidth() const
    {
        return width;
    }

    float getHeight() const
    {
        return height;
    }

    Color getColor() const
    {
        return objectColor;
    }

    // Setters
    void setX(float x)
    {
        xPosition = x;
    }

    void setY(float y)
    {
        yPosition = y;
    }

    void setColor(Color c)
    {
        objectColor = c;
    }

    // Check if this object collides with another
    bool checkCollision(const GameObject& other) const
    {
        bool collisionX = false;
        bool collisionY = false;

        // Check X axis collision
        if (xPosition < other.xPosition + other.width)
        {
            if (xPosition + width > other.xPosition)
            {
                collisionX = true;
            }
        }

        // Check Y axis collision
        if (yPosition < other.yPosition + other.height)
        {
            if (yPosition + height > other.yPosition)
            {
                collisionY = true;
            }
        }

        // Return true only if both axes collide
        if (collisionX && collisionY)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
};


class Paddle : public GameObject
{
private:
    float speed;
    int playerNumber;
    int score;
    RectangleShape paddleShape;
    Color originalColor;

public:
    // Constructor
    Paddle(int playerNum) : GameObject(0, 0, GameConstants::PADDLE_WIDTH,
                                       GameConstants::PADDLE_HEIGHT,
                                       GameConstants::PADDLE_COLOR)
    {
        playerNumber = playerNum;
        score = 0;
        speed = GameConstants::PADDLE_SPEED;
        paddleShape.setSize(Vector2f(width, height));


        if (playerNumber == 1)
        {
            originalColor = GameConstants::PLAYER1_COLOR;
        }
        else
        {
            originalColor = GameConstants::PLAYER2_COLOR;
        }

        paddleShape.setFillColor(originalColor);
        paddleShape.setOutlineThickness(2);
        paddleShape.setOutlineColor(Color::White);

        resetPosition();
    }


    void resetPosition()
    {
        if (playerNumber == 1)
        {
            xPosition = 50;
        }
        else
        {
            xPosition = GameConstants::WINDOW_WIDTH - 50 - width;
        }

        yPosition = GameConstants::WINDOW_HEIGHT / 2 - height / 2;
        paddleShape.setPosition(xPosition, yPosition);
    }


    void moveUp()
    {
        if (yPosition > 0)
        {
            yPosition = yPosition - speed;
            if (yPosition < 0)
            {
                yPosition = 0;
            }
            paddleShape.setPosition(xPosition, yPosition);
        }
    }


    void moveDown()
    {
        if (yPosition + height < GameConstants::WINDOW_HEIGHT)
        {
            yPosition = yPosition + speed;
            if (yPosition + height > GameConstants::WINDOW_HEIGHT)
            {
                yPosition = GameConstants::WINDOW_HEIGHT - height;
            }
            paddleShape.setPosition(xPosition, yPosition);
        }
    }


    void draw(RenderWindow& window)
    {
        window.draw(paddleShape);
    }

    int getScore() const
    {
        return score;
    }

    int getPlayerNumber() const
    {
        return playerNumber;
    }

    RectangleShape getShape() const
    {
        return paddleShape;
    }

    float getCenterY() const
    {
        return yPosition + height / 2;
    }


    void setScore(int s)
    {
        score = s;
    }

    void incrementScore()
    {
        score = score + 1;
    }

    // Update paddle position
    void updatePosition()
    {
        paddleShape.setPosition(xPosition, yPosition);
    }

    // Flash paddle when score is made
    void flash()
    {
        paddleShape.setFillColor(Color::Yellow);
    }

    // Reset paddle color
    void resetColor()
    {
        paddleShape.setFillColor(originalColor);
    }
};


class Ball : public GameObject
{
private:
    float velocityX;
    float velocityY;
    float speed;
    CircleShape ballShape;
    bool isActive;

public:

    Ball() : GameObject(GameConstants::WINDOW_WIDTH / 2,
                       GameConstants::WINDOW_HEIGHT / 2,
                       GameConstants::BALL_RADIUS * 2,
                       GameConstants::BALL_RADIUS * 2,
                       GameConstants::BALL_COLOR)
    {
        speed = GameConstants::BALL_SPEED;
        isActive = true;
        ballShape.setRadius(GameConstants::BALL_RADIUS);
        ballShape.setFillColor(objectColor);
        ballShape.setOutlineThickness(2);
        ballShape.setOutlineColor(Color::White);
        ballShape.setPosition(xPosition, yPosition);

        reset();
    }


    void reset()
    {
        xPosition = GameConstants::WINDOW_WIDTH / 2 - GameConstants::BALL_RADIUS;
        yPosition = GameConstants::WINDOW_HEIGHT / 2 - GameConstants::BALL_RADIUS;

        // Random direction for X
        int randomDirectionX = rand() % 2;
        int directionX;
        if (randomDirectionX == 0)
        {
            directionX = 1;
        }
        else
        {
            directionX = -1;
        }

        // Random direction for Y
        int randomDirectionY = rand() % 2;
        int directionY;
        if (randomDirectionY == 0)
        {
            directionY = 1;
        }
        else
        {
            directionY = -1;
        }

        // Random Y velocity
        int randomY = rand() % 3 + 1;
        velocityX = speed * directionX;
        velocityY = speed * randomY * 0.5 * directionY;

        ballShape.setPosition(xPosition, yPosition);
        isActive = true;
    }

    // Update ball position
    void update()
    {
        if (isActive == false)
        {
            return;
        }

        xPosition = xPosition + velocityX;
        yPosition = yPosition + velocityY;

        // Bounce off top
        if (yPosition <= 0)
        {
            velocityY = -velocityY;
            yPosition = 0;
        }

        // Bounce off bottom
        if (yPosition + height >= GameConstants::WINDOW_HEIGHT)
        {
            velocityY = -velocityY;
            yPosition = GameConstants::WINDOW_HEIGHT - height;
        }

        ballShape.setPosition(xPosition, yPosition);
    }

    // Bounce from paddle with angle calculation
    void bounceFromPaddle(const Paddle& paddle)
    {
        velocityX = -velocityX * 1.05f;

        // Calculate bounce angle based on where the ball hit the paddle
        float ballCenterY = yPosition + GameConstants::BALL_RADIUS;
        float paddleCenterY = paddle.getCenterY();
        float hitPosition = ballCenterY - paddleCenterY;
        float normalizedHit = hitPosition / (paddle.getHeight() / 2);
        velocityY = normalizedHit * speed;

        // Limit maximum angle
        if (velocityY > speed)
        {
            velocityY = speed;
        }

        if (velocityY < -speed)
        {
            velocityY = -speed;
        }
    }

    // Draw the ball
    void draw(RenderWindow& window)
    {
        if (isActive == true)
        {
            window.draw(ballShape);
        }
    }

    // Check if ball is out of bounds
    bool isOutOfBounds() const
    {
        if (xPosition < 0)
        {
            return true;
        }

        if (xPosition > GameConstants::WINDOW_WIDTH)
        {
            return true;
        }

        return false;
    }

    // Check which side the ball went out
    int getOutSide() const
    {
        if (xPosition < 0)
        {
            return 1;
        }

        if (xPosition > GameConstants::WINDOW_WIDTH)
        {
            return 2;
        }

        return 0;
    }

    // Getters
    float getVelocityX() const
    {
        return velocityX;
    }

    float getVelocityY() const
    {
        return velocityY;
    }

    bool getIsActive() const
    {
        return isActive;
    }

    CircleShape getShape() const
    {
        return ballShape;
    }

    // Setters
    void setVelocityX(float vx)
    {
        velocityX = vx;
    }

    void setVelocityY(float vy)
    {
        velocityY = vy;
    }

    void setIsActive(bool active)
    {
        isActive = active;
    }
};

class GameText
{
private:
    Font font;
    Text text;
    bool fontLoaded;

public:
    GameText()
    {
        fontLoaded = false;
        loadFont();
    }

    // Try to load font
    void loadFont()
    {
        bool loadSuccess = font.loadFromFile("arial.ttf");

        if (loadSuccess == false)
        {
            loadSuccess = font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");

            if (loadSuccess == false)
            {
                cout << "Warning: Could not load font. Text will not display properly." << endl;
                return;
            }
        }

        fontLoaded = true;
        text.setFont(font);
    }

    // Draw text at position
    void draw(RenderWindow& window, string str, int size, float x, float y, Color color = GameConstants::TEXT_COLOR)
    {
        if (fontLoaded == false)
        {
            return;
        }

        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(color);
        text.setPosition(x, y);
        window.draw(text);
    }

    // Draw centered text
    void drawCentered(RenderWindow& window, string str, int size, float y, Color color = GameConstants::TEXT_COLOR)
    {
        if (fontLoaded == false)
        {
            return;
        }

        text.setString(str);
        text.setCharacterSize(size);
        text.setFillColor(color);

        FloatRect textBounds = text.getLocalBounds();
        float centerX = textBounds.left + textBounds.width / 2.0f;
        float centerY = textBounds.top + textBounds.height / 2.0f;
        text.setOrigin(centerX, centerY);
        text.setPosition(GameConstants::WINDOW_WIDTH / 2.0f, y);

        window.draw(text);
    }

    bool isFontLoaded() const
    {
        return fontLoaded;
    }
};

class GameSounds
{
private:
    SoundBuffer paddleHitBuffer;
    SoundBuffer wallHitBuffer;
    SoundBuffer scoreBuffer;
    Sound paddleHitSound;
    Sound wallHitSound;
    Sound scoreSound;
    bool soundsLoaded;

public:
    GameSounds()
    {
        soundsLoaded = false;
        loadSounds();
    }

    void loadSounds()
    {
        soundsLoaded = true;
    }

    void playPaddleHit()
    {
        if (soundsLoaded == true)
        {
            paddleHitSound.play();
        }
    }

    void playWallHit()
    {
        if (soundsLoaded == true)
        {
            wallHitSound.play();
        }
    }

    void playScore()
    {
        if (soundsLoaded == true)
        {
            scoreSound.play();
        }
    }

    bool areSoundsLoaded() const
    {
        return soundsLoaded;
    }
};


class HighScoreEntry
{
private:
    string playerName;
    int score;

public:
    HighScoreEntry()
    {
        playerName = "";
        score = 0;
    }

    HighScoreEntry(string name, int s)
    {
        playerName = name;
        score = s;
    }

    string getName() const
    {
        return playerName;
    }

    int getScore() const
    {
        return score;
    }

    void setName(string name)
    {
        playerName = name;
    }

    void setScore(int s)
    {
        score = s;
    }
};


class HighScoreManager
{
private:
    string filename;
    vector<HighScoreEntry> highScores;
    const int MAX_HIGH_SCORES = 10;

    // Function to compare two high score entries
    static bool compareEntries(const HighScoreEntry& a, const HighScoreEntry& b)
    {
        if (a.getScore() > b.getScore())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

public:
    HighScoreManager()
    {
        filename = "highscores.txt";
        loadHighScores();
    }

    // Load high scores from file
    void loadHighScores()
    {
        ifstream file(filename.c_str());

        if (file.is_open() == false)
        {
            cout << "No existing high score file found. Creating new one." << endl;
            return;
        }

        highScores.clear();
        string name;
        int score;

        while (file >> name >> score)
        {
            if (highScores.size() < MAX_HIGH_SCORES)
            {
                HighScoreEntry entry(name, score);
                highScores.push_back(entry);
            }
            else
            {
                break;
            }
        }

        file.close();
        sortHighScores();
    }

    // Save high scores to file
    void saveHighScores()
    {
        ofstream file(filename.c_str());

        if (file.is_open() == false)
        {
            cout << "Error: Could not save high scores!" << endl;
            return;
        }

        for (size_t i = 0; i < highScores.size(); i++)
        {
            file << highScores[i].getName() << " " << highScores[i].getScore() << endl;
        }

        file.close();
    }

    // Check if score qualifies for high score
    bool isHighScore(int score)
    {
        if (highScores.size() < MAX_HIGH_SCORES)
        {
            return true;
        }

        if (score > highScores.back().getScore())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // Add a new high score
    void addHighScore(const string& name, int score)
    {
        HighScoreEntry newEntry(name, score);
        highScores.push_back(newEntry);
        sortHighScores();

        // Keep only top scores
        if (highScores.size() > MAX_HIGH_SCORES)
        {
            highScores.resize(MAX_HIGH_SCORES);
        }

        saveHighScores();
    }

    // Sort high scores in descending order
    void sortHighScores()
    {
        for (size_t i = 0; i < highScores.size(); i++)
        {
            for (size_t j = i + 1; j < highScores.size(); j++)
            {
                if (highScores[j].getScore() > highScores[i].getScore())
                {
                    HighScoreEntry temp = highScores[i];
                    highScores[i] = highScores[j];
                    highScores[j] = temp;
                }
            }
        }
    }

    // Get high scores for display
    vector<HighScoreEntry> getHighScores() const
    {
        return highScores;
    }

    // Display high scores
    void displayHighScores(RenderWindow& window, GameText& textRenderer)
    {
        textRenderer.drawCentered(window, "HIGH SCORES", 50, 100, Color::Yellow);

        float startY = 180;
        for (size_t i = 0; i < highScores.size(); i++)
        {
            string entryNumber = to_string(i + 1) + ". ";
            string entryName = highScores[i].getName();
            string entryScore = to_string(highScores[i].getScore());
            string entry = entryNumber + entryName + " - " + entryScore;
            textRenderer.drawCentered(window, entry, 30, startY + i * 40);
        }

        textRenderer.drawCentered(window, "Press SPACE to return to menu", 24, 600);
    }
};


class GameManager
{
private:
    RenderWindow gameWindow;
    Paddle* player1;
    Paddle* player2;
    Ball* gameBall;
    GameText textRenderer;
    GameSounds gameSounds;
    HighScoreManager highScoreManager;

    int gameState;
    int winner;
    bool isTwoPlayer;
    Clock gameClock;
    Time deltaTime;
    int flashTimer;
    int flashPlayer;

    // Player names
    string player1Name;
    string player2Name;
    string computerName;

    // Name input variables
    int nameInputState;
    string currentInput;
    Clock cursorBlinkClock;
    bool cursorVisible;
    bool nameEntered;

public:
    GameManager()
    {
        // Create window
        gameWindow.create(VideoMode(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT),
                         "A Ping Pong Game ");

        // Initialize game objects
        player1 = new Paddle(1);
        player2 = new Paddle(2);
        gameBall = new Ball();

        // Game state
        gameState = 0;
        winner = 0;
        isTwoPlayer = true;
        flashTimer = 0;
        flashPlayer = 0;

        // Initialize player names with default values
        player1Name = "Player 1";
        player2Name = "Player 2";
        computerName = "Computer";

        // Name input initialization
        nameInputState = 0;
        currentInput = "";
        cursorVisible = true;
        nameEntered = false;

        // Window settings
        gameWindow.setFramerateLimit(60);
        gameWindow.setKeyRepeatEnabled(false);

        // Seed random number generator
        srand(static_cast<unsigned int>(time(0)));

        cout << "Game initialized successfully!" << endl;
        cout << "Window size: " << GameConstants::WINDOW_WIDTH << "x" << GameConstants::WINDOW_HEIGHT << endl;
    }

    ~GameManager()
    {
        delete player1;
        delete player2;
        delete gameBall;
        cout << "Game cleaned up successfully!" << endl;
    }

    // Main game loop
    void run()
    {
        while (gameWindow.isOpen())
        {
            handleEvents();
            update();
            render();
        }
    }

    // Handle input events
    void handleEvents()
    {
        Event event;
        while (gameWindow.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                gameWindow.close();
            }

            // Keyboard presses
            if (event.type == Event::KeyPressed)
            {
                handleKeyPress(event.key.code);
            }
        }
    }

    // Handle keyboard input
    void handleKeyPress(Keyboard::Key key)
    {
        // If we're getting player names
        if (nameInputState > 0)
        {
            handleNameInputKey(key);
            return;
        }

        if (gameState == 0)
        {
            handleMenuInput(key);
        }
        else if (gameState == 1)
        {
            handleGameInput(key);
        }
        else if (gameState == 2)
        {
            if (key == Keyboard::P || key == Keyboard::Escape)
            {
                gameState = 1;
            }
        }
        else if (gameState == 3)
        {
            handleGameOverInput(key);
        }
        else if (gameState == 4)
        {
            if (key == Keyboard::Space || key == Keyboard::Escape)
            {
                gameState = 0;
            }
        }
    }

    // Handle name input keys
    void handleNameInputKey(Keyboard::Key key)
    {
        if (key == Keyboard::Enter)
        {
            // Save the entered name
            if (nameInputState == 1)
            {
                // Player 1 name
                if (currentInput.empty() == false)
                {
                    player1Name = currentInput;
                }

                if (isTwoPlayer == true)
                {

                    nameInputState = 2;
                    currentInput = "";
                    nameEntered = false;
                }
                else
                {
                    // Single player mode, start game
                    nameInputState = 0;
                    startNewGame();
                }
            }
            else if (nameInputState == 2)
            {
                // Player 2 name
                if (currentInput.empty() == false)
                {
                    player2Name = currentInput;
                }

                // Start the game
                nameInputState = 0;
                startNewGame();
            }
        }
        else if (key == Keyboard::Escape)
        {
            // Cancel name input
            nameInputState = 0;
            currentInput = "";
        }
        else if (key == Keyboard::BackSpace)
        {
            // Remove last character
            if (currentInput.empty() == false)
            {
                currentInput = currentInput.substr(0, currentInput.length() - 1);
            }
        }
        else
        {
            // Check for letter keys
            if (key >= Keyboard::A && key <= Keyboard::Z)
            {
                char c = 'A' + (key - Keyboard::A);
                if (currentInput.length() < 10)
                {
                    currentInput = currentInput + c;
                }
            }
            else if (key >= Keyboard::Num0 && key <= Keyboard::Num9)
            {
                char c = '0' + (key - Keyboard::Num0);
                if (currentInput.length() < 10)
                {
                    currentInput = currentInput + c;
                }
            }
            else if (key == Keyboard::Space)
            {
                if (currentInput.length() < 10 && currentInput.empty() == false)
                {
                    currentInput = currentInput + " ";
                }
            }
        }
    }

    // Handle menu input
    void handleMenuInput(Keyboard::Key key)
    {
        if (key == Keyboard::Num1 || key == Keyboard::Enter)
        {
            isTwoPlayer = true;
            // Start getting player names
            nameInputState = 1;
            currentInput = "";
            cursorVisible = true;
            nameEntered = false;
            cursorBlinkClock.restart();
        }
        else if (key == Keyboard::Num2)
        {
            isTwoPlayer = false;
            // Start getting player 1 name only
            nameInputState = 1;
            currentInput = "";
            cursorVisible = true;
            nameEntered = false;
            cursorBlinkClock.restart();
        }
        else if (key == Keyboard::Num3)
        {
            gameState = 4;
        }
        else if (key == Keyboard::Num4)
        {
            loadGame();
        }
        else if (key == Keyboard::Escape)
        {
            gameWindow.close();
        }
    }

    // Handle in game input
    void handleGameInput(Keyboard::Key key)
    {
        if (key == Keyboard::Escape || key == Keyboard::P)
        {
            gameState = 2;
        }
        else if (key == Keyboard::R)
        {
            resetGame();
        }
        else if (key == Keyboard::S)
        {
            saveGame();
        }
    }

    // Handle game over input
    void handleGameOverInput(Keyboard::Key key)
    {
        if (key == Keyboard::Enter || key == Keyboard::Space)
        {
            gameState = 0;
        }
        else if (key == Keyboard::R)
        {
            startNewGame();
        }
    }

    // Update game logic
    void update()
    {
        // Update cursor blink for name input
        if (nameInputState > 0)
        {
            Time elapsed = cursorBlinkClock.getElapsedTime();
            if (elapsed.asMilliseconds() > 500)
            {
                cursorVisible = !cursorVisible;
                cursorBlinkClock.restart();
            }
        }

        if (gameState != 1)
        {
            return;
        }

        // Update ball
        gameBall->update();

        // Player 1 controls (W/S keys)
        if (Keyboard::isKeyPressed(Keyboard::W))
        {
            player1->moveUp();
        }

        if (Keyboard::isKeyPressed(Keyboard::D))
        {
            player1->moveDown();
        }

        // Player 2 controls
        if (isTwoPlayer == true)
        {
            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                player2->moveUp();
            }

            if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                player2->moveDown();
            }
        }
        else
        {
            controlAI();
        }

        // Check collisions
        checkCollisions();

        // Check if ball is out of bounds
        if (gameBall->isOutOfBounds() == true)
        {
            handleScore();
        }

        // Check for winner
        if (player1->getScore() >= GameConstants::MAX_SCORE)
        {
            winner = 1;
            gameState = 3;
            checkHighScore(player1->getScore());
        }
        else if (player2->getScore() >= GameConstants::MAX_SCORE)
        {
            winner = 2;
            gameState = 3;
            checkHighScore(player2->getScore());
        }

        // Update flash timer
        updateFlashEffect();
    }

    // Control AI for single player mode
    void controlAI()
    {
        float ballCenterY = gameBall->getY() + GameConstants::BALL_RADIUS;
        float paddleCenterY = player2->getCenterY();

        if (ballCenterY < paddleCenterY - 20)
        {
            player2->moveUp();
        }
        else if (ballCenterY > paddleCenterY + 20)
        {
            player2->moveDown();
        }
    }

    // Check for collisions between ball and paddles
    void checkCollisions()
    {
        // Check collision with player 1 paddle
        float ballX = gameBall->getX();
        float ballY = gameBall->getY();
        float ballWidth = gameBall->getWidth();
        float ballHeight = gameBall->getHeight();

        float p1X = player1->getX();
        float p1Y = player1->getY();
        float p1Width = player1->getWidth();
        float p1Height = player1->getHeight();

        bool collisionP1 = false;

        if (ballX < p1X + p1Width)
        {
            if (ballX + ballWidth > p1X)
            {
                if (ballY < p1Y + p1Height)
                {
                    if (ballY + ballHeight > p1Y)
                    {
                        collisionP1 = true;
                    }
                }
            }
        }

        if (collisionP1 == true)
        {
            gameBall->bounceFromPaddle(*player1);
            gameSounds.playPaddleHit();
        }


        float p2X = player2->getX();
        float p2Y = player2->getY();
        float p2Width = player2->getWidth();
        float p2Height = player2->getHeight();

        bool collisionP2 = false;

        if (ballX < p2X + p2Width)
        {
            if (ballX + ballWidth > p2X)
            {
                if (ballY < p2Y + p2Height)
                {
                    if (ballY + ballHeight > p2Y)
                    {
                        collisionP2 = true;
                    }
                }
            }
        }

        if (collisionP2 == true)
        {
            gameBall->bounceFromPaddle(*player2);
            gameSounds.playPaddleHit();
        }
    }


    void handleScore()
    {
        int outSide = gameBall->getOutSide();

        if (outSide == 1)
        {
            player2->incrementScore();
            gameSounds.playScore();
            flashPlayer = 2;
            flashTimer = 30;
        }
        else if (outSide == 2)
        {
            player1->incrementScore();
            gameSounds.playScore();
            flashPlayer = 1;
            flashTimer = 30;
        }

        gameBall->reset();
        player1->resetPosition();
        player2->resetPosition();
    }

    // Update flash effect for scoring
    void updateFlashEffect()
    {
        if (flashTimer > 0)
        {
            flashTimer = flashTimer - 1;

            if (flashPlayer == 1)
            {
                player1->flash();
            }
            else if (flashPlayer == 2)
            {
                player2->flash();
            }

            if (flashTimer == 0)
            {
                player1->resetColor();
                player2->resetColor();
                flashPlayer = 0;
            }
        }
    }

    // Check if score qualifies for high score
    void checkHighScore(int score)
    {
        if (highScoreManager.isHighScore(score) == true)
        {
            if (winner == 1)
            {
                highScoreManager.addHighScore(player1Name, score);
            }
            else
            {
                highScoreManager.addHighScore(player2Name, score);
            }
        }
    }

    // Render everything
    void render()
    {
        gameWindow.clear(GameConstants::BACKGROUND_COLOR);

        if (nameInputState > 0)
        {
            renderNameInput();
        }
        else if (gameState == 0)
        {
            renderMenu();
        }
        else if (gameState == 1)
        {
            renderGame();
        }
        else if (gameState == 2)
        {
            renderPaused();
        }
        else if (gameState == 3)
        {
            renderGameOver();
        }
        else if (gameState == 4)
        {
            renderHighScores();
        }

        gameWindow.display();
    }

    // Render name input screen
    void renderNameInput()
    {
        if (nameInputState == 1)
        {

            textRenderer.drawCentered(gameWindow, "ENTER PLAYER 1 NAME", 50, 150, Color::Cyan);


            textRenderer.drawCentered(gameWindow, "Type name using keyboard (max 10 letters)", 24, 220);
            textRenderer.drawCentered(gameWindow, "Press ENTER to continue, ESC to cancel", 20, 260);


            drawInputBox(350, currentInput, cursorVisible);


            textRenderer.drawCentered(gameWindow, "Player 1 will appear as:", 24, 450);
            string displayName;
            if (currentInput.empty())
            {
                displayName = "Player 1";
            }
            else
            {
                displayName = currentInput;
            }
            textRenderer.drawCentered(gameWindow, displayName, 36, 500, GameConstants::PLAYER1_COLOR);
        }
        else if (nameInputState == 2)
        {

            textRenderer.drawCentered(gameWindow, "ENTER PLAYER 2 NAME", 50, 150, Color::Cyan);

            // Show player 1's name
            textRenderer.drawCentered(gameWindow, "Player 1: " + player1Name, 28, 220, GameConstants::PLAYER1_COLOR);

            // Instructions
            textRenderer.drawCentered(gameWindow, "Type name using keyboard (max 10 letters)", 24, 280);
            textRenderer.drawCentered(gameWindow, "Press ENTER to continue, ESC to cancel", 20, 320);

            // Draw input box
            drawInputBox(400, currentInput, cursorVisible);

            // Preview
            textRenderer.drawCentered(gameWindow, "Player 2 will appear as:", 24, 500);
            string displayName;
            if (currentInput.empty())
            {
                displayName = "Player 2";
            }
            else
            {
                displayName = currentInput;
            }
            textRenderer.drawCentered(gameWindow, displayName, 36, 550, GameConstants::PLAYER2_COLOR);
        }
    }

    // Draw input box with cursor
    void drawInputBox(float yPos, string text, bool showCursor)
    {
        // Draw background rectangle
        RectangleShape inputBox(Vector2f(400, 50));
        inputBox.setPosition(GameConstants::WINDOW_WIDTH / 2 - 200, yPos);
        inputBox.setFillColor(Color(30, 30, 60, 200));
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(Color::White);
        gameWindow.draw(inputBox);

        // Draw text
        if (text.empty() == false)
        {
            textRenderer.drawCentered(gameWindow, text, 36, yPos + 25, GameConstants::INPUT_COLOR);
        }
        else
        {
            textRenderer.drawCentered(gameWindow, "Type here...", 36, yPos + 25, Color(150, 150, 150));
        }


        if (showCursor && nameInputState > 0)
        {
            // Simple cursor - just a vertical line
            float cursorX;
            if (text.empty())
            {
                cursorX = GameConstants::WINDOW_WIDTH / 2 - 190;
            }
            else
            {

                cursorX = GameConstants::WINDOW_WIDTH / 2 - 190 + (text.length() * 18);
            }

            RectangleShape cursor(Vector2f(2, 30));
            cursor.setPosition(cursorX, yPos + 10);
            cursor.setFillColor(Color::White);
            gameWindow.draw(cursor);
        }
    }


    void renderMenu()
    {
        textRenderer.drawCentered(gameWindow, "Welcome To Ping Pong ", 50, 80, Color::Cyan);

        textRenderer.drawCentered(gameWindow, "1. Start Two Player Game", 36, 180);
        textRenderer.drawCentered(gameWindow, "2. Play with Computer", 36, 230);
        textRenderer.drawCentered(gameWindow, "3. View High Scores", 36, 280);
        textRenderer.drawCentered(gameWindow, "4. Load Saved Game", 36, 330);
        textRenderer.drawCentered(gameWindow, "ESC. Exit Game", 36, 380);

        textRenderer.drawCentered(gameWindow, "Player 1: W/D Keys", 24, 460);
        textRenderer.drawCentered(gameWindow, "Player 2: Up/Down Arrows", 24, 500);
        textRenderer.drawCentered(gameWindow, "P: Pause  R: Reset  S: Save", 24, 540);

        vector<HighScoreEntry> highScores = highScoreManager.getHighScores();
        if (highScores.empty() == false)
        {
            string highScoreName = highScores[0].getName();
            int highScoreValue = highScores[0].getScore();
            string highScoreStr = "High Score: " + highScoreName + " - " + to_string(highScoreValue);
            textRenderer.drawCentered(gameWindow, highScoreStr, 28, 600, Color::Yellow);
        }
    }


    void renderGame()
    {
        drawCenterLine();

        player1->draw(gameWindow);
        player2->draw(gameWindow);
        gameBall->draw(gameWindow);

        int score1 = player1->getScore();
        int score2 = player2->getScore();
        string scoreText = to_string(score1) + "   :   " + to_string(score2);
        textRenderer.drawCentered(gameWindow, scoreText, 80, 30);

        // Display player names
        string leftPlayerName = player1Name;
        string rightPlayerName;

        if (isTwoPlayer == true)
        {
            rightPlayerName = player2Name;
        }
        else
        {
            rightPlayerName = computerName;
        }

        // Adjust positions to fit on screen
        textRenderer.draw(gameWindow, leftPlayerName, 24, 150, 100, GameConstants::PLAYER1_COLOR);

        // Calculate width of right player name
        int rightNameWidth = rightPlayerName.length() * 15; // Approximate width
        int rightNameX = GameConstants::WINDOW_WIDTH - 50 - rightNameWidth;
        if (rightNameX < GameConstants::WINDOW_WIDTH - 200)
        {
            rightNameX = GameConstants::WINDOW_WIDTH - 200;
        }
        textRenderer.draw(gameWindow, rightPlayerName, 24, rightNameX, 100, GameConstants::PLAYER2_COLOR);

        string modeText;
        if (isTwoPlayer == true)
        {
            modeText = "TWO PLAYER MODE";
        }
        else
        {
            modeText = "VS COMPUTER MOD";
        }



        textRenderer.draw(gameWindow, "P: Pause  R: Reset  S: Save  ESC: Menu", 18, 10,
                         GameConstants::WINDOW_HEIGHT - 30);
    }


    void drawCenterLine()
    {
        RectangleShape line(Vector2f(2, GameConstants::WINDOW_HEIGHT));
        line.setPosition(GameConstants::WINDOW_WIDTH / 2, 0);
        line.setFillColor(GameConstants::LINE_COLOR);
        gameWindow.draw(line);

        for (int i = 0; i < GameConstants::WINDOW_HEIGHT; i = i + 40)
        {
            RectangleShape dash(Vector2f(2, 20));
            dash.setPosition(GameConstants::WINDOW_WIDTH / 2, i);
            dash.setFillColor(Color::White);
            gameWindow.draw(dash);
        }
    }


    void renderPaused()
    {
        renderGame();

        RectangleShape overlay(Vector2f(GameConstants::WINDOW_WIDTH, GameConstants::WINDOW_HEIGHT));
        overlay.setFillColor(Color(0, 0, 0, 150));
        gameWindow.draw(overlay);

        textRenderer.drawCentered(gameWindow, "GAME PAUSED", 70, 250, Color::Yellow);
        textRenderer.drawCentered(gameWindow, "Press P or ESC to continue", 30, 350);

        int score1 = player1->getScore();
        int score2 = player2->getScore();
        string scoreStr = "Current Score: " + to_string(score1) + " - " + to_string(score2);
        textRenderer.drawCentered(gameWindow, scoreStr, 36, 420);
    }


    void renderGameOver()
    {
        string winnerText;
        Color winnerColor;
        string winnerName;

        if (winner == 1)
        {
            winnerName = player1Name;
            winnerColor = GameConstants::PLAYER1_COLOR;
        }
        else
        {
            if (isTwoPlayer == true)
            {
                winnerName = player2Name;
            }
            else
            {
                winnerName = computerName;
            }
            winnerColor = GameConstants::PLAYER2_COLOR;
        }

        textRenderer.drawCentered(gameWindow, "GAME OVER", 80, 120, Color::Red);
        textRenderer.drawCentered(gameWindow, winnerName + " WINS!", 60, 220, winnerColor);

        int score1 = player1->getScore();
        int score2 = player2->getScore();
        string finalScore = "Final Score: " + to_string(score1) + " - " + to_string(score2);
        textRenderer.drawCentered(gameWindow, finalScore, 40, 320);

        textRenderer.drawCentered(gameWindow, "Press ENTER to return to menu", 30, 420);
        textRenderer.drawCentered(gameWindow, "Press R to play again", 30, 470);

        int winningScore;
        if (winner == 1)
        {
            winningScore = player1->getScore();
        }
        else
        {
            winningScore = player2->getScore();
        }

        if (highScoreManager.isHighScore(winningScore) == true)
        {
            textRenderer.drawCentered(gameWindow, "NEW HIGH SCORE!", 40, 550, Color::Yellow);
        }
    }


    void renderHighScores()
    {
        highScoreManager.displayHighScores(gameWindow, textRenderer);
    }


    void startNewGame()
    {
        player1->setScore(0);
        player2->setScore(0);
        player1->resetPosition();
        player2->resetPosition();
        gameBall->reset();
        gameState = 1;
        winner = 0;
        flashTimer = 0;
        flashPlayer = 0;
    }


    void resetGame()
    {
        player1->setScore(0);
        player2->setScore(0);
        player1->resetPosition();
        player2->resetPosition();
        gameBall->reset();
        flashTimer = 0;
        flashPlayer = 0;
    }

    // Save game state to file
    void saveGame()
    {
        ofstream saveFile("game_save.dat");

        if (saveFile.is_open() == false)
        {
            cout << "Error: Could not save game!" << endl;
            return;
        }

        saveFile << player1->getScore() << endl;
        saveFile << player2->getScore() << endl;
        saveFile << isTwoPlayer << endl;
        saveFile << gameState << endl;
        saveFile << player1Name << endl;
        saveFile << player2Name << endl;

        saveFile.close();
        cout << "Game saved successfully!" << endl;
    }

    // Load game state from file
    void loadGame()
    {
        ifstream loadFile("game_save.dat");

        if (loadFile.is_open() == false)
        {
            cout << "Error: No saved game found!" << endl;
            return;
        }

        int score1;
        int score2;
        bool twoPlayer;
        int state;
        string name1;
        string name2;

        loadFile >> score1;
        loadFile >> score2;
        loadFile >> twoPlayer;
        loadFile >> state;
        loadFile >> name1;
        loadFile >> name2;

        player1->setScore(score1);
        player2->setScore(score2);
        isTwoPlayer = twoPlayer;
        gameState = state;
        player1Name = name1;
        player2Name = name2;

        loadFile.close();
        cout << "Game loaded successfully!" << endl;
    }
};


int main()
{

    cout<<"PING PONG GAME"<<endl;

    GameManager game;
    game.run();

    cout<<"Game ended successfully"<<endl;
    return 0;
}
