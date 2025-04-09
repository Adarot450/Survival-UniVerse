#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <SFML/Audio.hpp>
#include <sstream>
#include <iomanip>
#include<cmath>
using namespace std;
using namespace sf;


//structs

struct Background {
    Sprite sprite;
    Texture texture;
};


Background menuBG[4];

struct Button {
    Sprite sprite;
    Texture texture;
};

Button playButton;


struct Enemy {
    int HP;
    int DMG;
    int Speed;
};

// Constants
const int PLAYER_SPEED = 200;
const float PLAYER_ANIMATION_RATE = 0.05; // // Indicates how fast are we switching player's texture
const int WHIP_TRAVEL_DISTANCE = 2000; //How far the whip moves away from the player
const float WHIP_RATE = 0.2; //how fast are we moving through whip texture
const float WHIP_COOLDOWN = 1.5f;

//Global VVariables
bool isMoving = false;
bool playerPos = true; // true = left  | false  = right
int walkIndx = 0;
float PLAYER_ANIMATION_TIMER = 0;// Player's texture switch timer (Always set to ZERO)
float whipTimer = 0; //whip's animation timer (always set to ZERO)
int  whipIndx = 0;
float logoScale = 1.5;
int menu = 0; // 0 = main menu | 1 = playing
float whipCooldownTimer = 0;
bool canAttack = true;
//Enemy
int ZombieAnimationIndex = 0;

// Health system
float maxHealth = 100.0f;
float currentHealth = maxHealth;

int bgIndx = 0;

// Scale animation variables
float scaleRange = 0.1f;  // How much it expands/shrinks
float scaler = 2.0f;       // scaler of sin graph
float timeElapsed = 0;    // time tracker
float rotationAngle;




sf::RenderWindow window(sf::VideoMode(1920, 1080), "Survival@Uni-Verse");


// sprite and Textures
Sprite player;
Sprite background;
Sprite whip;
Sprite logo;
Sprite Zombie;

Texture logoTexture;
Texture menuBackgroundTexture;
Texture playerSheet;
Texture backgroundTexture;
Texture whipSheet;
Texture ZombieSheet;


//Shapes
RectangleShape playerHitbox(Vector2f(20, 40));
RectangleShape whipHitbox(Vector2f(50, 125));
RectangleShape healthBarBackground(Vector2f(60, 5));  // Increased width from 40 to 60
RectangleShape healthBarFill(Vector2f(60, 5));       // Increased width from 40 to 60
RectangleShape ZombieHitBox(Vector2f(35, 48));


//miscellaneous
View view(Vector2f(1920 / 2, 1080 / 2), Vector2f(1920, 1080));
float deltaTime;




//Functions Declerations

void Update();
void Start();
void Draw();

void backgroundHandeling();
void playerMovement();
void loadTextures();
void playerAnimation();
void playeCollider();
void playerHitboxHandeling();
void BorderCollision();
void lockViewToBackground();
void whipAnimation();
void whipAnimation();
void whipCollider();
void whipHitboxHandeling();
void mainmenuWidgets();
void logoAnimation();
void menuBgRandomizer();
void healthBarHandling();
void takeDamage(float damage);
void heal(float amount);
void ZombieAnimation();
void enemyCollision();

int main()
{
    Start();
    Clock clock;
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        Update();
        Draw();
    }
    return 0;
}
void Start()
{
    window.setFramerateLimit(144);

    backgroundHandeling();
    loadTextures();
    mainmenuWidgets();
    whip.setPosition(player.getPosition().x - 75, player.getPosition().y - 75); // set the start of the animation
    playerHitboxHandeling();
    whipHitboxHandeling();

    // Initialize health bar
    healthBarBackground.setFillColor(Color::Black);
    healthBarFill.setFillColor(Color::Red);
    healthBarBackground.setOrigin(healthBarBackground.getSize().x / 2, healthBarBackground.getSize().y / 2);
    healthBarFill.setOrigin(healthBarFill.getSize().x / 2, healthBarFill.getSize().y / 2);
    //whip
    whip.setOrigin(whip.getLocalBounds().width / 2, player.getLocalBounds().height / 2);
    whipHitbox.setOrigin(whipHitbox.getLocalBounds().width / 2, whipHitbox.getLocalBounds().height / 2);

    //Enemy

    Zombie.setPosition(background.getGlobalBounds().width / 2, background.getGlobalBounds().height / 2);
    ZombieHitBox.setFillColor(Color::Transparent);
    ZombieHitBox.setOutlineColor(Color::Red);
    ZombieHitBox.setOutlineThickness(2);
}
void Update()
{

    window.setView(view);

    if (menu == 0) { // main menu

        window.draw(menuBG[bgIndx].sprite);
        logoAnimation();

        if (playButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
        {
            playButton.sprite.setTextureRect(IntRect(0, 32, 128, 32));
            playButton.sprite.setColor(sf::Color(200, 200, 200));
            if (Mouse::isButtonPressed(Mouse::Left)) {
                sleep(milliseconds(200));
                menu = 1;
            }
        }

        else
        {
            playButton.sprite.setTextureRect(IntRect(0, 0, 128, 32));
            playButton.sprite.setColor(Color::White);
        }
    }
    else if (menu == 1) { // game
        BorderCollision();
        lockViewToBackground();
        playerMovement();
        playerAnimation();
        playeCollider();

        whipAnimation();
        whipAnimation();
        whipCollider();

    }
    //Enemy
    ZombieHitBox.setPosition(Zombie.getPosition());
    Zombie.setOrigin(Zombie.getLocalBounds().width / 2, Zombie.getLocalBounds().height / 2);
    ZombieHitBox.setOrigin(Zombie.getOrigin());
    ZombieAnimation();

}
void Draw()
{
    window.clear();
    if (menu == 0) { // main menu
        window.draw(menuBG[bgIndx].sprite);
        window.draw(logo);
        window.draw(playButton.sprite);
    }
    else if (menu == 1) { // game
        window.draw(background);
        window.draw(player);
        window.draw(playerHitbox);

        // Draw health bar
        healthBarHandling();
        window.draw(healthBarBackground);
        window.draw(healthBarFill);
        //enemy
        window.draw(ZombieHitBox);
        window.draw(Zombie);
        //whip
        if (whipIndx != 3) {
            window.draw(whip);
            window.draw(whipHitbox);
        }
    }
    window.display(); //Display sprites on screen
}
void backgroundHandeling()
{
    backgroundTexture.loadFromFile("assets/Background.png");
    background.setTexture(backgroundTexture);
    player.setPosition(background.getGlobalBounds().width / 2, background.getGlobalBounds().height / 2);
}
void playerMovement()
{
    isMoving = false;
    if (Keyboard::isKeyPressed(Keyboard::W))
    {
        player.move(0, -PLAYER_SPEED * deltaTime);
        isMoving = true;

    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        player.move(0, PLAYER_SPEED * deltaTime);
        isMoving = true;
    }
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        player.move(-PLAYER_SPEED * deltaTime, 0);
        isMoving = true;
        player.setScale(1, 1);
        playerPos = true; //Player is facing Left
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        player.move(PLAYER_SPEED * deltaTime, 0);
        isMoving = true;
        player.setScale(-1, 1);
        playerPos = false; //Player is facing right

    }
    player.setOrigin(player.getLocalBounds().width / 2, player.getLocalBounds().height / 2);


}
void loadTextures()
{
    playerSheet.loadFromFile("assets/player_sheet.png");
    whipSheet.loadFromFile("assets/whipsheet.png");
    logoTexture.loadFromFile("assets/Logo.png");
    playButton.texture.loadFromFile("assets/play_button.png");
    menuBG[0].texture.loadFromFile("assets/menu_background1.png");
    menuBG[1].texture.loadFromFile("assets/menu_background2.png");
    menuBG[2].texture.loadFromFile("assets/menu_background3.png");
    menuBG[3].texture.loadFromFile("assets/menu_background4.png");
    ZombieSheet.loadFromFile("assets/zombie1.png");
}
void playerAnimation()
{
    PLAYER_ANIMATION_TIMER += deltaTime;

    if (isMoving)
    {
        if (PLAYER_ANIMATION_TIMER >= PLAYER_ANIMATION_RATE)
        {
            PLAYER_ANIMATION_TIMER = 0;
            walkIndx = (walkIndx + 1) % 16;
        }
        // Display the current frame from the vertical sprite sheet.
        player.setTexture(playerSheet);
        player.setTextureRect(IntRect(0, walkIndx * 56, 40, 56));
    }
    else
    {
        // When idle, use a fixed frame (for example, the first frame)
        player.setTexture(playerSheet);
        player.setTextureRect(IntRect(0, 0, 40, 56));
    }
}
void playeCollider()
{

    playerHitbox.setPosition(player.getPosition().x - 10, player.getPosition().y - 20);

}
void playerHitboxHandeling()
{
    playerHitbox.setFillColor(Color::Transparent);
    playerHitbox.setOutlineColor(Color::Red);
    playerHitbox.setOutlineThickness(2);
}
void BorderCollision()
{
    // Get the background boundaries.
    FloatRect bgBounds = background.getGlobalBounds();


    Vector2f pos = player.getPosition(); // Get player's current position.



    FloatRect playerBounds = player.getGlobalBounds();

    // Calculate min and max allowed positions.
    float halfWidth = playerBounds.width / 2;
    float halfHeight = playerBounds.height / 2;

    float minX = bgBounds.left + halfWidth;
    float maxX = bgBounds.left + bgBounds.width - halfWidth;
    float minY = bgBounds.top + halfHeight;
    float maxY = bgBounds.top + bgBounds.height - halfHeight;

    // Lock the position.
    if (pos.x < minX) pos.x = minX;
    if (pos.x > maxX) pos.x = maxX;
    if (pos.y < minY) pos.y = minY;
    if (pos.y > maxY) pos.y = maxY;

    // Update the player's position.
    player.setPosition(pos);
}
void lockViewToBackground()
{
    // Get the background bounds
    FloatRect bgBounds = background.getGlobalBounds();

    // Get half of the view size
    float halfViewWidth = view.getSize().x / 2;
    float halfViewHeight = view.getSize().y / 2;

    // Start with the player's position
    Vector2f desiredCenter = player.getPosition();

    // lock horizontally
    if (desiredCenter.x < bgBounds.left + halfViewWidth)
        desiredCenter.x = bgBounds.left + halfViewWidth;
    if (desiredCenter.x > (bgBounds.left + bgBounds.width) - halfViewWidth)
        desiredCenter.x = (bgBounds.left + bgBounds.width) - halfViewWidth;

    // lock vertically
    if (desiredCenter.y < bgBounds.top + halfViewHeight)
        desiredCenter.y = bgBounds.top + halfViewHeight;
    if (desiredCenter.y > (bgBounds.top + bgBounds.height) - halfViewHeight)
        desiredCenter.y = (bgBounds.top + bgBounds.height) - halfViewHeight;

    // Set the lock center
    view.setCenter(desiredCenter);
}
void updatePlayerAndWhip()
{
    // === Player Animation ===
    PLAYER_ANIMATION_TIMER += deltaTime;

    if (isMoving)
    {
        if (PLAYER_ANIMATION_TIMER >= PLAYER_ANIMATION_RATE)
        {
            PLAYER_ANIMATION_TIMER = 0;
            walkIndx = (walkIndx + 1) % 16;
        }
        player.setTexture(playerSheet);
        player.setTextureRect(IntRect(0, walkIndx * 56, 40, 56));
    }
    else
    {
        player.setTexture(playerSheet);
        player.setTextureRect(IntRect(0, 0, 40, 56));
    }

    // === Whip Animation and Cooldown ===
    whipCooldownTimer += deltaTime;

    if (whipCooldownTimer >= WHIP_COOLDOWN)
    {
        // Set whip initial position based on player facing
        if (whipIndx == 0)
        {
            if (playerPos == 1)
            {
                whip.setPosition(player.getPosition().x - 100, player.getPosition().y - 75);
                whip.setScale(1, 1);
            }
            else if (playerPos == 0)
            {
                whip.setPosition(player.getPosition().x + 100, player.getPosition().y - 75);
                whip.setScale(-1, 1);
            }
        }

        whipTimer += deltaTime;

        if (whipTimer >= WHIP_RATE)
        {
            whipTimer = 0;
            whipIndx = (whipIndx + 1) % 4;

            // Move and animate the whip
            if (whip.getScale() == Vector2f(-1, 1))
            {
                whip.move(-WHIP_TRAVEL_DISTANCE * deltaTime, 0);
            }
            else if (whip.getScale() == Vector2f(1, 1))
            {
                whip.move(WHIP_TRAVEL_DISTANCE * deltaTime, 0);
            }

            whip.setTexture(whipSheet);
            whip.setTextureRect(IntRect(0, whipIndx * 170, 170, 170));

            if (whipIndx == 3)
            {
                whipCooldownTimer = 0; // Reset cooldown after final frame
            }
        }
    }
}
void whipAnimation()
{
    // Update cooldown timer
    whipCooldownTimer += deltaTime;

    // Only animate if cooldown has passed
    if (whipCooldownTimer < WHIP_COOLDOWN)
        return;

    // Set whip position at the final frame based on player direction
    if (playerPos == 1 && whipIndx == 3) // Facing left
    {
        whip.setScale(1, 1);
        whip.setPosition(player.getPosition().x - 100, player.getPosition().y - 75);
    }
    else if (playerPos == 0 && whipIndx == 3) // Facing right
    {
        whip.setScale(-1, 1);
        whip.setPosition(player.getPosition().x + 100, player.getPosition().y - 75);
    }

    // Animate whip
    whipTimer += deltaTime;

    if (whipTimer >= WHIP_RATE)
    {
        whipTimer = 0;
        whipIndx = (whipIndx + 1) % 4;

        if (whip.getScale() == Vector2f(-1, 1)) // Facing right
        {
            if (whipIndx != 3)
            {
                whip.move(WHIP_TRAVEL_DISTANCE * deltaTime, 0);
            }
            else
            {
                whip.setPosition(player.getPosition().x + 100, player.getPosition().y - 75);
                whipCooldownTimer = 0; // Start cooldown after final frame
            }
        }
        else if (whip.getScale() == Vector2f(1, 1)) // Facing left
        {
            if (whipIndx != 3)
            {
                whip.move(-WHIP_TRAVEL_DISTANCE * deltaTime, 0);
            }
            else
            {
                whip.setPosition(player.getPosition().x - 100, player.getPosition().y - 75);
                whipCooldownTimer = 0; // Start cooldown after final frame
            }
        }
    }

    whip.setTexture(whipSheet);
    whip.setTextureRect(IntRect(0, whipIndx * 170, 170, 170));
}

void whipCollider()
{
    Vector2f whipPos = whip.getPosition(); // this is the origin position
    Vector2f offset;

    if (whip.getScale().x < 0) // Facing right
    {
        offset.x = -25; // visually compensate
    }
    else // Facing left
    {
        offset.x = 25;
    }

    offset.y = 85;

    whipHitbox.setPosition(whipPos.x + offset.x, whipPos.y + offset.y);
}
void whipHitboxHandeling()
{
    whipHitbox.setFillColor(Color::Transparent);
    whipHitbox.setOutlineColor(Color::Red);
    whipHitbox.setOutlineThickness(2);
}
void healthBarHandling()
{
    // Position health bar closer to player
    Vector2f playerPos = player.getPosition();
    healthBarBackground.setPosition(playerPos.x, playerPos.y + 30);
    healthBarFill.setPosition(playerPos.x, playerPos.y + 30);

    // Update health bar fill based on current health
    float healthPercentage = currentHealth / maxHealth;
    Vector2f newSize = healthBarFill.getSize();
    newSize.x = healthBarBackground.getSize().x * healthPercentage;
    healthBarFill.setSize(newSize);
}
void takeDamage(float damage)
{
    currentHealth -= damage;
    if (currentHealth < 0)
        currentHealth = 0;
}
void heal(float amount)
{
    currentHealth += amount;
    if (currentHealth > maxHealth)
        currentHealth = maxHealth;
}
void mainmenuWidgets() {
    logo.setTexture(logoTexture);
    logo.setOrigin(logo.getLocalBounds().width / 2, logo.getLocalBounds().height / 2);
    logo.setPosition(1920 / 2, 1080 / 5);


    playButton.sprite.setTexture(playButton.texture);
    playButton.sprite.setOrigin(playButton.sprite.getLocalBounds().width / 2, playButton.sprite.getLocalBounds().height / 2);
    playButton.sprite.setPosition(1920 / 2, 1080 / 2);
    playButton.sprite.setScale(3.0f, 3.0f);

    menuBG[0].sprite.setTexture(menuBG[0].texture);
    menuBG[1].sprite.setTexture(menuBG[1].texture);
    menuBG[2].sprite.setTexture(menuBG[2].texture);
    menuBG[3].sprite.setTexture(menuBG[3].texture);

    menuBgRandomizer();
}
void menuBgRandomizer() {
    srand(time(NULL)); // seed rand() to generate true random values
    bgIndx = rand() % 4;

}
void logoAnimation() {

    timeElapsed += deltaTime;

    logoScale = 2.5 + scaleRange * sin(scaler * timeElapsed);;
    logo.setScale(logoScale, logoScale);

    float rotationAngle = sin(2 * (5 + timeElapsed));
    logo.setRotation(rotationAngle);
}
void ZombieAnimation() {
    int colSize = 2;
    Zombie.setTexture(ZombieSheet);
    ZombieAnimationIndex = (ZombieAnimationIndex + 1) % colSize;
    Zombie.setTextureRect(IntRect(ZombieAnimationIndex * 35, 0, 35, 48));
}
void enemyCollision();




/*player sheet : 26 rows , first 16 are walking  size of each is 46 width and 56 height*/



