#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/Graphics.hpp>

class Game {
private:

    // VARIABLES //
    //Window
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event event;

    //Mouse positions
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    // Resources
    sf::Font font;

    //Textures
    sf::Texture alien;
    sf::Texture alienHeal;
    sf::Texture alienAngry;
    sf::Texture sky;
    sf::Texture target;
    sf::Texture stopGame;
    sf::Texture restart;

    //Text
    sf::Text uiText;
    sf::Text scoreText;

    //Game logic
    bool endGame;
    unsigned highScore;
    unsigned points;
    unsigned maxPoints;
    int health;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;
    bool mouseHeld;
    int enemiesSpawned;
    float gameSpeed;

    //Game objects
    std::vector<sf::Sprite> enemies; // vector containing all enemies there are on the screen
    sf::Sprite enemy;
    sf::Sprite enemyHeal;
    sf::Sprite enemyAngry;
    sf::Sprite background;
    sf::Sprite crosshair;
    sf::Sprite gameOver;
    sf::Sprite retry;

    // FUNCTIONS //
    // Initializer functions
    void initializeVariables();
    void initializeWindow();
    void initializeFonts();
    void initializeText();
    void initializeTextures();
    void initializeSprites();

    // Game logic functions
    void spawnEnemy();

    // Update Functions
    void updateMousePositions();
    void updateText();
    void updateEnemies();
    void updateGameOver();

    // Render Functions
    void pollEvents();
    void renderBackground(sf::RenderTarget& target);
    void renderUI(sf::RenderTarget& target);
    void renderEnemies(sf::RenderTarget& target);
    void renderGameOver(sf::RenderTarget& target);
    void renderCrosshair(sf::RenderTarget& target);

public:
    // Constructors / Destructors
    Game();
    virtual~Game();

    // Functions
    const bool running() const;
    void render();
    void update();
};
