#include "Game.h"

// Constructors / Destructors
Game::Game()
{
    this->initializeVariables();
    this->initializeWindow();
    this->initializeFonts();
    this->initializeText();
    this->initializeTextures();
    this->initializeSprites();

    // Initializes the high score variable once in the constructor instead of the 'initializeVariables()' function
    // as the latter is used to reset all the variables once the game is restarted by the user, which should not apply to the high score
    this->maxPoints = 0;
}

Game::~Game()
{
    delete this->window;
}

// ACCESSORS //
const bool Game::running() const
{
    return this->window->isOpen();
}

// INITIALIZATION FUNCTIONS //
void Game::initializeVariables()
{
    this->endGame = false;
    this->points = 0;
    this->health = 10;
    this->enemySpawnTimer = this->enemySpawnTimerMax = 40.f;
    this->maxEnemies = 10;
    this->mouseHeld = false;
    this->enemiesSpawned = 0;
    this->gameSpeed = 2;
}

void Game::initializeWindow()
{
    this->videoMode.height = 800;
    this->videoMode.width = 800;

    this->window = nullptr;
    this->window = new sf::RenderWindow(this->videoMode, "Alien Extermination! by Mark Onopchenko", sf::Style::Close | sf::Style::Titlebar);


    //Caps the window's FPS to 60 to avoid excessive power consumption
    this->window->setFramerateLimit(60);

    //Hides the default mouse cursor for future replacement
    this->window->setMouseCursorVisible(false);
}

void Game::initializeFonts()
{
    if (!this->font.loadFromFile("images//font.ttf"))
        std::cout << "ERROR::Game::initializeFonts::Failed to load font!\n";
}

void Game::initializeText()
{
    // Initializing the UI text
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(30);
    this->uiText.setFillColor(sf::Color::White);

    // Initializing the text that displays the high score
    this->scoreText.setFont(this->font);
    this->scoreText.setCharacterSize(36);
    this->scoreText.setFillColor(sf::Color::Yellow);
    this->scoreText.setPosition(this->window->getSize().x / 2 - this->scoreText.getGlobalBounds().width / 2 - 80, this->window->getSize().y / 2 - this->scoreText.getGlobalBounds().height / 2 + 80);
}

void Game::initializeTextures()
{
    // if any of the textures fail to load, print an error message and exit the function
    if (!this->alien.loadFromFile("images//alien.png") ||
        !this->sky.loadFromFile("images//sky.jpg") ||
        !this->target.loadFromFile("images//crosshair.png") ||
        !this->alienHeal.loadFromFile("images//alienHealth.webp") ||
        !this->alienAngry.loadFromFile("images//alienAngry.png") ||
        !this->stopGame.loadFromFile("images//gameOver.png") ||
        !this->restart.loadFromFile("images//retry.png"))
    {
        std::cout << "ERROR::Game::initializeTextures()::Failed to load a texture!\n";
        return;
    }

    alien.setSmooth(true);
    alienHeal.setSmooth(true);
    alienAngry.setSmooth(true);
    target.setSmooth(true);
    restart.setSmooth(true);
}

void Game::initializeSprites()
{
    // Initializes the enemy sprites
    // Normal Enemy
    this->enemy.setTexture(alien);
    this->enemy.setScale(sf::Vector2f(0.15f, 0.15f));
    this->enemy.setColor(sf::Color::Magenta);

    // Health Enemy
    this->enemyHeal.setTexture(alienHeal);
    this->enemyHeal.setScale(sf::Vector2f(0.20f, 0.20f));
    this->enemyHeal.setColor(sf::Color::Green); // Light blue color(173, 216, 230)

    // Angry Enemy
    this->enemyAngry.setTexture(alienAngry);
    this->enemyAngry.setScale(sf::Vector2f(0.12f, 0.12f));
    this->enemyAngry.setColor(sf::Color::Red);

    // Initializes the background sprite
    this->background.setTexture(sky);
    this->background.setScale(sf::Vector2f(0.5f, 0.5f));

    // Initializes the player's crosshair
    this->crosshair.setTexture(target);
    this->crosshair.setScale(sf::Vector2f(0.2f, 0.2f));
    
    // Initializes the 'Game Over' sprite
    this->gameOver.setTexture(stopGame);
    this->gameOver.setScale(sf::Vector2f(0.5f, 0.5f));
    this->gameOver.setPosition(this->window->getSize().x / 2 - this->gameOver.getGlobalBounds().width / 2, this->window->getSize().y / 2 - this->gameOver.getGlobalBounds().height / 2);

    // Initializes the retry button sprite
    this->retry.setTexture(restart);
    this->retry.setScale(sf::Vector2f(0.3f, 0.3f));
    this->retry.setPosition(this->window->getSize().x / 2 - this->retry.getGlobalBounds().width / 2, this->window->getSize().y / 2 - this->retry.getGlobalBounds().height / 2 + 220);
}

// UPDATE FUNCTIONS //
void Game::updateEnemies()
{
    if (this->enemies.size() < this->maxEnemies)
    {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
            //Spawn the enemy and reset the timer
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
            enemiesSpawned++;
        }
        else
            this->enemySpawnTimer += 1.f;
    }

    // after 5 enemies have spawned, increase the game speed and the spawn speed slightly
    if (enemiesSpawned == 5)
    {
        this->enemiesSpawned = 0;
        this->gameSpeed += 0.5f;
        this->enemySpawnTimerMax -= 2.0f;
    }

    //Moving and updating the states of the enemies
    for (int i = 0; i < this->enemies.size(); i++)
    {
        this->enemies[i].move(0.f, gameSpeed);

        if (this->enemies[i].getPosition().y > this->window->getSize().y) // if the enemy reaches the bottom of the screen, erase it
        {
            if (enemies[i].getTexture() == &alienAngry) // if a red alien slips through, 5 lives are lost
                this->health -= 5;
            else
                this->health--;

            this->enemies.erase(this->enemies.begin() + i);
            i--;
        }
    }

    //Check if clicked on & the mouse button has not been held down before
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (this->mouseHeld == false)
        {
            this->mouseHeld = true;
            bool deleted = false;
            for (size_t i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView))
                {
                    if (enemies[i].getTexture() == &alienHeal) // if a health-giving alien slips through, 5 lives are gained
                        this->health += 5;

                    //Delete the enemy
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
                    i--;

                    //Gain points
                    this->points++;
                }
            }
        }
    }
    else
        this->mouseHeld = false;
}

void Game::spawnEnemy()
{
    /*
        Spawns enemies and sets their colors and positions
        -Sets a given position
        -Sets a given color
        -Adds enemy to the vector

    */
    int spawn = rand() % 10 + 1; // creates a random number from 1 to 10

    if (spawn <= 3)
    {
        this->enemyAngry.setPosition(static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemyAngry.getTexture()->getSize().x * this->enemyAngry.getScale().x)), -100.f);
        this->enemies.push_back(this->enemyAngry);
    }
    else if (spawn <= 9)
    {
        this->enemy.setPosition(static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getTexture()->getSize().x * this->enemy.getScale().x)), -100.f);
        //Spawn the enemy
        this->enemies.push_back(this->enemy);
    }
    else
    {
        this->enemyHeal.setPosition(static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemyHeal.getTexture()->getSize().x * this->enemyHeal.getScale().x)), -100.f);
        this->enemies.push_back(this->enemyHeal);
    }
}

void Game::updateGameOver()
{
    /*
        Runs when the game is over, and the user may press the retry button
    */
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if (this->retry.getGlobalBounds().contains(this->mousePosView))
        {
            this->initializeVariables();
            for (size_t i = 0; i < this->enemies.size(); i++)
            {
                this->enemies.erase(this->enemies.begin() + i);
                i--;
            }
        }
    }
}

void Game::updateText()
{
    std::stringstream ss1;
    std::stringstream ss2;

    ss1 << "Points : " << this->points << '\n'
        << "Health : " << this->health;
    this->uiText.setString(ss1.str());


    if (this->points > this->maxPoints)
        this->maxPoints = this->points;

    ss2 << "High Score  " << this->maxPoints << "\n    Score  " << this->points;
    this->scoreText.setString(ss2.str());

}

void Game::pollEvents()
{
    //Event polling
    while (this->window->pollEvent(this->event))
    {
        switch (this->event.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;

        case sf::Event::KeyPressed:
            if (this->event.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }
    }
}

void Game::updateMousePositions()
{
    /*
    Updates the mouse positions:
        - Mouse position relative to window (Vector2i)
    */

    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    // mapping the mousePosWindow to float and window view (SFML-specific requirement)
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
    // placing the crosshair sprite on top of the mouse cursor
    this->crosshair.setPosition(sf::Vector2f(this->mousePosView.x - 50, this->mousePosView.y - 40));
}

void Game::update()
{
    this->pollEvents();

    this->updateMousePositions();

    this->updateText();

    //End game condition
    if (this->health <= 0)
    {
        this->endGame = true;
        this->health = 0;
        this->updateGameOver();
    }
    else
        this->updateEnemies();    
}

// RENDER FUNCTIONS //
void Game::render()
{
    /*
        Renders the game objects

        - clear old frame
        - render objects
        - display frame in window
    */
        
    this->window->clear();

    // Draws Background
    this->renderBackground(*this->window);
    // Draws UI
    this->renderUI(*this->window);

    // Draws enemies if it's not the end of the game
    if (this->endGame)
        renderGameOver(*this->window);
    else
        this->renderEnemies(*this->window);

    // Draws the crosshair
    this->renderCrosshair(*this->window);
    
    // Displays the frame
    this->window->display();
}

void Game::renderBackground(sf::RenderTarget& target)
{
    target.draw(this->background);
}

void Game::renderEnemies(sf::RenderTarget& target)
{
    //Move the enemies
    for (auto& c : this->enemies)
        target.draw(c);
}

void Game::renderUI(sf::RenderTarget& target)
{
    target.draw(this->uiText);      
}

void Game::renderGameOver(sf::RenderTarget& target)
{
    target.draw(this->gameOver);
    target.draw(this->retry);
    target.draw(this->scoreText);
}

void Game::renderCrosshair(sf::RenderTarget& target)
{
    target.draw(this->crosshair);
}
