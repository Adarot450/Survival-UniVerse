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


// Constants
const float PLAYER_ANIMATION_RATE = 0.05; // Indicates how fast are we switching player's texture
const float LOGO_ANIMATION_RATE = 0.06; // Indicates how fast are we switching logo's texture
const int WHIP_TRAVEL_DISTANCE = 2000; //How far the whip moves away from the player
const float WHIP_RATE = 0.025; //how fast are we moving through whip texture
const float WHIP_COOLDOWN = 1.0f;
const float PROJECTILE_SPEED = 400.0f;  // Adjust speed as needed
const float PROJECTILE_COOLDOWN = 0.7f;  // Cooldown between auto shots
const float EnemyAnimationRate = 0.135f;  //  How fast we are switching Enemy's texture
const int numOfZombieTypes = 10;
const int UPGRADES_NUM = 6;     //Upgrades number
float ProjectileDamage = 10.0;
float projectileCooldown = 0.5f;
float projectileTimer = 0.0f;
float projectileDespawnTime = 3.0f;
float whipDamage = 10.0;

//Global Variables

//Upgrades
int healingUpgradeLevel = 0;
bool isUpgrading = false;
bool isMenuOpen = false;
bool isProjectileUnlocked = false;
int whipLvl = 1;
int swings = 0;

int playerSpeed = 200;
bool isDead = false;
bool isPaused = false;
bool isMoving = false;
bool playerPos = true; // true = left  | false  = right
int walkIndx = 0;
int logoIndx = 0;
float logoAnimationTimer = 0;// Player's texture switch timer (Always set to ZERO)
float playerAnimationTimer = 0;// Player's texture switch timer (Always set to ZERO)
float whipTimer = 0; //whip's animation timer (always set to ZERO)
int  whipIndx = 11;
int menu = 0; // 0 = main menu | 1 = playing
float whipCooldownTimer = 0;
bool canAttack = true;
float timeSinceLastHit = 999;
float SpawnDelay = 0.5;
float SpawnTimer = 0;
float clickRegisterTimer = 0.1f;
int zombiesKilled;

// Health system
float maxHealth = 100.0f;
float currentHealth = maxHealth;
int bgIndx = 0;
int coins = 0;

// XP system
float maxXP = 10.0f;
float currentXP = 0.0f;
int playerLevel = 1;



sf::RenderWindow window(sf::VideoMode(1920, 1080), "Survival@Uni-Verse", Style::Fullscreen);
Font font;
Clock timerClock;
Time totalPausedTime = Time::Zero;
Time pauseStart;

// Sound Buffers
SoundBuffer clickBuffer;
SoundBuffer deathBuffer;
SoundBuffer gameOverBuffer;
SoundBuffer footstepsBuffer;
SoundBuffer enemyAppearBuffer;
SoundBuffer enemyHitBuffer;
SoundBuffer playerHitBuffer;
SoundBuffer healthBuffer;
SoundBuffer bossBuffer;
SoundBuffer coinBuffer;
SoundBuffer levelCompleteBuffer;

// Sound Instances
Sound clickSound;
Sound deathSound;
Sound gameOverSound;
Sound footstepsSound;
Sound enemyAppearSound;
Sound enemyHitSound;
Sound playerHitSound;
Sound healthSound;
Sound bossSound;
Sound coinSound;
Sound levelCompleteSound;

// Background Music
Music backgroundMusic;

// sprite and Textures
Sprite player;
Sprite background;
Sprite whip;
Sprite logo;
Sprite xpBarSprite;  // New XP bar sprite

//texts
Text scoreText;
Text timerText;
Text gameoverText[4];
Text coinsText;

Texture logoTexture;
Texture menuBackgroundTexture;
Texture playerSheet;
Texture backgroundTexture;
Texture whipSheet;
Texture xpBarTexture;  // New XP bar texture


//Shapes
RectangleShape playerHitbox(Vector2f(20, 40));
RectangleShape whipHitbox(Vector2f(300, 45));
RectangleShape healthBarBackground(Vector2f(60, 5));
RectangleShape healthBarFill(Vector2f(60, 5));
RectangleShape xpBarFill(Vector2f(0, 15));       // Start with 0 width to be empty


//miscellaneous
View view(Vector2f(0, 0), Vector2f(1920, 1080));

float deltaTime;


//Functions Declerations

void Update();
void Start();
void Draw();

void logoAnimation();                       // Aly
void mainmenuWidgets();                     // Aly
void mainMenuButtons();                     // Aly
void pauseMenu();                           // Aly
void backtomenuButtonHandler();             // Aly
void pauseButtonHandler();                  // Aly
string scoreFormatHandler(int score);       // Aly
int time();                                 // Aly 
string timerFormatHandler(int time);        // Aly
void resetGame();                           // Aly
void gameoverWidgets();                     // Aly
void gameoverMenuHandler();                 // Aly
void whipAnimation();                       // Aly & Amr
void backgroundHandeling();                 // Amr
void playerMovement();                      // Amr
void playerAnimation();                     // Amr
void playeCollider();                       // Amr
void playerHitboxHandeling();               // Amr
void BorderCollision();                     // Amr
void lockViewToBackground();                // Amr
void whipCollider();                        // Amr
void whipHitboxHandeling();                 // Amr
void whipDmg();                             // Amr
string coinFormatHandler(int coins);        // Amr
void ZombieHandler();                       // Amr & Adam
void SpwaningZombies();                     // Adam
void separateZombies();                     // Adam
Vector2f Normalize(Vector2f vector);        // Adam
void zombieInitalization();                 // Adam
void bleedEffect();                         // Yassin & Adam
void loadTextures();                        // Yassin
void healthBarHandling();                   // Yassin
void addXp(float xpToAdd);                  // Yassin
void takeDamage(float damage);              // Yassin
void heal(float amount);                    // Yassin
void createProjectile();                    // Yassin
void updateProjectile();                    // Yassin
void createXPOrb(Vector2f position);        // Yassin
void updateXPOrbs();                        // Yassin 
Vector2f getDirectionToNearestZombie();     // Yassin
void powerUps();                            // Marwan
void healingUpgrade();                      // Marwan
void drawPowerUpsMenu();                    // Marwan
void upgradeItemsHandeling();               // Marwan
void maxHealthIncrease(int value, int limit);//Marwan
void increasePlayerSpeed(int value);        // Marwan
void upgradesMenuText();                    // Marwan
void upgradeItemsName();                    // Marwan
void upgradesTextHandeling();               // Marwan
void loadSounds();                          // Maritsia
void playBackgroundMusic();                 // Maritsia
void playEnemyAppearSound();                // Maritsia
void playFootstepsSound();                  // Maritsia
void playEnemyHitSound();                   // Maritsia
void playPlayerHitSound();                  // Maritsia



//structs

struct UpgradesItem
{
    Sprite sprite;
    Texture texture;
    Font font;
    Text text;
    int healingUpgradeLevel = 0;
    int maxHealthLevel = 0;
    int defenseUpgradeLevel = 0;
    int increasePlayerSpeedLevel = 0;
};
UpgradesItem upgradeLevel;
UpgradesItem allUpgrades[UPGRADES_NUM];
int offSetX[3] = { -510,0,510 };
int upgradesIndices[3];

struct Background {
    Sprite sprite;
    Texture texture;
};

Background menuBG[10]; // 0:Main menu || 1:Pause Menu || 2:Gameover || 3:Upgrading

struct Projectile {
    Sprite sprite;
    Vector2f direction;
    float speed;
    bool active = false;
};

Projectile projectile;        // Single projectile instead of vector
Texture projectileTexture;

struct Button {
    Sprite sprite;
    Texture texture;
};

//menu
Button playButton;
Button shopButton;
Button optionsButton;
Button quitButton;

//ingame
Button pauseButton;
Button backButton;
Button backtomenuButton;


// Add after other structs
struct XPOrb {
    Sprite sprite;
};

// Add after other global variables
vector<XPOrb> xpOrbs;
Texture xpOrbTexture;

Texture zombieTextureSheets[numOfZombieTypes];

struct Enemies {
    //Adam

    Sprite Shape;
    int type; // 1 :: normal || 2 :: ice
    RectangleShape HitBox;
    Vector2f velocity;
    int hitboxWidth;
    int hitboxHeight;
    int spriteWidth;
    int spriteHeight;
    int DMG;
    int HP;
    int speed;
    float attackRate;
    float attackTimer = 0;
    int colSize;
    int AnimationIndex = 0;
    float AnimtaionTimer = 0;
    float AnimtaionRate;
    bool isDead = false;
    float lastHitTime = 0.0f;  // Track when this zombie was last hit from whip
    bool canBeHit = true;      // Track if this zombie can be hit from whip

    void Start() {
        Shape.setTexture(zombieTextureSheets[type]);
        Shape.setTextureRect(IntRect(AnimationIndex * spriteWidth, 0, spriteWidth, spriteHeight));
        Shape.setOrigin(Shape.getLocalBounds().width / 2, Shape.getLocalBounds().height / 2);

        HitBox.setSize(Vector2f(hitboxWidth, hitboxHeight));
        HitBox.setOrigin(HitBox.getLocalBounds().width / 2, HitBox.getLocalBounds().height / 2);
        HitBox.setFillColor(Color::Transparent);
        //HitBox.setOutlineColor(Color::Red);
        //HitBox.setOutlineThickness(2);

    }

    void Update() {
        CalcDirection();
        walk();
        Animtaion();
        Attack();
        Die();

    }

    void CalcDirection() {
        velocity = player.getPosition() - Shape.getPosition(); //get line from player to zombie
        velocity = Normalize(velocity); //Normalize velocity to mulltiply it by speed
    }

    void walk() {
        Shape.move(velocity.x * speed * deltaTime, velocity.y * speed * deltaTime);
        HitBox.setPosition(Shape.getPosition());
    }
    void Animtaion() {
        AnimtaionTimer += deltaTime;

        //animate
        if (AnimtaionTimer >= AnimtaionRate) {
            AnimtaionTimer = 0;
            Shape.setTexture(zombieTextureSheets[type]);


            AnimationIndex = (AnimationIndex + 1) % colSize;
            Shape.setTextureRect(IntRect(AnimationIndex * spriteWidth, 0, spriteWidth, spriteHeight));

        }
        //turn left and right
        if (Shape.getPosition().x > player.getPosition().x) {
            Shape.setScale(1, 1);
        }
        else {
            Shape.setScale(-1, 1);
        }
    }
    void Attack() {
        if (HitBox.getGlobalBounds().intersects(playerHitbox.getGlobalBounds())) {
            attackTimer += deltaTime;
            if (attackTimer >= attackRate) {
                attackTimer = 0;
                timeSinceLastHit = 0;
                takeDamage(DMG);
            }
        }
    }

    void Die() {
        if (HP <= 0) {
            isDead = true;

        }
    }

}ZombieTypes[10];

//vectors
vector<Enemies>Zombies;

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

    font.loadFromFile("assets/Pixel_Game.otf");
    scoreText.setFont(font);
    timerText.setFont(font);
    coinsText.setFont(font);

    gameoverWidgets();
    backgroundHandeling();
    loadTextures();
    mainmenuWidgets();
    playerHitboxHandeling();
    whipHitboxHandeling();

    loadSounds();

    playBackgroundMusic();

    // Initialize health bar
    healthBarBackground.setFillColor(Color::Black);
    healthBarFill.setFillColor(Color::Red);
    healthBarBackground.setOrigin(healthBarBackground.getSize().x / 2, healthBarBackground.getSize().y / 2);
    healthBarFill.setOrigin(healthBarFill.getSize().x / 2, healthBarFill.getSize().y / 2);

    // Initialize XP bar
    xpBarFill.setFillColor(Color::Cyan);
    xpBarSprite.setScale(2, 2);
    xpBarFill.setScale(1.85, 2.5);

    //whip
    whip.setOrigin(whip.getLocalBounds().width / 2, player.getLocalBounds().height / 2);
    whipHitbox.setOrigin(whipHitbox.getLocalBounds().width / 2, whipHitbox.getLocalBounds().height / 2);

    //position setting for xp bars
    xpBarSprite.setPosition((1920 / 50) + 310, 1080 / 100);
    xpBarFill.setPosition((1920 / 50) + 350, (1080 / 50) + 68);

    //position setting for pause button
    pauseButton.sprite.setPosition(96, 54);

    //pause manu
    backButton.sprite.setOrigin(backButton.sprite.getLocalBounds().width / 2, backButton.sprite.getLocalBounds().height / 2);
    backButton.sprite.setPosition(1920 / 1.65, 1080 / 2);
    backButton.sprite.setScale(1.5, 1.5);

    //gameover menu
    backtomenuButton.sprite.setOrigin(backtomenuButton.sprite.getLocalBounds().width / 2, backtomenuButton.sprite.getLocalBounds().height / 2);


    //xpbar
    xpBarFill.setFillColor(Color(0, 255, 255));

    // Upgrades

    allUpgrades[0].sprite.setScale(5, 5);
    allUpgrades[1].sprite.setScale(5, 5);
    allUpgrades[2].sprite.setScale(5, 5);
    allUpgrades[3].sprite.setScale(5, 5);
    allUpgrades[4].sprite.setScale(5, 5);
    allUpgrades[5].sprite.setScale(5, 5);

    upgradesMenuText();

    //Enemies
    zombieInitalization();
}
void Update()
{


    window.setView(view);



    if (menu == 0) { // main menu

        logoAnimation();
        mainMenuButtons();

    }
    else if (menu == 1) { // game

        //UI



        if (isDead) {
            backtomenuButtonHandler();
            return;
        }

        if (isUpgrading) {
            powerUps();
            upgradeItemsHandeling();
            return;
        }

        if (isPaused) {
            pauseMenu();
            logoAnimation();
            backtomenuButtonHandler();
            return;
        }

        pauseButtonHandler();

        scoreText.setString(scoreFormatHandler(zombiesKilled));
        timerText.setString(timerFormatHandler(time()));
        coinsText.setString(coinFormatHandler((coins)));

        BorderCollision();
        lockViewToBackground();
        playerMovement();
        playerAnimation();
        playeCollider();
        whipAnimation();
        whipCollider();
        bleedEffect();
        whipDmg();


        // Handle projectile
        if (isProjectileUnlocked)
        {
            createProjectile();
            updateProjectile();
        }

        //Enemy
        ZombieHandler();
        SpwaningZombies();
        separateZombies();

        // run upgrades
        healingUpgrade();

        //orbs
        updateXPOrbs();

    }
}
void Draw()
{
    window.clear();
    if (menu == 0) { // main menu
        window.setView(window.getDefaultView());
        window.draw(menuBG[0].sprite);
        window.draw(logo);
        window.draw(playButton.sprite);
        window.draw(shopButton.sprite);
        window.draw(optionsButton.sprite);
        window.draw(quitButton.sprite);
    }
    else if (menu == 1) { // game
        // Draw game world (with game view)
        window.setView(view);
        window.draw(background);
        window.draw(player);
        window.draw(playerHitbox);

        for (const auto& orb : xpOrbs) {
            window.draw(orb.sprite);
        }
        // Draw projectile if active
        if (projectile.active)
        {
            window.draw(projectile.sprite);
        }

        if (whipIndx != 12) {
            window.draw(whip);
            window.draw(whipHitbox);
        }

        //Draw Zombies
        for (int i = 0; i < Zombies.size(); i++) {
            window.draw(Zombies[i].HitBox);
            window.draw(Zombies[i].Shape);
        }

        // Draw health bar
        healthBarHandling();
        window.draw(healthBarBackground);
        window.draw(healthBarFill);
        // Draw XP orbs



        // Draw UI elements (with fixed view)
        View fixedView(FloatRect(0, 0, 1920, 1080)); // yassin
        window.setView(fixedView); // yassin

        // Draw XP bar
        window.draw(xpBarFill);
        window.draw(xpBarSprite);
        window.draw(scoreText);
        window.draw(timerText);
        window.draw(coinsText);

        // pause menu


        if (isDead) {
            window.draw(menuBG[2].sprite);
            window.draw(backtomenuButton.sprite);
            window.draw(scoreText);
            window.draw(timerText);
            window.draw(coinsText);
            gameoverMenuHandler();
        }

        if (isUpgrading) {
            drawPowerUpsMenu();
        }

        if (!isPaused) {
            window.draw(pauseButton.sprite);
        }
        else
        {
            window.draw(menuBG[1].sprite);
            window.draw(backButton.sprite);
            window.draw(backtomenuButton.sprite);
            window.draw(logo);
        }
    }
    window.display();
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
        player.move(0, -playerSpeed * deltaTime);
        isMoving = true;
        //playFootstepsSound();
    }
    if (Keyboard::isKeyPressed(Keyboard::S))
    {
        player.move(0, playerSpeed * deltaTime);
        isMoving = true;
        //playFootstepsSound();
    }
    if (Keyboard::isKeyPressed(Keyboard::A))
    {
        player.move(-playerSpeed * deltaTime, 0);
        isMoving = true;
        player.setScale(1, 1);
        playerPos = true; //Player is facing Left
        //playFootstepsSound();
    }
    if (Keyboard::isKeyPressed(Keyboard::D))
    {
        player.move(playerSpeed * deltaTime, 0);
        isMoving = true;
        player.setScale(-1, 1);
        playerPos = false; //Player is facing right
        //playFootstepsSound();

    }
    player.setOrigin(player.getLocalBounds().width / 2, player.getLocalBounds().height / 2);


}
void loadTextures()
{
    playerSheet.loadFromFile("assets/player_sheet.png");
    whipSheet.loadFromFile("assets/whipsheet.png");
    whip.setTexture(whipSheet);
    logoTexture.loadFromFile("assets/Logo.png");
    playButton.texture.loadFromFile("assets/play_button.png");
    shopButton.texture.loadFromFile("assets/shop_button.png");
    optionsButton.texture.loadFromFile("assets/options_button.png");
    quitButton.texture.loadFromFile("assets/quit_button.png");
    menuBG[0].texture.loadFromFile("assets/menu_background.png");
    menuBG[1].texture.loadFromFile("assets/pause_background.png");
    menuBG[1].sprite.setTexture(menuBG[1].texture);
    menuBG[2].texture.loadFromFile("assets/gameover_background.png");
    menuBG[2].sprite.setTexture(menuBG[2].texture);
    menuBG[3].texture.loadFromFile("assets/upgrade_background.png");
    menuBG[3].sprite.setTexture(menuBG[3].texture);
    xpBarTexture.loadFromFile("assets/xpbar.png");
    xpBarSprite.setTexture(xpBarTexture);
    projectileTexture.loadFromFile("assets/projectile.png");
    pauseButton.texture.loadFromFile("assets/pause_button.png");
    pauseButton.sprite.setTexture(pauseButton.texture);
    backButton.texture.loadFromFile("assets/back_button.png");
    backButton.sprite.setTexture(backButton.texture);
    backtomenuButton.texture.loadFromFile("assets/backtomenu_button.png");
    backtomenuButton.sprite.setTexture(backtomenuButton.texture);
    xpOrbTexture.loadFromFile("assets/xpOrb.png");

    //upgrades
    allUpgrades[0].texture.loadFromFile("assets/maxhealth_upgrade.png");
    allUpgrades[0].sprite.setTexture(allUpgrades[0].texture);
    allUpgrades[1].texture.loadFromFile("assets/defense_upgrade.png");
    allUpgrades[1].sprite.setTexture(allUpgrades[1].texture);
    allUpgrades[2].texture.loadFromFile("assets/whip_upgrade.png");
    allUpgrades[2].sprite.setTexture(allUpgrades[2].texture);
    allUpgrades[3].texture.loadFromFile("assets/regen_upgrade.png");
    allUpgrades[3].sprite.setTexture(allUpgrades[3].texture);
    allUpgrades[4].texture.loadFromFile("assets/secondweapon_upgrade.png");
    allUpgrades[4].sprite.setTexture(allUpgrades[4].texture);
    allUpgrades[5].texture.loadFromFile("assets/speed_upgrade.png");
    allUpgrades[5].sprite.setTexture(allUpgrades[5].texture);
}
void playerAnimation()
{
    playerAnimationTimer += deltaTime;

    if (isMoving)
    {
        if (playerAnimationTimer >= PLAYER_ANIMATION_RATE)
        {
            playerAnimationTimer = 0;
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
    //playerHitbox.setOutlineColor(Color::Red);
    //playerHitbox.setOutlineThickness(2);
}
void BorderCollision()
{
    // Get the background boundaries.
    FloatRect bgBounds = background.getGlobalBounds();


    Vector2f pos = player.getPosition(); // Get player's current position.



    FloatRect playerBounds = player.getGlobalBounds();

    // Calculate min and F allowed positions.
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
void whipAnimation()
{
    // Update cooldown timer
    whipCooldownTimer += deltaTime;

    // Only animate if cooldown has passed
    if (whipCooldownTimer < WHIP_COOLDOWN)
        return;

    // Animate whip
    whipTimer += deltaTime;

    if (whipTimer >= WHIP_RATE)
    {
        whipTimer = 0;
        whipIndx = (whipIndx + 1) % 13;

        whip.setTextureRect(IntRect(0, whipIndx * 99, 405, 99));

        // Set position per frame
        if (whip.getScale() == Vector2f(-1, 1)) { // Right
            playPlayerHitSound();
            whip.setPosition(player.getPosition().x + 250, player.getPosition().y - 50);
        }
        else { // Left
            playPlayerHitSound();
            whip.setPosition(player.getPosition().x - 250, player.getPosition().y - 50);
        }

        // End of full swing animation
        if (whipIndx == 12)
        {
            if (whipLvl == 2 && swings == 0)
            {
                // Flip for second swing
                swings = 1;
                whipIndx = -1; // So next loop starts at 0 again (because (-1 + 1) % 13 == 0)

                if (whip.getScale() == Vector2f(1, 1))      whip.setScale(-1, 1); // Left -> Right
                else if (whip.getScale() == Vector2f(-1, 1)) whip.setScale(1, 1);  // Right -> Left
            }
            else
            {
                // Done with both swings (or single if level 1)
                whipCooldownTimer = 0;
                swings = 0;

                // Reset to player center
                whip.setScale(player.getScale());
                whip.setPosition(player.getPosition().x, player.getPosition().y);
            }
        }
    }

    // Extra cosmetic position when frame is 12
    if (whipIndx == 12)
    {
        if (playerPos == 1) // Facing left
        {
            whip.setScale(1, 1);
            whip.setPosition(player.getPosition().x - 250, player.getPosition().y - 50);
        }
        else // Facing right
        {
            whip.setScale(-1, 1);
            whip.setPosition(player.getPosition().x + 250, player.getPosition().y - 50);
        }
    }
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

    offset.y = 45;

    whipHitbox.setPosition(whipPos.x + offset.x, whipPos.y + offset.y);
}
void whipHitboxHandeling()
{
    whipHitbox.setFillColor(Color::Transparent);
    //whipHitbox.setOutlineColor(Color::Red);
    //whipHitbox.setOutlineThickness(2);
}
void healthBarHandling()
{
    if (isDead) {
        return;
    }

    // Position health bar closer to player
    Vector2f playerPos = player.getPosition();
    healthBarBackground.setPosition(playerPos.x, playerPos.y + 30);
    healthBarFill.setPosition(playerPos.x, playerPos.y + 30);

    // Update health bar fill based on current health
    float healthPercentage = currentHealth / maxHealth;
    Vector2f newSize = healthBarFill.getSize();
    newSize.x = healthBarBackground.getSize().x * healthPercentage;
    healthBarFill.setSize(newSize);

    if (currentHealth <= 0.05) {
        isDead = true;
        gameOverSound.play();
    }

}//Yassin Amr
void addXp(float xpToAdd)
{
    // Add XP from orb collection
    currentXP += xpToAdd;
    if (currentXP > maxXP)
    {
        currentXP = 0; // so that it never exceeds max
        maxXP *= 2;
        isUpgrading = true;
        isMenuOpen = true;
        levelCompleteSound.play();
    }

    // Update the fill size based on XP percentage
    float xpPercentage = currentXP / maxXP;
    Vector2f newSize = xpBarFill.getSize();
    newSize.x = xpBarTexture.getSize().x * xpPercentage;
    xpBarFill.setSize(newSize);

}//Yassin Amr
void takeDamage(float damage)
{
    currentHealth -= damage;
    playEnemyHitSound();
    if (currentHealth < 0)
        currentHealth = 0;
}//Yassin Amr
void heal(float amount)
{
    currentHealth += amount;
    if (currentHealth > maxHealth)
        currentHealth = maxHealth;
}//Yassin Amr
void mainmenuWidgets() {
    logo.setTexture(logoTexture);
    logo.setOrigin(413.5, 263);
    logo.setPosition(1920 / 4, 1080 / 2);
    logo.setScale(1, 1);

    playButton.sprite.setTexture(playButton.texture);
    playButton.sprite.setOrigin(playButton.sprite.getLocalBounds().width / 2, playButton.sprite.getLocalBounds().height / 2);
    playButton.sprite.setPosition(1470, 400);
    playButton.sprite.setScale(2.0f, 2.0f);

    shopButton.sprite.setTexture(shopButton.texture);
    shopButton.sprite.setOrigin(shopButton.sprite.getLocalBounds().width / 2, shopButton.sprite.getLocalBounds().height / 2);
    shopButton.sprite.setPosition(1470, 650);
    shopButton.sprite.setScale(2.0f, 2.0f);

    optionsButton.sprite.setTexture(optionsButton.texture);
    optionsButton.sprite.setOrigin(optionsButton.sprite.getLocalBounds().width / 2, optionsButton.sprite.getLocalBounds().height / 2);
    optionsButton.sprite.setPosition(1470, 900);
    optionsButton.sprite.setScale(2.0f, 2.0f);

    quitButton.sprite.setTexture(quitButton.texture);
    quitButton.sprite.setPosition(96, 54);

    menuBG[0].sprite.setTexture(menuBG[0].texture);
}
void bleedEffect()
{
    float recoveryTime = 0.3f;
    timeSinceLastHit += deltaTime;

    if (timeSinceLastHit < recoveryTime)
    {
        player.setColor(Color::Red);
    }
    else {
        player.setColor(Color::White);
    }
}
void logoAnimation()
{
    logoAnimationTimer += deltaTime;



    if (logoAnimationTimer >= LOGO_ANIMATION_RATE)
    {
        logoAnimationTimer = 0;
        logoIndx = (logoIndx + 1) % 17;
    }
    // Display the current frame from the vertical sprite sheet.
    logo.setTexture(logoTexture);
    logo.setTextureRect(IntRect(0, logoIndx * 526, 827, 526));

}
void mainMenuButtons() {
    if (playButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        playButton.sprite.setTextureRect(IntRect(0, 108, 222, 108));
        playButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            clickSound.play();
            sleep(milliseconds(200)); // little delay before starting game for smooth transition
            menu = 1;
            backgroundMusic.pause();
            //RESETING
            resetGame();
        }
    }
    else
    {
        playButton.sprite.setTextureRect(IntRect(0, 0, 222, 108));
        playButton.sprite.setColor(Color::White);
    }


    if (shopButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        shopButton.sprite.setTextureRect(IntRect(0, 108, 222, 108));
        shopButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            clickSound.play();
            sleep(milliseconds(200)); // little delay before starting game for smooth transition
            menu = 1;
        }
    }
    else
    {
        shopButton.sprite.setTextureRect(IntRect(0, 0, 222, 108));
        shopButton.sprite.setColor(Color::White);
    }


    if (optionsButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        optionsButton.sprite.setTextureRect(IntRect(0, 108, 222, 108));
        optionsButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            clickSound.play();
            sleep(milliseconds(200)); // little delay before starting game for smooth transition
            menu = 1;
        }
    }
    else
    {
        optionsButton.sprite.setTextureRect(IntRect(0, 0, 222, 108));
        optionsButton.sprite.setColor(Color::White);
    }


    if (quitButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        quitButton.sprite.setTextureRect(IntRect(0, 108, 102, 108));
        quitButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            sleep(milliseconds(200)); // little delay before starting game for smooth transition
            window.close();
        }
    }
    else
    {
        quitButton.sprite.setTextureRect(IntRect(0, 0, 102, 108));
        quitButton.sprite.setColor(Color::White);
    }
}
Vector2f getDirectionToNearestZombie() {
    if (Zombies.empty()) {
        return Vector2f(0, 0);  // Return zero vector if no zombies
    }

    float minDistance = FLT_MAX;
    Vector2f nearestZombiePos;

    for (int i = 0; i < Zombies.size(); i++) {
        float distance = sqrt(pow(Zombies[i].Shape.getPosition().x - player.getPosition().x, 2) +
            pow(Zombies[i].Shape.getPosition().y - player.getPosition().y, 2));
        if (distance < minDistance) {
            minDistance = distance;
            nearestZombiePos = Zombies[i].Shape.getPosition();
        }
    }

    // Calculate direction to nearest zombie
    Vector2f direction = nearestZombiePos - player.getPosition();
    return Normalize(direction);  // Use the existing Normalize function
}
void createProjectile()
{
    if (projectileTimer >= PROJECTILE_COOLDOWN && !projectile.active && !Zombies.empty())
    {
        projectileTimer = 0;  // Reset timer
        projectile.sprite.setTexture(projectileTexture);
        projectile.sprite.setOrigin(projectile.sprite.getLocalBounds().width / 2, projectile.sprite.getLocalBounds().height / 2);
        projectile.sprite.setPosition(player.getPosition());
        projectile.active = true;

        // Get direction to nearest zombie
        projectile.direction = getDirectionToNearestZombie();
        projectile.speed = PROJECTILE_SPEED;
    }
}
void updateProjectile()
{
    projectileTimer += deltaTime;

    if (projectile.active)
    {
        // Move projectile
        projectile.sprite.move(projectile.direction * projectile.speed * deltaTime);

        // Check collision with all zombies
        for (auto& zombie : Zombies) {
            if (projectile.sprite.getGlobalBounds().intersects(zombie.Shape.getGlobalBounds()))
            {
                projectile.active = false;
                zombie.HP -= ProjectileDamage; //Damage logic for zombie
                break;  // Exit loop once we hit a zombie
            }
        }

        // Check if projectile is off screen
        Vector2f pos = projectile.sprite.getPosition();
        if (pos.x < 0 || pos.x > background.getGlobalBounds().width ||
            pos.y < 0 || pos.y > background.getGlobalBounds().height || projectileTimer >= projectileDespawnTime)
        {
            projectile.active = false;
        }
    }
}
Enemies selectSpwanZombie() {
    Enemies newZombie;
    int diffrence = 100;
    int newKilled = zombiesKilled % (diffrence * numOfZombieTypes);

    if (newKilled <= diffrence * 1) {
        newZombie = ZombieTypes[0];
    }
    else if (newKilled <= diffrence * 2) {
        newZombie = ZombieTypes[1];
    }
    else if (newKilled <= diffrence * 3) {
        newZombie = ZombieTypes[2];
    }
    else if (newKilled <= diffrence * 4) {
        newZombie = ZombieTypes[3];
    }
    else if (newKilled <= diffrence * 5) {
        newZombie = ZombieTypes[4];
    }
    else if (newKilled <= diffrence * 6) {
        newZombie = ZombieTypes[5];
    }
    else if (newKilled <= diffrence * 7) {
        newZombie = ZombieTypes[6];
    }
    else if (newKilled <= diffrence * 8) {
        newZombie = ZombieTypes[7];
    }
    else if (newKilled <= diffrence * 9) {
        newZombie = ZombieTypes[8];
    }
    else {
        newZombie = ZombieTypes[9];
    }

    return newZombie;
}
void SpwaningZombies() {

    SpawnTimer += deltaTime;
    if (SpawnTimer >= SpawnDelay) {
        SpawnTimer = 0;
        Enemies newZombie = selectSpwanZombie();
        // Random X and Y within your map bounds
        //generate num between 0-1 for x and y
        float x = static_cast<float>(rand()) / RAND_MAX;
        float y = static_cast<float>(rand()) / RAND_MAX;
        //multiply by map size
        x *= 7680;
        y *= 4320;

        newZombie.Shape.setPosition(Vector2f(x, y));
        newZombie.Start();
        Zombies.push_back(newZombie);
        playEnemyAppearSound();
    }
}
Vector2f Normalize(Vector2f vector) {
    float mag = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
    Vector2f res = Vector2f(vector.x / mag, vector.y / mag);
    return res;
}
void ZombieHandler() {
    for (int i = 0; i < Zombies.size(); i++) {
        if (Zombies[i].isDead) {
            createXPOrb(Zombies[i].Shape.getPosition());
            int coinChance = rand() % 3;
            coins += coinChance;
            Zombies.erase(Zombies.begin() + i);
            zombiesKilled++;
            continue;
        }

        // Update zombie's hit cooldown for whip
        if (!Zombies[i].canBeHit) {
            Zombies[i].lastHitTime += deltaTime;
            if (Zombies[i].lastHitTime >= WHIP_COOLDOWN) {
                Zombies[i].canBeHit = true;
            }
        }

        Zombies[i].Update();
    }
}
void separateZombies() {
    float minDistance = 42.0f;
    for (int i = 0; i < Zombies.size(); ++i) {
        for (int j = i + 1; j < Zombies.size(); ++j) {
            Vector2f pos1 = Zombies[i].HitBox.getPosition();
            Vector2f pos2 = Zombies[j].HitBox.getPosition();

            Vector2f diff = pos1 - pos2;
            float dist = sqrt(diff.x * diff.x + diff.y * diff.y);

            if (dist < minDistance && dist > 0.0f) {
                Vector2f offset = diff / dist * (minDistance - dist) / 2.0f;
                Zombies[i].Shape.move(offset);
                Zombies[j].Shape.move(-offset);
            }
        }
    }
}
void pauseButtonHandler() {

    if (pauseButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        pauseButton.sprite.setTextureRect(IntRect(0, 108, 102, 108));
        pauseButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left) && isPaused == 0) {
            logo.setPosition(1920 / 2.8, 1080 / 2);
            logo.setScale(0.5, 0.5);
            isPaused = 1;

            pauseStart = timerClock.getElapsedTime();


        }
    }
    else
    {
        pauseButton.sprite.setTextureRect(IntRect(0, 0, 102, 108));
        pauseButton.sprite.setColor(Color::White);
    }

    if (Keyboard::isKeyPressed(Keyboard::Escape) && isPaused == 0) {
        logo.setPosition(1920 / 2.8, 1080 / 2);
        logo.setScale(0.5, 0.5);
        isPaused = 1;

        pauseStart = timerClock.getElapsedTime();

    }
}
void pauseMenu() {
    if (backButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        backButton.sprite.setTextureRect(IntRect(0, 108, 330, 108));
        backButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            isPaused = 0;

            Time pausedDuration = timerClock.getElapsedTime() - pauseStart;
            totalPausedTime += pausedDuration;
        }
    }
    else
    {
        backButton.sprite.setTextureRect(IntRect(0, 0, 330, 108));
        backButton.sprite.setColor(Color::White);
    }
}
void backtomenuButtonHandler() {
    if (backtomenuButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        backtomenuButton.sprite.setTextureRect(IntRect(0, 108, 330, 108));
        backtomenuButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            menu = 0;
            backgroundMusic.play();
            logo.setPosition(1920 / 4, 1080 / 2);
            logo.setScale(1, 1);
        }
    }
    else
    {
        backtomenuButton.sprite.setTextureRect(IntRect(0, 0, 330, 108));
        backtomenuButton.sprite.setColor(Color::White);
    }
}
void zombieInitalization() {
    //Enemy 1/Normal Zombie

    zombieTextureSheets[0].loadFromFile("assets/Zombie1.png");
    ZombieTypes[0].type = 0;
    ZombieTypes[0].hitboxWidth = 35;
    ZombieTypes[0].hitboxHeight = 48;
    ZombieTypes[0].spriteWidth = 35;
    ZombieTypes[0].spriteHeight = 48;
    ZombieTypes[0].DMG = 2;
    ZombieTypes[0].HP = 10;
    ZombieTypes[0].speed = 160;
    ZombieTypes[0].attackRate = 0.25;
    ZombieTypes[0].colSize = 3;
    ZombieTypes[0].AnimtaionRate = 0.135f;

    //Enemy 2/Ice Zombie

    zombieTextureSheets[1].loadFromFile("assets/Zombie2.png");
    ZombieTypes[1].type = 1;
    ZombieTypes[1].hitboxWidth = 35;
    ZombieTypes[1].hitboxHeight = 48;
    ZombieTypes[1].spriteWidth = 35;
    ZombieTypes[1].spriteHeight = 48;
    ZombieTypes[1].DMG = 5;
    ZombieTypes[1].HP = 30;
    ZombieTypes[1].speed = 140;
    ZombieTypes[1].attackRate = 0.25;
    ZombieTypes[1].colSize = 3;
    ZombieTypes[1].AnimtaionRate = 0.135f;

    //Enemy 3

    zombieTextureSheets[2].loadFromFile("assets/Zombie3.png");
    ZombieTypes[2].type = 2;
    ZombieTypes[2].hitboxWidth = 35;
    ZombieTypes[2].hitboxHeight = 48;
    ZombieTypes[2].spriteWidth = 35;
    ZombieTypes[2].spriteHeight = 48;
    ZombieTypes[2].DMG = 3;
    ZombieTypes[2].HP = 10;
    ZombieTypes[2].speed = 170;
    ZombieTypes[2].attackRate = 0.25;
    ZombieTypes[2].colSize = 3;
    ZombieTypes[2].AnimtaionRate = 0.135f;

    //Enemy 4

    zombieTextureSheets[3].loadFromFile("assets/Zombie4.png");
    ZombieTypes[3].type = 3;
    ZombieTypes[3].hitboxWidth = 35;
    ZombieTypes[3].hitboxHeight = 48;
    ZombieTypes[3].spriteWidth = 35;
    ZombieTypes[3].spriteHeight = 48;
    ZombieTypes[3].DMG = 10;
    ZombieTypes[3].HP = 30;
    ZombieTypes[3].speed = 160;
    ZombieTypes[3].attackRate = 0.25;
    ZombieTypes[3].colSize = 3;
    ZombieTypes[3].AnimtaionRate = 0.135f;

    //Enemy 5

    zombieTextureSheets[4].loadFromFile("assets/Zombie5.png");
    ZombieTypes[4].type = 4;
    ZombieTypes[4].hitboxWidth = 35;
    ZombieTypes[4].hitboxHeight = 48;
    ZombieTypes[4].spriteWidth = 35;
    ZombieTypes[4].spriteHeight = 48;
    ZombieTypes[4].DMG = 1;
    ZombieTypes[4].HP = 50;
    ZombieTypes[4].speed = 210;
    ZombieTypes[4].attackRate = 0.25;
    ZombieTypes[4].colSize = 3;
    ZombieTypes[4].AnimtaionRate = 0.135f;

    //Enemy 6

    zombieTextureSheets[5].loadFromFile("assets/Zombie6.png");
    ZombieTypes[5].type = 5;
    ZombieTypes[5].hitboxWidth = 35;
    ZombieTypes[5].hitboxHeight = 43;
    ZombieTypes[5].spriteWidth = 35;
    ZombieTypes[5].spriteHeight = 43;
    ZombieTypes[5].DMG = 10;
    ZombieTypes[5].HP = 20;
    ZombieTypes[5].speed = 150;
    ZombieTypes[5].attackRate = 0.25;
    ZombieTypes[5].colSize = 3;
    ZombieTypes[5].AnimtaionRate = 0.135f;

    //Enemy 7

    zombieTextureSheets[6].loadFromFile("assets/Zombie7.png");
    ZombieTypes[6].type = 6;
    ZombieTypes[6].hitboxWidth = 35;
    ZombieTypes[6].hitboxHeight = 48;
    ZombieTypes[6].spriteWidth = 35;
    ZombieTypes[6].spriteHeight = 48;
    ZombieTypes[6].DMG = 5;
    ZombieTypes[6].HP = 70;
    ZombieTypes[6].speed = 150;
    ZombieTypes[6].attackRate = 0.25;
    ZombieTypes[6].colSize = 3;
    ZombieTypes[6].AnimtaionRate = 0.135f;

    //Enemy 8

    zombieTextureSheets[7].loadFromFile("assets/Zombie8.png");
    ZombieTypes[7].type = 7;
    ZombieTypes[7].hitboxWidth = 35;
    ZombieTypes[7].hitboxHeight = 41;
    ZombieTypes[7].spriteWidth = 35;
    ZombieTypes[7].spriteHeight = 41;
    ZombieTypes[7].DMG = 15;
    ZombieTypes[7].HP = 1;
    ZombieTypes[7].speed = 230;
    ZombieTypes[7].attackRate = 0.25;
    ZombieTypes[7].colSize = 3;
    ZombieTypes[7].AnimtaionRate = 0.135f;

    //Enemy 9

    zombieTextureSheets[8].loadFromFile("assets/Zombie9.png");
    ZombieTypes[8].type = 8;
    ZombieTypes[8].hitboxWidth = 35;
    ZombieTypes[8].hitboxHeight = 48;
    ZombieTypes[8].spriteWidth = 35;
    ZombieTypes[8].spriteHeight = 48;
    ZombieTypes[8].DMG = 5;
    ZombieTypes[8].HP = 30;
    ZombieTypes[8].speed = 150;
    ZombieTypes[8].attackRate = 0.25;
    ZombieTypes[8].colSize = 3;
    ZombieTypes[8].AnimtaionRate = 0.135f;

    //Enemy 10

    zombieTextureSheets[9].loadFromFile("assets/Zombie10.png");
    ZombieTypes[9].type = 9;
    ZombieTypes[9].hitboxWidth = 35;
    ZombieTypes[9].hitboxHeight = 48;
    ZombieTypes[9].spriteWidth = 35;
    ZombieTypes[9].spriteHeight = 48;
    ZombieTypes[9].DMG = 20;
    ZombieTypes[9].HP = 100;
    ZombieTypes[9].speed = 210;
    ZombieTypes[9].attackRate = 0.25;
    ZombieTypes[9].colSize = 3;
    ZombieTypes[9].AnimtaionRate = 0.135f;

}
string scoreFormatHandler(int score) {
    string formatedScore = "";

    if (score < 10)
        formatedScore = "0000" + to_string(score);
    else if (score < 100)
        formatedScore = "000" + to_string(score);
    else if (score < 1000)
        formatedScore = "00" + to_string(score);
    else if (score < 10000)
        formatedScore = "0" + to_string(score);
    else
        formatedScore = to_string(score);

    return formatedScore;
}
int time() {
    Time timer = timerClock.getElapsedTime();


    return (timer - totalPausedTime).asSeconds(); // subtract paused time from total time

}
string timerFormatHandler(int time) {
    int minutes = time / 60;
    int seconds = time % 60;

    string formattedTime = "";

    if (minutes < 10) {
        formattedTime += "0";
    }

    formattedTime += to_string(minutes);
    formattedTime += ":";

    // Add leading zero to seconds if needed
    if (seconds < 10) {
        formattedTime += "0";
    }

    formattedTime += std::to_string(seconds);

    return formattedTime;
}
void resetGame() {
    currentXP = 0.0f;
    Zombies.clear();
    xpOrbs.clear();
    maxHealth = 100.0f;
    currentHealth = maxHealth;
    projectile.active = false;
    isDead = false;
    isPaused = false;
    zombiesKilled = 0;
    healingUpgradeLevel = 0;
    isUpgrading = false;
    isMoving = false;
    playerPos = true; // true = left  | false  = right
    walkIndx = 0;
    whipIndx = 0;
    whipCooldownTimer = 0;
    canAttack = true;
    timeSinceLastHit = 999;
    playerLevel = 1;
    swings = 0;
    whipLvl = 1;
    upgradeLevel.healingUpgradeLevel = 0;
    isProjectileUnlocked = false;
    timerClock.restart();
    totalPausedTime = Time::Zero;
    whip.setPosition(5000, 5000);
    player.setScale(1, 1);
    backtomenuButton.sprite.setPosition(1920 / 1.65, 1080 / 1.5);
    backtomenuButton.sprite.setScale(1.5, 1.5);

    scoreText.setCharacterSize(100);
    scoreText.setPosition(1660, 30);
    scoreText.setFillColor(Color(205, 145, 43));
    scoreText.setOutlineColor(Color::Black);
    scoreText.setOutlineThickness(2);

    timerText.setCharacterSize(100);
    timerText.setPosition(871, 100);
    timerText.setFillColor(Color(94, 70, 100));
    timerText.setOutlineColor(Color::Black);
    timerText.setOutlineThickness(2);

    coinsText.setCharacterSize(100);
    coinsText.setPosition(1660, 100);
    coinsText.setFillColor(Color(205, 145, 43));
    coinsText.setOutlineColor(Color::Black);
    coinsText.setOutlineThickness(2);

    xpBarFill.setSize(Vector2f(0, 15));

    player.setPosition(background.getGlobalBounds().width / 2, background.getGlobalBounds().height / 2);

}
void whipDmg()
{
    // Only process damage if whip is in attack animation (not at idle frame)
    if (whipIndx != 12)
    {
        for (auto& zombie : Zombies)
        {
            // Check if zombie can be hit and if whip hitbox intersects with zombie
            if (zombie.canBeHit && whipHitbox.getGlobalBounds().intersects(zombie.Shape.getGlobalBounds()))
            {
                // Apply damage
                zombie.Shape.setColor(Color::Red);
                zombie.HP -= whipDamage;
                zombie.canBeHit = false;  // Mark zombie as hit
                zombie.lastHitTime = 0.0f;  // Reset hit timer
                break;  // Exit loop after hitting one zombie
            }

            if (zombie.lastHitTime > 0.15) {
                zombie.Shape.setColor(Color::White);
            }

        }
    }
}
void createXPOrb(Vector2f position)
{
    XPOrb newOrb;
    newOrb.sprite.setTexture(xpOrbTexture);
    newOrb.sprite.setOrigin(newOrb.sprite.getLocalBounds().width / 2, newOrb.sprite.getLocalBounds().height / 2);
    newOrb.sprite.setPosition(position);
    xpOrbs.push_back(newOrb);
}
void updateXPOrbs()
{
    for (int i = 0; i < xpOrbs.size(); i++) {


        if (xpOrbs[i].sprite.getGlobalBounds().intersects(player.getGlobalBounds())) {
            addXp(1.0f);
            xpOrbs.erase(xpOrbs.begin() + i);
        }

    }
}
void gameoverWidgets() {

    for (int i = 0; i < 4; i++) {
        gameoverText[i].setFont(font);
        gameoverText[i].setCharacterSize(100);
        gameoverText[i].setFillColor(Color::Black);
    }

    gameoverText[0].setString("Better luck next time...");
    gameoverText[0].setPosition(1920 / 4 + 50, 1080 / 4 + 100);

    gameoverText[1].setString("Score:");
    gameoverText[1].setPosition(1920 / 5, 1080 / 4 + 200);

    gameoverText[2].setString("Time Survived: ");
    gameoverText[2].setPosition(1920 / 5, 1080 / 4 + 350);

    //gameoverText[3].setString("You can do better.");
    //gameoverText[3].setPosition(1920 / 4 + 130, 1080 / 4 + 550);
}
void gameoverMenuHandler() {

    for (int i = 0; i < 4; i++) {
        window.draw(gameoverText[i]);
    }

    backtomenuButton.sprite.setPosition(1920 / 2, 900);
    scoreText.setPosition(1920 / 1.2 - 270, 1080 / 4 + 200);
    timerText.setPosition(1920 / 1.2 - 270, 1080 / 4 + 350);
    coinsText.setPosition(1920 / 1.2 - 270, 1080 / 4 + 200);

}
string coinFormatHandler(int coins) {
    string formatedCoins = "";

    if (coins < 10)
        formatedCoins = "0000" + to_string(coins);
    else if (coins < 100)
        formatedCoins = "000" + to_string(coins);
    else if (coins < 1000)
        formatedCoins = "00" + to_string(coins);
    else if (coins < 10000)
        formatedCoins = "0" + to_string(coins);
    else
        formatedCoins = to_string(coins);

    return formatedCoins;
}
void drawPowerUpsMenu()
{
    window.draw(menuBG[3].sprite);
    for (int i = 0; i < 3; i++)
    {
        window.draw(allUpgrades[upgradesIndices[i]].sprite);
        window.draw(allUpgrades[upgradesIndices[i]].text);
    }
}
void powerUps()
{
    for (int i = 0; i < UPGRADES_NUM; i++)
    {
        // Check if the mouse is over the upgrade sprite
        if (allUpgrades[i].sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
        {
            // Check if the user clicked the left mouse button
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                switch (i)
                {
                case 0:
                    // Upgrade 0: Increase Max Health
                    maxHealthIncrease(10, 150);
                    break;

                case 1:
                    // Upgrade 1: Defense upgrade

                    break;

                case 2:
                    // Upgrade 2: Upgrade whip
                    whipLvl = 2;
                    break;

                case 3:
                    // Upgrade 3: Health regeneration
                    upgradeLevel.healingUpgradeLevel++;
                    if (upgradeLevel.healingUpgradeLevel > 5)
                    {
                        upgradeLevel.healingUpgradeLevel = 5;
                    }
                    break;

                case 4:
                    // Upgrade 4: Unlock second weapon (projectile)
                    isProjectileUnlocked = true;
                    break;

                case 5:
                    // Upgrade 5: Increase player speed
                    increasePlayerSpeed(50);
                    break;
                }
                // After any upgrade is clicked, we stop the upgrade screen
                isUpgrading = false;

                // Prevent checking more upgrades — only one can be selected per click
                break;
            }
        }
    }
}
void healingUpgrade()
{
    float amount = (upgradeLevel.healingUpgradeLevel * 0.5) + 0.5;
    if (upgradeLevel.healingUpgradeLevel > 0)
    {
        heal(amount * deltaTime);
    }
}
void upgradeItemsHandeling()
{
    //Origin
    for (int i = 0; i < UPGRADES_NUM; i++)
    {
        allUpgrades[i].sprite.setOrigin(allUpgrades[i].sprite.getLocalBounds().width / 2, allUpgrades[i].sprite.getLocalBounds().height / 2);
    }
    srand(time(0));
    if (isMenuOpen)
    {
        for (int i = 0; i < 3; i++)
        {
            upgradesIndices[i] = rand() % 6;
        }
        while (upgradesIndices[0] == upgradesIndices[1])
        {
            upgradesIndices[1] = rand() % 6;
        }
        while (upgradesIndices[0] == upgradesIndices[2] || upgradesIndices[1] == upgradesIndices[2])
        {
            upgradesIndices[2] = rand() % 6;
        }
        for (int i = 0; i < 3; i++)
        {
            allUpgrades[upgradesIndices[i]].sprite.setPosition((1920 / 2) + offSetX[i], 1080 / 2);
        }
        upgradesTextHandeling();
        isMenuOpen = false;
    }
}
void maxHealthIncrease(int addValue, int limit)
{
    if (maxHealth < limit)
    {
        maxHealth += addValue;
    }
}
void increasePlayerSpeed(int value)
{
    playerSpeed += value;
}
void upgradesMenuText()
{
    //coordinates:(left:-720,down:+255),(center:-210,down:+255),(right:+300,down:+255)
    for (int i = 0; i < UPGRADES_NUM; i++)
    {
        allUpgrades[i].font.loadFromFile("assets/Pixel_Game.otf");
        allUpgrades[i].text.setFont(allUpgrades[i].font);
        allUpgrades[i].text.setFillColor(Color::Yellow);
        allUpgrades[i].text.setOutlineColor(Color::Black);
        allUpgrades[i].text.setOutlineThickness(4);
    }
    upgradeItemsName();
}
void upgradeItemsName()
{
    allUpgrades[0].text.setString("Life Crystal");
    allUpgrades[1].text.setString("Shield");
    allUpgrades[2].text.setString("Upgrade whip");
    allUpgrades[3].text.setString("Vital Essence");
    allUpgrades[4].text.setString("Second Weapon");
    allUpgrades[5].text.setString("Falcon Boots");
}
void upgradesTextHandeling()
{
    allUpgrades[0].text.setPosition(allUpgrades[0].sprite.getPosition().x - 200, allUpgrades[0].sprite.getPosition().y + 270);
    allUpgrades[0].text.setCharacterSize(85);

    allUpgrades[1].text.setPosition(allUpgrades[1].sprite.getPosition().x - 100, allUpgrades[1].sprite.getPosition().y + 270);
    allUpgrades[1].text.setCharacterSize(85);

    allUpgrades[2].text.setPosition(allUpgrades[2].sprite.getPosition().x - 210, allUpgrades[2].sprite.getPosition().y + 270);
    allUpgrades[2].text.setCharacterSize(85);

    allUpgrades[3].text.setPosition(allUpgrades[3].sprite.getPosition().x - 210, allUpgrades[3].sprite.getPosition().y + 270);
    allUpgrades[3].text.setCharacterSize(85);

    allUpgrades[4].text.setPosition(allUpgrades[4].sprite.getPosition().x - 210, allUpgrades[4].sprite.getPosition().y + 275);
    allUpgrades[4].text.setCharacterSize(75);

    allUpgrades[5].text.setPosition(allUpgrades[5].sprite.getPosition().x - 210, allUpgrades[5].sprite.getPosition().y + 270);
    allUpgrades[5].text.setCharacterSize(85);
}

void loadSounds()
{
    clickBuffer.loadFromFile("assets/sounds/click.mp3");
    deathBuffer.loadFromFile("assets/sounds/death.mp3");
    gameOverBuffer.loadFromFile("assets/sounds/gameover.mp3");
    footstepsBuffer.loadFromFile("assets/sounds/run.mp3");
    enemyAppearBuffer.loadFromFile("assets/sounds/zombie.mp3");
    enemyHitBuffer.loadFromFile("assets/sounds/enemy_hit.mp3");
    playerHitBuffer.loadFromFile("assets/sounds/player_hit.mp3");
    healthBuffer.loadFromFile("assets/sounds/health_recharge.mp3");
    bossBuffer.loadFromFile("assets/sounds/boss.mp3");
    coinBuffer.loadFromFile("assets/sounds/coins.mp3");
    levelCompleteBuffer.loadFromFile("assets/sounds/victory.mp3");

    clickSound.setBuffer(clickBuffer);
    deathSound.setBuffer(deathBuffer);
    gameOverSound.setBuffer(gameOverBuffer);
    footstepsSound.setBuffer(footstepsBuffer);
    enemyAppearSound.setBuffer(enemyAppearBuffer);
    enemyHitSound.setBuffer(enemyHitBuffer);
    playerHitSound.setBuffer(playerHitBuffer);
    healthSound.setBuffer(healthBuffer);
    bossSound.setBuffer(bossBuffer);
    coinSound.setBuffer(coinBuffer);
    levelCompleteSound.setBuffer(levelCompleteBuffer);

    backgroundMusic.openFromFile("assets/sounds/background.mp3");
}
void playBackgroundMusic()
{
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
}
void playFootstepsSound() {
    if (footstepsSound.getStatus() != Sound::Playing) {
        footstepsSound.play();
    }
}
void playEnemyAppearSound() {
    if (enemyAppearSound.getStatus() != Sound::Playing && (zombiesKilled != 0) && (zombiesKilled % 5 == 0)) {
        enemyAppearSound.play();
    }
}
void playEnemyHitSound() {
    deathSound.play();
}
void playPlayerHitSound() {
    if (playerHitSound.getStatus() != Sound::Playing) {
        playerHitSound.play();
    }
}
