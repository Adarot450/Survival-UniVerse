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
const float LOGO_ANIMATION_RATE = 0.06; // Indicates how fast are we switching logo's texture
const float COIN_ANIMATION_RATE = 0.06; // Indicates how fast are we switching logo's texture
const int WHIP_TRAVEL_DISTANCE = 2000; //How far the whip moves away from the player
const float WHIP_RATE = 0.025; //how fast are we moving through whip texture
const float PROJECTILE_SPEED = 400.0f;  // Adjust speed as needed
const float PROJECTILE_COOLDOWN = 0.7f;  // Cooldown between auto shots
const float EnemyAnimationRate = 0.135f;  //  How fast we are switching Enemy's texture
const int numOfZombieTypes = 10;
const int UPGRADES_NUM = 7;     //Upgrades number

float projectileTimer = 0.0f;
float projectileDespawnTime = 3.0f;

//Global Variables

//Upgrades
int character = 0; // 0 = main character ||
int healingUpgradeLevel = 0;
bool isUpgrading = false;
bool isMenuOpen = false;

int swings = 0;
int shopPage = 0;
bool isDead = false;
bool isPaused = false;
int walkIndx = 0;
int logoIndx = 0;
float logoAnimationTimer = 0;// Logo's texture switch timer (Always set to ZERO)
float coinAnimationTimer = 0;
float whipTimer = 0; //whip's animation timer (always set to ZERO)
int  whipIndx = 11;
int coinIndx = 0;
int menu = 0; // 0 = main menu | 1 = level | 2 = Shop | 3 = gore alert | 4 = options | 5 = leaderboard | 6 = name
float whipCooldownTimer = 0;
float timeSinceLastHit = 999;
float SpawnDelay;
float MinSpawnDelay = 0.025;
float SpawnTimer = 0;
float clickRegisterTimer = 0.1f;
int zombiesKilled;
int volume = 100;
bool controls = 1; // 0:arrows | 1:wasd

int bgIndx = 0;
int coins = 500;
int gameMode = 0; // 0:Normal | 1:Gore

// chest system
bool chestbroken = false;
bool orbcollected = false; //logy
int healthBuff = 40;
bool contentcollected = false;
bool hasmagnet = false;

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
SoundBuffer chestBreakBuffer;
SoundBuffer potionDrinkBuffer;

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
Sound chestBreakSound;
Sound potionDrinkSound;

// Background Music
Music backgroundMusic;
Music normalLevelMusic;
Music goreModeMusic;


// sprite and Textures
Sprite background;
Sprite whip;
Sprite logo;
Sprite xpBarSprite;  // New XP bar sprite
Sprite menuCoin;
Sprite chest; // logy
Sprite potion; // logy

Texture logoTexture;
Texture menuBackgroundTexture;
Texture backgroundTexture;
Texture backgroundHorrorTexture;
Texture whipSheet;
Texture xpBarTexture;  // New XP bar texture
Texture menuCoinTexture;
Texture chestTexture; // logy
Texture potionTexture; // logy
Texture coinsTexture;
Texture magnetTexture;

//texts
Text scoreText;
Text timerText;
Text gameoverText[4];
Text coinsText;
Text playerSpeedStats;
Text playerHealthStats;
Text whipDmgStats;
Text ringBaseDmgStats;
Text whipCooldownStats;
Text projectileDmgStats;
Text projectileCooldownStats;
Text playerStartingWeapon;
Text currentName;
Text leaderboardNames[10];
Text leaderboardScores[10];
Text leaderboardTimes[10];


//Shapes
RectangleShape whipHitbox(Vector2f(450, 45));
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
void gameoverMenu();                        // Aly
void gameoverMenuHandler();                 // Aly
void shopWidgets();                         // Aly
void nextButtonHandler();                   // Aly
void backButtonHandler();                   // Aly
void buyButtonHandler();                    // Aly
void selectButtonHandler();                 // Aly
void goreMenuInitialization();              // Aly
void goreMenuHandler();                     // Aly
void backgroundInitialization();            // Aly
void optionsWidgets();                      // Aly
void optionsHandler();                      // Aly
void leaderboardWidgets();                  // Aly
void leaderboardHandler();                  // Aly
void nameWidgets();                         // Aly
void nameHandler(Event event);              // Aly
void notValidUpgradeRemover();              // Aly
void whipAnimation();                       // Aly & Amr
void saveGameData();                        // Amr
void loadGameData();                        // Amr
void charachterInitalization();             // Amr
void BorderCollision();                     // Amr
void lockViewToBackground();                // Amr
void whipCollider();                        // Amr
void whipHitboxHandeling();                 // Amr
void whipDmg();                             // Amr
string coinFormatHandler(int coins);        // Amr
void fontStatsInitalizer();                 // Amr
void DefaultPlayerStats();                  // Amr
void playerOneStats();                      // Amr
void playerTwoStats();                      // Amr
void playerThreeStats();                    // Amr
void playerFourStats();                     // Amr
void playerFiveStats();                     // Amr
void playerSixStats();                      // Amr
void playerSevenStats();                    // Amr
void ZombieHandler();                       // Adam & Yassin & Amr
void SpwaningZombies();                     // Adam
void separateZombies();                     // Adam
Vector2f Normalize(Vector2f vector);        // Adam
void zombieInitalization();                 // Adam
void setSpawnRate();                        // Adam
void bleedEffect();                         // Yassin & Adam
void loadTextures();                        // Yassin
void healthBarHandling();                   // Yassin
void addXp(float xpToAdd);                  // Yassin
void takeDamage(float damage);              // Yassin
void heal(float amount);                    // Yassin
void createProjectile();                    // Yassin
void updateProjectile();                    // Yassin
void createXPOrb(Vector2f position, float xpValue);//Yassin
void updateXPOrbs();                        // Yassin 
Vector2f getDirectionToNearestZombie();     // Yassin
void updateRing();                          //Yassin
void ringInitialization();                  //Yassin
void ringAnimation(float deltaTime);        //Yassin
void ringRotation(float deltaTime, const Vector2f& playerPos);//Yassin
void ringCollision();                       //Yassin
void powerUps();                            // Marwan
void healingUpgrade();                      // Marwan
void drawPowerUpsMenu();                    // Marwan
void upgradeItemsHandeling();               // Marwan
void maxHealthIncrease(int value);          // Marwan
void increasePlayerSpeed(int value);        // Marwan
void upgradesMenuText();                    // Marwan
void upgradeItemsName();                    // Marwan
void upgradesTextHandeling();               // Marwan
void maxLevelChecker();                     // Marwan
void upgradeLevelText();                    // Marwan
void upgradeDescriptionText();              // Marwan    
void upgradeDescriptionNames();             // Marwan
void upgradeDescriptionHandeling();         // Marwan
void loadSounds();                          // Maritsia
void playBackgroundMusic();                 // Maritsia
void playNormalLevelMusic();                // Maritsia
void playGoreLevelMusic();                  // Maritsia
void playEnemyAppearSound();                // Maritsia
void playFootstepsSound();                  // Maritsia
void playEnemyHitSound();                   // Maritsia
void playPlayerHitSound();                  // Maritsia
void playChestBreakSound();                 // Maritsia
void playPotionDrinkSound();                // Maritsia
void playCoinSound();                       // Maritsia
void loadChestAndOrb();                     // Logy
void updateChestLogic();                    // Logy
void updateposition1();                     // Logy
void updateposition2();                     // Logy
void updateposition3();                     // Logy
void orbdirection();                        // Logy
void coinAnimation();                       // Logy
void leaderboardSort();

//structs

enum potiontype
{
    health,
    coin,
    magnet
};

struct chests
{
    bool chestbroken = false;
    bool contentcollected = false;
    potiontype type;
    Sprite sprite;
    Sprite itemsprite;
    Texture texture;


}chest1, chest2, chest3;

struct RingWeapon {
    Sprite sprite;
    float radius = 150.0f;
    float angle = 0.0f;
    float rotationSpeed = 300.0f;
    float animationTimer = 0.0f;
    int frameIndex = 0;
    bool isActive = false;
};

RingWeapon ring;
Texture ringTexture;

struct UpgradesItem
{
    Sprite sprite;
    Texture texture;
    Font font;
    Text text;
    int maxHealthLevel = 0;
    int defenseUpgradeLevel = 0;
    int whipLvl = 1;
    int whipLvlReal = 0;
    int healingUpgradeLevel = 0;
    int secondWeaponUpgradeLvl = 0;
    int increasePlayerSpeedLevel = 0;
    int ringUpgradeLevel = 0;
    int upgradeMaxLevel = 5;
};
UpgradesItem upgradeLevel;
UpgradesItem allUpgrades[UPGRADES_NUM];
int offSetX[3] = { -510,0,510 };
int upgradesIndices[3];
int validUpgrades[UPGRADES_NUM];
int currentUpgradelvl[UPGRADES_NUM];

Text currentLevelText[UPGRADES_NUM];

Text descriptionText[UPGRADES_NUM];

struct Background {
    Sprite sprite;
    Texture texture;
    Texture horrorTexture;
};
Background menuBG[9]; // 0:Main menu || 1:Pause Menu || 2:Gameover || 3:Upgrading || 4:Shop || 5:gore alert || 6:Options || 7: Leaderboard || 8:Name

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
    Texture horrorTexture;
    Texture secondForm; // like off and on in volume ;)
    Texture horrorSecondForm;
};

struct Score {
    string name = "--------------------";
    int time = 0;
    int score = 0;
}highscores[10], current;

//menu
Button playButton;
Button shopButton;
Button optionsButton;
Button quitButton;
Button modeChangeButton;
Button yesButton;
Button noButton;
Button sliderButton;
Button volumeButton;
Button leaderboardButton;
Button doneButton;

Sprite slider;
Texture sliderTexture;
Sprite wasd;
Texture wasdTexture;
Texture wasdHorrorTexture;
Sprite arrows;
Texture arrowsTexture;
Texture arrowsHorrorTexture;


//shop
Button backButton;
Button nextButton;
Button selectButton;
Button buyButton;

//ingame
Button pauseButton;
Button backtogameButton;
Button backtomenuButton;


// Add after other structs
struct XPOrb {
    Sprite sprite;
    float xpValue;
};

// Add after other global variables
vector<XPOrb> xpOrbs;
Texture xpOrbTexture;

Texture zombieTextureSheets[numOfZombieTypes];

struct Character {
    const float ANIMATION_RATE = 0.05; // Indicates how fast are we switching player's texture
    int speed = 200;
    bool position = true; // true = left  | false  = right
    float animationTimer = 0;// Player's texture switch timer (Always set to ZERO)
    int level = 1;
    bool isMoving = false;
    bool canAttack = true;
    float whipDamage = 10.0;
    float ringDamage = 5.0;
    float whipCooldown = 1.0f;
    float ProjectileDamage = 10.0;
    float projectileCooldown = 2.0f;
    float playerWidth = 40;
    float playerHeight = 56;
    float hitboxWidth = 20;
    float hitboxHeight = 40;
    int playerFrameIndx = 16;
    bool isProjectileUnlocked = false;
    bool isWhipUnlocked = true;
    bool isUnlocked = false;
    bool isRingUnlocked = false;

    // Health system
    float maxHealth = 100.0f;
    float currentHealth = maxHealth;

    // XP system
    float maxXP = 10.0f;
    float currentXP = 0.0f;

    Texture texture;
    Sprite sprite;

    RectangleShape hitbox;

    void Start() {
        sprite.setTexture(texture);
        playerHitboxHandeling();
    }
    void Update() {
        playerMovement();
        playerAnimation();
        playeCollider();
    }

    void playerMovement()
    {
        isMoving = false;
        if (controls == 0) {
            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                sprite.move(0, -speed * deltaTime);
                isMoving = true;
                //playFootstepsSound();
            }
            if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                sprite.move(0, speed * deltaTime);
                isMoving = true;
                //playFootstepsSound();
            }
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                sprite.move(-speed * deltaTime, 0);
                isMoving = true;
                sprite.setScale(1, 1);
                position = true; //Player is facing Left
                //playFootstepsSound();
            }
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                sprite.move(speed * deltaTime, 0);
                isMoving = true;
                sprite.setScale(-1, 1);
                position = false; //Player is facing right
                //playFootstepsSound();

            }
        } if (controls == 1) {
            if (Keyboard::isKeyPressed(Keyboard::W))
            {
                sprite.move(0, -speed * deltaTime);
                isMoving = true;
                //playFootstepsSound();
            }
            if (Keyboard::isKeyPressed(Keyboard::S))
            {
                sprite.move(0, speed * deltaTime);
                isMoving = true;
                //playFootstepsSound();
            }
            if (Keyboard::isKeyPressed(Keyboard::A))
            {
                sprite.move(-speed * deltaTime, 0);
                isMoving = true;
                sprite.setScale(1, 1);
                position = true; //Player is facing Left
                //playFootstepsSound();
            }
            if (Keyboard::isKeyPressed(Keyboard::D))
            {
                sprite.move(speed * deltaTime, 0);
                isMoving = true;
                sprite.setScale(-1, 1);
                position = false; //Player is facing right
                //playFootstepsSound();

            }
        }

        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

    }
    void playerAnimation()
    {
        animationTimer += deltaTime;

        if (isMoving)
        {
            if (animationTimer >= ANIMATION_RATE)
            {
                animationTimer = 0;
                walkIndx = (walkIndx + 1) % playerFrameIndx;
            }
            // Display the current frame from the vertical sprite sheet.
            sprite.setTextureRect(IntRect(0, walkIndx * playerHeight, playerWidth, playerHeight));
        }
        else
        {
            // When idle, use a fixed frame (for example, the first frame)
            sprite.setTextureRect(IntRect(0, 0, playerWidth, playerHeight));
        }
    }
    void playeCollider()
    {

        hitbox.setPosition(sprite.getPosition().x - 10, sprite.getPosition().y - 20);

    }
    void playerHitboxHandeling()
    {
        hitbox.setSize(Vector2f(hitboxWidth, hitboxHeight));
        hitbox.setFillColor(Color::Transparent);
        //hitbox.setOutlineColor(Color::Red);
        //hitbox.setOutlineThickness(2);
    }
    void playerShopAnimation()
    {
        animationTimer += deltaTime;
        if (animationTimer >= ANIMATION_RATE)
        {
            animationTimer = 0;
            walkIndx = (walkIndx + 1) % playerFrameIndx;
        }
        // Display the current frame from the vertical sprite sheet.
        sprite.setTextureRect(IntRect(0, walkIndx * playerHeight, playerWidth, playerHeight));
        sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);

    }
}player[8];

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
        velocity = player[character].sprite.getPosition() - Shape.getPosition(); //get line from player to zombie
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


            AnimationIndex = (AnimationIndex + 1) % colSize;
            Shape.setTextureRect(IntRect(AnimationIndex * spriteWidth, 0, spriteWidth, spriteHeight));

        }
        //turn left and right
        if (Shape.getPosition().x > player[character].sprite.getPosition().x) {
            Shape.setScale(1, 1);
        }
        else {
            Shape.setScale(-1, 1);
        }
    }
    void Attack() {
        if (HitBox.getGlobalBounds().intersects(player[character].hitbox.getGlobalBounds())) {
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

Enemies selectSpwanZombie();  //Adam

//vectors
vector<Enemies>Zombies;

int main()
{
    Start();

    Clock clock;

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        Event event;
        while (window.pollEvent(event))
        {
            nameHandler(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        Update();
        Draw();
    }

    saveGameData();
    return 0;
}
void Start()
{
    loadGameData();
    //cout << volume << endl;

    srand(time(0));

    window.setFramerateLimit(144);
    player[character].Start();
    font.loadFromFile("assets/Pixel_Game.otf");
    scoreText.setFont(font);
    timerText.setFont(font);
    coinsText.setFont(font);
    currentName.setFont(font);
    fontStatsInitalizer();

    loadTextures();
    mainmenuWidgets();
    shopWidgets();
    optionsWidgets();
    whipHitboxHandeling();

    loadSounds();

    playBackgroundMusic();
    //initialize the ring
    ringInitialization();
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
    whip.setOrigin(whip.getLocalBounds().width / 2, player[character].sprite.getLocalBounds().height / 2);
    whipHitbox.setOrigin(whipHitbox.getLocalBounds().width / 2, whipHitbox.getLocalBounds().height / 2);

    //position setting for xp bars
    xpBarSprite.setPosition((1920 / 50) + 310, 1080 / 100);
    xpBarFill.setPosition((1920 / 50) + 350, (1080 / 50) + 68);

    //position setting for pause button
    pauseButton.sprite.setPosition(96, 54);

    //pause manu
    backtogameButton.sprite.setOrigin(backtogameButton.sprite.getLocalBounds().width / 2, backtogameButton.sprite.getLocalBounds().height / 2);
    backtogameButton.sprite.setPosition(1920 / 1.65, 1080 / 2);
    backtogameButton.sprite.setScale(1.5, 1.5);

    //gameover menu
    backtomenuButton.sprite.setOrigin(backtomenuButton.sprite.getLocalBounds().width / 2, backtomenuButton.sprite.getLocalBounds().height / 2);


    //xpbar
    xpBarFill.setFillColor(Color(0, 255, 255));

    // Upgrades

    allUpgrades[0].sprite.setScale(8, 8);
    allUpgrades[1].sprite.setScale(8, 8);
    allUpgrades[2].sprite.setScale(8, 8);
    allUpgrades[3].sprite.setScale(8, 8);
    allUpgrades[4].sprite.setScale(8, 8);
    allUpgrades[5].sprite.setScale(8, 8);
    allUpgrades[6].sprite.setScale(8, 8);

    upgradesMenuText();

    for (int i = 0; i < UPGRADES_NUM; i++)
    {
        currentLevelText[i].setFont(font);
        currentLevelText[i].setCharacterSize(70);
        currentLevelText[i].setFillColor(Color::White);
        currentLevelText[i].setOutlineColor(Color::Black);
        currentLevelText[i].setOutlineThickness(3);
    }

    upgradeDescriptionText();

    //Enemies
    zombieInitalization();

    // Characters
    charachterInitalization();

    loadChestAndOrb();
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
            backtomenuButton.sprite.setPosition(1920 / 2, 900);
            scoreText.setPosition(1920 / 1.2 - 270, 1080 / 4 + 200);
            timerText.setPosition(1920 / 1.2 - 270, 1080 / 4 + 350);
            backtomenuButtonHandler();
            return;
        }

        if (isUpgrading) {
            powerUps();
            upgradeItemsHandeling();
            notValidUpgradeRemover();
            return;
        }

        if (isPaused) {
            pauseMenu();
            logoAnimation();
            backtomenuButtonHandler();
            return;
        }

        /* if (ring.isActive) {
             window.draw(ring.sprite);
         }*/
        updateRing();

        pauseButtonHandler();

        scoreText.setString(scoreFormatHandler(zombiesKilled));
        timerText.setString(timerFormatHandler(time()));
        coinsText.setString(coinFormatHandler((coins)));

        BorderCollision();
        lockViewToBackground();
        player[character].Update();
        bleedEffect();

        // Handle projectile
        if (player[character].isProjectileUnlocked)
        {
            createProjectile();
            updateProjectile();
        }

        if (player[character].isWhipUnlocked) {
            whipAnimation();
            whipCollider();
            whipDmg();
        }


        //Enemy
        setSpawnRate();
        ZombieHandler();
        SpwaningZombies();
        separateZombies();

        // run upgrades
        healingUpgrade();

        //orbs
        updateXPOrbs();
        updateChestLogic();
        coinAnimation();
    }
    else if (menu == 2) { // Shop
        backtomenuButtonHandler();
        backButtonHandler();
        nextButtonHandler();
        buyButtonHandler();
        selectButtonHandler();
        player[shopPage].playerShopAnimation();
        coinsText.setString(coinFormatHandler((coins)));
    }
    else if (menu == 3) { //gore menu
        goreMenuInitialization();
        goreMenuHandler();
    }
    else if (menu == 4) {
        backtomenuButtonHandler();
        optionsHandler();
    }
    else if (menu == 5) {
        backtomenuButtonHandler();
        leaderboardHandler();
    }
    else if (menu == 6) {
        player[character].playerShopAnimation();
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
        window.draw(coinsText);
        window.draw(menuCoin);
        window.draw(modeChangeButton.sprite);
        window.draw(leaderboardButton.sprite);
    }
    else if (menu == 1) { // game
        // Draw game world (with game view)
        window.setView(view);
        window.draw(background);
        window.draw(player[character].sprite);
        window.draw(player[character].hitbox);

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
        if (player[character].isRingUnlocked) {
            window.draw(ring.sprite);
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

        if (!chest1.chestbroken) {
            window.draw(chest1.sprite);
        }
        else if (!chest1.contentcollected) { //logy
            window.draw(chest1.itemsprite);

        }
        if (!chest2.chestbroken)
        {
            window.draw(chest2.sprite);
        }
        else if (!chest2.contentcollected)
        {
            window.draw(chest2.itemsprite);
        }
        if (!chest3.chestbroken)
        {
            window.draw(chest3.sprite);
        }
        else if (!chest3.contentcollected)
        {
            window.draw(chest3.itemsprite);
        }



        // Draw UI elements (with fixed view)
        View fixedView(FloatRect(0, 0, 1920, 1080)); // yassin
        window.setView(fixedView); // yassin

        // Draw XP bar
        window.draw(xpBarFill);
        window.draw(xpBarSprite);
        window.draw(scoreText);
        window.draw(timerText);

        // menus

        if (isDead) {
            window.draw(menuBG[2].sprite);
            window.draw(backtomenuButton.sprite);
            window.draw(scoreText);
            window.draw(timerText);
            gameoverMenuHandler();
        }
        else {
            window.draw(coinsText);
            window.draw(menuCoin);
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
            window.draw(backtogameButton.sprite);
            window.draw(backtomenuButton.sprite);
            window.draw(logo);
        }
    }
    else if (menu == 2) { // Shop
        window.setView(window.getDefaultView());
        window.draw(menuBG[4].sprite);
        window.draw(backtomenuButton.sprite);
        window.draw(coinsText);
        window.draw(menuCoin);
        DefaultPlayerStats();
        playerOneStats();
        playerTwoStats();
        playerThreeStats();
        playerFourStats();
        playerFiveStats();
        playerSixStats();
        playerSevenStats();

        if (shopPage != 0) {
            window.draw(backButton.sprite);
        }
        if (shopPage != 7) {
            window.draw(nextButton.sprite);
        }
        if (player[shopPage].isUnlocked && character != shopPage) {
            window.draw(selectButton.sprite);
        }
        if (!player[shopPage].isUnlocked) {

            window.draw(buyButton.sprite);
        }
        window.draw(player[shopPage].sprite);

    }

    else if (menu == 3) { // gore alert
        window.setView(window.getDefaultView());
        window.draw(menuBG[5].sprite);
        window.draw(yesButton.sprite);
        window.draw(noButton.sprite);

    }
    else if (menu == 4) {
        window.setView(window.getDefaultView());
        window.draw(menuBG[6].sprite);
        window.draw(backtomenuButton.sprite);
        window.draw(volumeButton.sprite);
        window.draw(slider);
        window.draw(sliderButton.sprite);
        window.draw(selectButton.sprite);
        window.draw(wasd);
        window.draw(arrows);
    }
    else if (menu == 5) {
        window.setView(window.getDefaultView());
        window.draw(menuBG[7].sprite);
        window.draw(backtomenuButton.sprite);

        for (int i = 0; i < 10; i++) {

            window.draw(leaderboardNames[i]);
            window.draw(leaderboardScores[i]);
            window.draw(leaderboardTimes[i]);

        }
    }
    else if (menu == 6) {
        window.setView(window.getDefaultView());
        window.draw(menuBG[8].sprite);
        window.draw(doneButton.sprite);
        window.draw(currentName);
        window.draw(player[character].sprite);
    }
    window.display();
}

void loadTextures()
{
    backgroundTexture.loadFromFile("assets/Background.png");
    backgroundHorrorTexture.loadFromFile("assets/Background_horror.png");
    backgroundInitialization();

    player[0].texture.loadFromFile("assets/player_sheet.png");
    whipSheet.loadFromFile("assets/whipsheet.png");
    whip.setTexture(whipSheet);
    logoTexture.loadFromFile("assets/Logo.png");
    playButton.texture.loadFromFile("assets/play_button.png");
    playButton.horrorTexture.loadFromFile("assets/play_button_horror.png");
    playButton.sprite.setTexture(playButton.texture);
    shopButton.texture.loadFromFile("assets/shop_button.png");
    shopButton.horrorTexture.loadFromFile("assets/shop_button_horror.png");
    optionsButton.texture.loadFromFile("assets/options_button.png");
    optionsButton.horrorTexture.loadFromFile("assets/options_button_horror.png");
    quitButton.texture.loadFromFile("assets/quit_button.png");
    quitButton.horrorTexture.loadFromFile("assets/quit_button_horror.png");
    backButton.texture.loadFromFile("assets/back_button.png");
    backButton.horrorTexture.loadFromFile("assets/back_button_horror.png");
    nextButton.texture.loadFromFile("assets/next_button.png");
    nextButton.horrorTexture.loadFromFile("assets/next_button_horror.png");
    buyButton.texture.loadFromFile("assets/buy_button.png");
    buyButton.horrorTexture.loadFromFile("assets/buy_button_horror.png");
    selectButton.texture.loadFromFile("assets/select_button.png");
    selectButton.horrorTexture.loadFromFile("assets/select_button_horror.png");
    modeChangeButton.texture.loadFromFile("assets/modechange_button_horror.png");
    modeChangeButton.horrorTexture.loadFromFile("assets/modechange_button.png");
    modeChangeButton.sprite.setTexture(modeChangeButton.texture);
    yesButton.texture.loadFromFile("assets/yes_button.png");
    yesButton.sprite.setTexture(yesButton.texture);
    noButton.texture.loadFromFile("assets/no_button.png");
    noButton.sprite.setTexture(noButton.texture);
    leaderboardButton.texture.loadFromFile("assets/leaderboard_button.png");
    leaderboardButton.horrorTexture.loadFromFile("assets/leaderboard_button_horror.png");
    doneButton.texture.loadFromFile("assets/done_button.png");
    doneButton.horrorTexture.loadFromFile("assets/done_button_horror.png");
    menuBG[0].texture.loadFromFile("assets/menu_background.png");
    menuBG[0].horrorTexture.loadFromFile("assets/goremenu_background.png");
    menuBG[0].sprite.setTexture(menuBG[0].texture);
    menuBG[1].texture.loadFromFile("assets/pause_background.png");
    menuBG[1].sprite.setTexture(menuBG[1].texture);
    menuBG[2].texture.loadFromFile("assets/gameover_background.png");
    menuBG[2].sprite.setTexture(menuBG[2].texture);
    menuBG[3].texture.loadFromFile("assets/upgrade_background.png");
    menuBG[3].sprite.setTexture(menuBG[3].texture);
    menuBG[4].texture.loadFromFile("assets/shop_background.png");
    menuBG[4].horrorTexture.loadFromFile("assets/goreshop_background.png");
    menuBG[4].sprite.setTexture(menuBG[4].texture);
    menuBG[5].horrorTexture.loadFromFile("assets/gorealert_background.png");
    menuBG[5].sprite.setTexture(menuBG[5].texture);
    menuBG[6].texture.loadFromFile("assets/options_background.png");
    menuBG[6].horrorTexture.loadFromFile("assets/goreoptions_background.png");
    menuBG[6].sprite.setTexture(menuBG[6].texture);
    menuBG[7].texture.loadFromFile("assets/leaderboard_background.png");
    menuBG[7].horrorTexture.loadFromFile("assets/goreleaderboard_background.png");
    menuBG[7].sprite.setTexture(menuBG[7].texture);
    menuBG[8].texture.loadFromFile("assets/name_background.png");
    menuBG[8].horrorTexture.loadFromFile("assets/gorename_background.png");
    menuBG[8].sprite.setTexture(menuBG[8].texture);
    xpBarTexture.loadFromFile("assets/xpbar.png");
    xpBarSprite.setTexture(xpBarTexture);
    projectileTexture.loadFromFile("assets/projectile.png");
    projectile.sprite.setTexture(projectileTexture);
    pauseButton.texture.loadFromFile("assets/pause_button.png");
    pauseButton.horrorTexture.loadFromFile("assets/pause_button_horror.png");
    backtogameButton.texture.loadFromFile("assets/backtogame_button.png");
    backtogameButton.horrorTexture.loadFromFile("assets/backtogame_button_horror.png");
    backtogameButton.sprite.setTexture(backtogameButton.texture);
    backtomenuButton.texture.loadFromFile("assets/backtomenu_button.png");
    backtomenuButton.horrorTexture.loadFromFile("assets/backtomenu_button_horror.png");
    backtomenuButton.sprite.setTexture(backtomenuButton.texture);
    xpOrbTexture.loadFromFile("assets/xpOrb.png");
    menuCoinTexture.loadFromFile("assets/menu_coin.png");
    menuCoin.setTexture(menuCoinTexture);
    ringTexture.loadFromFile("assets/Rings.png");
    sliderButton.texture.loadFromFile("assets/slider_button.png");
    sliderButton.horrorTexture.loadFromFile("assets/slider_button_horror.png");
    sliderTexture.loadFromFile("assets/slider.png");
    slider.setTexture(sliderTexture);
    volumeButton.texture.loadFromFile("assets/volume_on_button.png");
    volumeButton.secondForm.loadFromFile("assets/volume_off_button.png");
    volumeButton.horrorTexture.loadFromFile("assets/volume_on_button_horror.png");
    volumeButton.horrorSecondForm.loadFromFile("assets/volume_off_button_horror.png");
    arrowsTexture.loadFromFile("assets/arrows.png");
    arrowsHorrorTexture.loadFromFile("assets/arrows_horror.png");
    wasdTexture.loadFromFile("assets/wasd.png");
    wasdHorrorTexture.loadFromFile("assets/wasd_horror.png");

    chestTexture.loadFromFile("assets/chest.png");
    potionTexture.loadFromFile("assets/potion.png");
    coinsTexture.loadFromFile("assets/coin.png");
    magnetTexture.loadFromFile("assets/magnet.png");
    chest.setTexture(chestTexture);
    chest1.sprite.setTexture(chestTexture);
    chest1.type = health;
    chest1.itemsprite.setTexture(potionTexture);
    chest1.sprite.setScale(1.5, 1.5);
    chest1.itemsprite.setScale(1.5, 1.5);
    chest2.sprite.setTexture(chestTexture);
    chest2.type = coin;
    chest2.itemsprite.setTexture(coinsTexture);
    chest2.sprite.setScale(1.5, 1.5);
    chest2.itemsprite.setScale(1.5, 1.5);
    chest3.sprite.setTexture(chestTexture);
    chest3.type = magnet;
    chest3.itemsprite.setTexture(magnetTexture);
    chest3.sprite.setScale(1.5, 1.5);
    chest3.itemsprite.setScale(1.5, 1.5);

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
    allUpgrades[6].texture.loadFromFile("assets/Rings_upgrade.png");
    allUpgrades[6].sprite.setTexture(allUpgrades[6].texture);
    //charachter textures
    player[0].texture.loadFromFile("assets/player_sheet.png");
    player[1].texture.loadFromFile("assets/adam_sheet.png");
    player[2].texture.loadFromFile("assets/aly_sheet.png");
    player[3].texture.loadFromFile("assets/amr_sheet.png");
    player[4].texture.loadFromFile("assets/logy_sheet.png");
    player[5].texture.loadFromFile("assets/maritsia_sheet.png");
    player[6].texture.loadFromFile("assets/yassin_sheet.png");
    player[7].texture.loadFromFile("assets/marwan_sheet.png");

}
void BorderCollision()
{
    // Get the background boundaries.
    FloatRect bgBounds = background.getGlobalBounds();


    Vector2f pos = player[character].sprite.getPosition(); // Get player's current position.



    FloatRect playerBounds = player[character].sprite.getGlobalBounds();

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
    player[character].sprite.setPosition(pos);
}
void lockViewToBackground()
{
    // Get the background bounds
    FloatRect bgBounds = background.getGlobalBounds();

    // Get half of the view size
    float halfViewWidth = view.getSize().x / 2;
    float halfViewHeight = view.getSize().y / 2;

    // Start with the player's position
    Vector2f desiredCenter = player[character].sprite.getPosition();

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
    if (whipCooldownTimer < player[character].whipCooldown)
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
            whip.setPosition(player[character].sprite.getPosition().x + 250, player[character].sprite.getPosition().y - 50);
        }
        else { // Left
            playPlayerHitSound();
            whip.setPosition(player[character].sprite.getPosition().x - 250, player[character].sprite.getPosition().y - 50);
        }

        // End of full swing animation
        if (whipIndx == 12)
        {
            if (upgradeLevel.whipLvl == 2 && swings == 0)
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
                whip.setScale(player[character].sprite.getScale());
                whip.setPosition(player[character].sprite.getPosition().x, player[character].sprite.getPosition().y);
            }
        }
    }

    // Extra cosmetic position when frame is 12
    if (whipIndx == 12)
    {
        if (player[character].position == 1) // Facing left
        {
            whip.setScale(1, 1);
            whip.setPosition(player[character].sprite.getPosition().x - 250, player[character].sprite.getPosition().y - 50);
        }
        else // Facing right
        {
            whip.setScale(-1, 1);
            whip.setPosition(player[character].sprite.getPosition().x + 250, player[character].sprite.getPosition().y - 50);
        }
    }
}
void whipCollider()
{
    Vector2f whipPos = whip.getPosition(); // this is the origin position
    Vector2f offset;

    if (whip.getScale().x < 0) // Facing right
    {
        offset.x = -50; // visually compensate
    }
    else // Facing left
    {
        offset.x = 50;
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
    Vector2f playerPos = player[character].sprite.getPosition();
    healthBarBackground.setPosition(playerPos.x, playerPos.y + 34);
    healthBarFill.setPosition(playerPos.x, playerPos.y + 34);

    // Update health bar fill based on current health
    float healthPercentage = player[character].currentHealth / player[character].maxHealth;
    Vector2f newSize = healthBarFill.getSize();
    newSize.x = healthBarBackground.getSize().x * healthPercentage;
    healthBarFill.setSize(newSize);

    if (player[character].currentHealth <= 0.05) {
        isDead = true;
        gameOverSound.play();
        gameoverMenu();
    }

}//Yassin Amr
void addXp(float xpToAdd)
{
    // Add XP from orb collection
    player[character].currentXP += xpToAdd;
    if (player[character].currentXP > player[character].maxXP)
    {
        player[character].currentXP = 0; // so that it never exceeds max
        player[character].maxXP *= 1.5;
        isUpgrading = true;
        isMenuOpen = true;
        levelCompleteSound.play();
    }

    // Update the fill size based on XP percentage
    float xpPercentage = player[character].currentXP / player[character].maxXP;
    Vector2f newSize = xpBarFill.getSize();
    newSize.x = xpBarTexture.getSize().x * xpPercentage;
    xpBarFill.setSize(newSize);

}//Yassin Amr
void takeDamage(float damage)
{
    float reduction = 0.10 * upgradeLevel.defenseUpgradeLevel;
    player[character].currentHealth -= damage * (1.0 - reduction);
    playEnemyHitSound();
    if (player[character].currentHealth < 0)
        player[character].currentHealth = 0;
}
void heal(float amount)
{
    player[character].currentHealth += amount;
    if (player[character].currentHealth > player[character].maxHealth)
        player[character].currentHealth = player[character].maxHealth;
}//Yassin Amr
void mainmenuWidgets() {
    logo.setTexture(logoTexture);
    logo.setOrigin(413.5, 263);
    logo.setPosition(1920 / 4, 1080 / 2);
    logo.setScale(1, 1);


    playButton.sprite.setPosition(1260, 200);
    playButton.sprite.setScale(2.0f, 2.0f);

    shopButton.sprite.setPosition(1260, 450);
    shopButton.sprite.setScale(2.0f, 2.0f);

    optionsButton.sprite.setPosition(1260, 700);
    optionsButton.sprite.setScale(2.0f, 2.0f);

    leaderboardButton.sprite.setPosition(300, 54);

    quitButton.sprite.setPosition(96, 54);

    coinsText.setString(coinFormatHandler((coins)));
    coinsText.setCharacterSize(100);
    coinsText.setPosition(1660, 0);
    coinsText.setFillColor(Color(205, 145, 43));
    coinsText.setOutlineColor(Color::Black);
    coinsText.setOutlineThickness(2);

    modeChangeButton.sprite.setPosition(1920 / 4 - 100, 900);

    if (gameMode == 0) { // normal textures
        modeChangeButton.sprite.setTexture(modeChangeButton.texture);
        playButton.sprite.setTexture(playButton.texture);
        menuBG[0].sprite.setTexture(menuBG[0].texture);
        optionsButton.sprite.setTexture(optionsButton.texture);
        shopButton.sprite.setTexture(shopButton.texture);
        quitButton.sprite.setTexture(quitButton.texture);
        pauseButton.sprite.setTexture(pauseButton.texture);
        backtomenuButton.sprite.setTexture(backtomenuButton.texture);
        backtogameButton.sprite.setTexture(backtogameButton.texture);
        leaderboardButton.sprite.setTexture(leaderboardButton.texture);
    }
    else if (gameMode == 1) { // horror textures
        modeChangeButton.sprite.setTexture(modeChangeButton.horrorTexture);
        playButton.sprite.setTexture(playButton.horrorTexture);
        menuBG[0].sprite.setTexture(menuBG[0].horrorTexture);
        optionsButton.sprite.setTexture(optionsButton.horrorTexture);
        shopButton.sprite.setTexture(shopButton.horrorTexture);
        quitButton.sprite.setTexture(quitButton.horrorTexture);
        pauseButton.sprite.setTexture(pauseButton.horrorTexture);
        backtomenuButton.sprite.setTexture(backtomenuButton.horrorTexture);
        backtogameButton.sprite.setTexture(backtogameButton.horrorTexture);
        leaderboardButton.sprite.setTexture(leaderboardButton.horrorTexture);
    }

    menuCoin.setPosition(1600, 35);
}
void bleedEffect()
{
    float recoveryTime = 0.3f;
    timeSinceLastHit += deltaTime;

    if (timeSinceLastHit < recoveryTime)
    {
        player[character].sprite.setColor(Color::Red);
    }
    else {
        player[character].sprite.setColor(Color::White);
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
            menu = 6;
            nameWidgets();
            sleep(milliseconds(200));
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
            coinsText.setPosition(200, 50);
            menuCoin.setPosition(140, 85);
            sleep(milliseconds(200));
            shopPage = 0;
            shopWidgets();

            for (int i = 0; i < 8; i++) {
                player[i].sprite.setScale(5, 5);
                player[i].sprite.setPosition(570, 1080 / 2 + 50);
                player[i].sprite.setColor(Color::White);
            }
            menu = 2;
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
            sleep(milliseconds(200));

            backtomenuButton.sprite.setPosition(1920 / 1.2 + 100, 1080 / 5 - 50);
            backtomenuButton.sprite.setScale(1, 1);

            menu = 4;

            optionsWidgets();

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
            clickSound.play();
            sleep(milliseconds(200));
            window.close();
        }
    }
    else
    {
        quitButton.sprite.setTextureRect(IntRect(0, 0, 102, 108));
        quitButton.sprite.setColor(Color::White);
    }

    if (modeChangeButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        modeChangeButton.sprite.setTextureRect(IntRect(0, 108, 102, 108));
        modeChangeButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (gameMode == 0) {
                menu = 3;
            }
            else if (gameMode == 1) {
                gameMode = 0;
                mainmenuWidgets();
            }
            clickSound.play();
            sleep(milliseconds(200));
        }
    }
    else
    {
        modeChangeButton.sprite.setTextureRect(IntRect(0, 0, 102, 108));
        modeChangeButton.sprite.setColor(Color::White);
    }

    if (leaderboardButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        leaderboardButton.sprite.setTextureRect(IntRect(0, 108, 102, 108));
        leaderboardButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            menu = 5;
            clickSound.play();
            sleep(milliseconds(200));
            leaderboardWidgets();
        }
    }
    else
    {
        leaderboardButton.sprite.setTextureRect(IntRect(0, 0, 102, 108));
        leaderboardButton.sprite.setColor(Color::White);
    }

}
Vector2f getDirectionToNearestZombie() {
    if (Zombies.empty()) {
        return Vector2f(0, 0);  // Return zero vector if no zombies
    }

    float minDistance = FLT_MAX;
    Vector2f nearestZombiePos;

    for (int i = 0; i < Zombies.size(); i++) {
        float distance = sqrt(pow(Zombies[i].Shape.getPosition().x - player[character].sprite.getPosition().x, 2) +
            pow(Zombies[i].Shape.getPosition().y - player[character].sprite.getPosition().y, 2));
        if (distance < minDistance) {
            minDistance = distance;
            nearestZombiePos = Zombies[i].Shape.getPosition();
        }
    }

    // Calculate direction to nearest zombie
    Vector2f direction = nearestZombiePos - player[character].sprite.getPosition();
    return Normalize(direction);  // Use the existing Normalize function
}
void createProjectile()
{
    if (projectileTimer >= PROJECTILE_COOLDOWN && !projectile.active && !Zombies.empty())
    {
        projectileTimer = 0;  // Reset timer
        projectile.sprite.setOrigin(projectile.sprite.getLocalBounds().width / 2, projectile.sprite.getLocalBounds().height / 2);
        projectile.sprite.setPosition(player[character].sprite.getPosition());
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

                // Apply damage
                zombie.HP -= player[character].ProjectileDamage;

                // Apply bleed effect
                zombie.Shape.setColor(Color::Red);
                zombie.canBeHit = false;  // Mark zombie as hit
                zombie.lastHitTime = 0.0f;  // Reset hit timer

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

    // Reset zombie color after bleed effect duration
    for (auto& zombie : Zombies) {
        if (zombie.lastHitTime > 0.15f) {  // 0.15 seconds bleed effect duration
            zombie.Shape.setColor(Color::White);
        }
    }
}
Enemies selectSpwanZombie() {
    Enemies newZombie;
    int diffrence = 100;
    int selectKills = zombiesKilled % (diffrence * numOfZombieTypes);

    if (selectKills <= diffrence * 1) {
        newZombie = ZombieTypes[0];
    }
    else if (selectKills <= diffrence * 2) {
        newZombie = ZombieTypes[1];
    }
    else if (selectKills <= diffrence * 3) {
        newZombie = ZombieTypes[2];
    }
    else if (selectKills <= diffrence * 4) {
        newZombie = ZombieTypes[3];
    }
    else if (selectKills <= diffrence * 5) {
        newZombie = ZombieTypes[4];
    }
    else if (selectKills <= diffrence * 6) {
        newZombie = ZombieTypes[5];
    }
    else if (selectKills <= diffrence * 7) {
        newZombie = ZombieTypes[6];
    }
    else if (selectKills <= diffrence * 8) {
        newZombie = ZombieTypes[7];
    }
    else if (selectKills <= diffrence * 9) {
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
            // Assign XP based on zombie type
            float xpValue = 0.0f;
            switch (Zombies[i].type) {
            case 0: xpValue = 1.0f; break;
            case 1: xpValue = 1.5f; break;
            case 2: xpValue = 1.5f; break;
            case 3: xpValue = 2.0f; break;
            case 4: xpValue = 2.0f; break;
            case 5: xpValue = 2.5f; break;
            case 6: xpValue = 3.0f; break;
            case 7: xpValue = 2.0f; break;
            case 8: xpValue = 3.0f; break;
            case 9: xpValue = 5.0f; break;
            }

            // Create XP orb with the assigned XP value
            createXPOrb(Zombies[i].Shape.getPosition(), xpValue);

            // Handle coins and remove the zombie
            int coinChance = rand() % 3;
            coins += coinChance;


            Zombies.erase(Zombies.begin() + i);
            zombiesKilled++;
            continue;
        }

        // Update zombie's hit cooldown for whip
        if (!Zombies[i].canBeHit) {
            Zombies[i].lastHitTime += deltaTime;
            if (Zombies[i].lastHitTime >= player[character].whipCooldown) {
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

            Vector2f diff = pos1 - pos2; //line pointing from zombie 1 to zombie 2
            float dist = sqrt(diff.x * diff.x + diff.y * diff.y);  // distance between the zombies

            if (dist < minDistance && dist > 0.0f) {
                Vector2f offset = Normalize(diff); // normalize diff
                offset = offset * (minDistance - dist) / 2.0f; // calc distance each should move
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
            clickSound.play();
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
    if (backtogameButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        backtogameButton.sprite.setTextureRect(IntRect(0, 108, 330, 108));
        backtogameButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            clickSound.play();
            isPaused = 0;

            Time pausedDuration = timerClock.getElapsedTime() - pauseStart;
            totalPausedTime += pausedDuration;
        }
    }
    else
    {
        backtogameButton.sprite.setTextureRect(IntRect(0, 0, 330, 108));
        backtogameButton.sprite.setColor(Color::White);
    }
}
void backtomenuButtonHandler() {
    if (backtomenuButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        backtomenuButton.sprite.setTextureRect(IntRect(0, 108, 330, 108));
        backtomenuButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {

            if (menu == 1) { // logic that happens only if we back to menu from game
                backgroundMusic.play();
                goreModeMusic.pause();
                normalLevelMusic.pause();

                if (zombiesKilled > highscores[9].score) {
                    highscores[9].score = zombiesKilled;
                    highscores[9].time = time();

                    if (current.name == "") {
                        highscores[9].name = "Anonymous";
                    }
                    else {
                        highscores[9].name = current.name;
                    }
                }
            }
            clickSound.play();
            mainmenuWidgets();
            menu = 0;
        }
    }
    else
    {
        backtomenuButton.sprite.setTextureRect(IntRect(0, 0, 330, 108));
        backtomenuButton.sprite.setColor(Color::White);
    }
}
void zombieInitalization() {

    //set assets
    if (gameMode == 0) {
        //Enemy 1/Normal Zombie
        zombieTextureSheets[0].loadFromFile("assets/Zombie1.png");
        ZombieTypes[0].hitboxWidth = 35;
        ZombieTypes[0].hitboxHeight = 48;
        ZombieTypes[0].spriteWidth = 35;
        ZombieTypes[0].spriteHeight = 48;
        ZombieTypes[0].colSize = 3;

        //Enemy 2/Ice Zombie
        zombieTextureSheets[1].loadFromFile("assets/Zombie2.png");
        ZombieTypes[1].hitboxWidth = 35;
        ZombieTypes[1].hitboxHeight = 48;
        ZombieTypes[1].spriteWidth = 35;
        ZombieTypes[1].spriteHeight = 48;
        ZombieTypes[1].colSize = 3;

        //Enemy 3
        zombieTextureSheets[2].loadFromFile("assets/Zombie3.png");
        ZombieTypes[2].hitboxWidth = 35;
        ZombieTypes[2].hitboxHeight = 48;
        ZombieTypes[2].spriteWidth = 35;
        ZombieTypes[2].spriteHeight = 48;
        ZombieTypes[2].colSize = 3;

        //Enemy 4
        zombieTextureSheets[3].loadFromFile("assets/Zombie4.png");
        ZombieTypes[3].hitboxWidth = 35;
        ZombieTypes[3].hitboxHeight = 48;
        ZombieTypes[3].spriteWidth = 35;
        ZombieTypes[3].spriteHeight = 48;
        ZombieTypes[3].colSize = 3;

        //Enemy 5
        zombieTextureSheets[4].loadFromFile("assets/Zombie5.png");
        ZombieTypes[4].hitboxWidth = 35;
        ZombieTypes[4].hitboxHeight = 48;
        ZombieTypes[4].spriteWidth = 35;
        ZombieTypes[4].spriteHeight = 48;
        ZombieTypes[4].colSize = 3;

        //Enemy 6
        zombieTextureSheets[5].loadFromFile("assets/Zombie6.png");
        ZombieTypes[5].hitboxWidth = 35;
        ZombieTypes[5].hitboxHeight = 43;
        ZombieTypes[5].spriteWidth = 35;
        ZombieTypes[5].spriteHeight = 43;
        ZombieTypes[5].colSize = 3;

        //Enemy 7
        zombieTextureSheets[6].loadFromFile("assets/Zombie7.png");
        ZombieTypes[6].hitboxWidth = 35;
        ZombieTypes[6].hitboxHeight = 48;
        ZombieTypes[6].spriteWidth = 35;
        ZombieTypes[6].spriteHeight = 48;
        ZombieTypes[6].colSize = 3;

        //Enemy 8
        zombieTextureSheets[7].loadFromFile("assets/Zombie8.png");
        ZombieTypes[7].hitboxWidth = 35;
        ZombieTypes[7].hitboxHeight = 41;
        ZombieTypes[7].spriteWidth = 35;
        ZombieTypes[7].spriteHeight = 41;
        ZombieTypes[7].colSize = 3;

        //Enemy 9
        zombieTextureSheets[8].loadFromFile("assets/Zombie9.png");
        ZombieTypes[8].hitboxWidth = 35;
        ZombieTypes[8].hitboxHeight = 48;
        ZombieTypes[8].spriteWidth = 35;
        ZombieTypes[8].spriteHeight = 48;
        ZombieTypes[8].colSize = 3;

        //Enemy 10
        zombieTextureSheets[9].loadFromFile("assets/Zombie10.png");
        ZombieTypes[9].hitboxWidth = 35;
        ZombieTypes[9].hitboxHeight = 48;
        ZombieTypes[9].spriteWidth = 35;
        ZombieTypes[9].spriteHeight = 48;
        ZombieTypes[9].colSize = 3;

    }
    else if (gameMode == 1) {
        //Enemy 1/Normal Zombie
        zombieTextureSheets[0].loadFromFile("assets/Zombie1horror.png");
        ZombieTypes[0].hitboxWidth = 40;
        ZombieTypes[0].hitboxHeight = 44;
        ZombieTypes[0].spriteWidth = 40;
        ZombieTypes[0].spriteHeight = 44;
        ZombieTypes[0].colSize = 7;

        //Enemy 2/Ice Zombie
        zombieTextureSheets[1].loadFromFile("assets/Zombie2horror.png");
        ZombieTypes[1].hitboxWidth = 30;
        ZombieTypes[1].hitboxHeight = 40;
        ZombieTypes[1].spriteWidth = 30;
        ZombieTypes[1].spriteHeight = 40;
        ZombieTypes[1].colSize = 7;

        //Enemy 3
        zombieTextureSheets[2].loadFromFile("assets/Zombie3horror.png");
        ZombieTypes[2].hitboxWidth = 40;
        ZombieTypes[2].hitboxHeight = 44;
        ZombieTypes[2].spriteWidth = 40;
        ZombieTypes[2].spriteHeight = 44;
        ZombieTypes[2].colSize = 5;

        //Enemy 4
        zombieTextureSheets[3].loadFromFile("assets/Zombie4horror.png");
        ZombieTypes[3].hitboxWidth = 34;
        ZombieTypes[3].hitboxHeight = 46;
        ZombieTypes[3].spriteWidth = 34;
        ZombieTypes[3].spriteHeight = 46;
        ZombieTypes[3].colSize = 12;

        //Enemy 5
        zombieTextureSheets[4].loadFromFile("assets/Zombie5horror.png");
        ZombieTypes[4].hitboxWidth = 34;
        ZombieTypes[4].hitboxHeight = 50;
        ZombieTypes[4].spriteWidth = 34;
        ZombieTypes[4].spriteHeight = 50;
        ZombieTypes[4].colSize = 3;

        //Enemy 6
        zombieTextureSheets[5].loadFromFile("assets/Zombie6horror.png");
        ZombieTypes[5].hitboxWidth = 34;
        ZombieTypes[5].hitboxHeight = 52;
        ZombieTypes[5].spriteWidth = 34;
        ZombieTypes[5].spriteHeight = 52;
        ZombieTypes[5].colSize = 3;

        //Enemy 7
        zombieTextureSheets[6].loadFromFile("assets/Zombie7horror.png");
        ZombieTypes[6].hitboxWidth = 44;
        ZombieTypes[6].hitboxHeight = 46;
        ZombieTypes[6].spriteWidth = 44;
        ZombieTypes[6].spriteHeight = 46;
        ZombieTypes[6].colSize = 14;

        //Enemy 8
        zombieTextureSheets[7].loadFromFile("assets/Zombie8horror.png");
        ZombieTypes[7].hitboxWidth = 23;
        ZombieTypes[7].hitboxHeight = 36;
        ZombieTypes[7].spriteWidth = 46;
        ZombieTypes[7].spriteHeight = 40;
        ZombieTypes[7].colSize = 3;

        //Enemy 9
        zombieTextureSheets[8].loadFromFile("assets/Zombie9horror.png");
        ZombieTypes[8].hitboxWidth = 36;
        ZombieTypes[8].hitboxHeight = 50;
        ZombieTypes[8].spriteWidth = 36;
        ZombieTypes[8].spriteHeight = 50;
        ZombieTypes[8].colSize = 13;

        //Enemy 10
        zombieTextureSheets[9].loadFromFile("assets/Zombie10horror.png");
        ZombieTypes[9].hitboxWidth = 30;
        ZombieTypes[9].hitboxHeight = 48;
        ZombieTypes[9].spriteWidth = 30;
        ZombieTypes[9].spriteHeight = 48;
        ZombieTypes[9].colSize = 13;
    }

    //set stats
    //Enemy 1/Normal Zombie

    ZombieTypes[0].type = 0;
    ZombieTypes[0].DMG = 2;
    ZombieTypes[0].HP = 10;
    ZombieTypes[0].speed = 160;
    ZombieTypes[0].attackRate = 0.25;
    ZombieTypes[0].AnimtaionRate = 0.135f;

    //Enemy 2/Ice Zombie

    ZombieTypes[1].type = 1;
    ZombieTypes[1].DMG = 5;
    ZombieTypes[1].HP = 30;
    ZombieTypes[1].speed = 140;
    ZombieTypes[1].attackRate = 0.25;
    ZombieTypes[1].AnimtaionRate = 0.135f;

    //Enemy 3

    ZombieTypes[2].type = 2;
    ZombieTypes[2].DMG = 3;
    ZombieTypes[2].HP = 10;
    ZombieTypes[2].speed = 170;
    ZombieTypes[2].attackRate = 0.25;
    ZombieTypes[2].AnimtaionRate = 0.135f;

    //Enemy 4

    ZombieTypes[3].type = 3;
    ZombieTypes[3].DMG = 10;
    ZombieTypes[3].HP = 30;
    ZombieTypes[3].speed = 160;
    ZombieTypes[3].attackRate = 0.25;
    ZombieTypes[3].AnimtaionRate = 0.135f;

    //Enemy 5

    ZombieTypes[4].type = 4;
    ZombieTypes[4].DMG = 1;
    ZombieTypes[4].HP = 50;
    ZombieTypes[4].speed = 210;
    ZombieTypes[4].attackRate = 0.25;
    ZombieTypes[4].AnimtaionRate = 0.135f;

    //Enemy 6

    ZombieTypes[5].type = 5;
    ZombieTypes[5].DMG = 10;
    ZombieTypes[5].HP = 20;
    ZombieTypes[5].speed = 150;
    ZombieTypes[5].attackRate = 0.25;
    ZombieTypes[5].AnimtaionRate = 0.135f;

    //Enemy 7

    ZombieTypes[6].type = 6;
    ZombieTypes[6].DMG = 5;
    ZombieTypes[6].HP = 70;
    ZombieTypes[6].speed = 150;
    ZombieTypes[6].attackRate = 0.25;
    ZombieTypes[6].AnimtaionRate = 0.135f;

    //Enemy 8

    ZombieTypes[7].type = 7;
    ZombieTypes[7].DMG = 15;
    ZombieTypes[7].HP = 1;
    ZombieTypes[7].speed = 230;
    ZombieTypes[7].attackRate = 0.25;
    ZombieTypes[7].AnimtaionRate = 0.135f;

    //Enemy 9

    ZombieTypes[8].type = 8;
    ZombieTypes[8].DMG = 5;
    ZombieTypes[8].HP = 30;
    ZombieTypes[8].speed = 150;
    ZombieTypes[8].attackRate = 0.25;
    ZombieTypes[8].AnimtaionRate = 0.135f;

    //Enemy 10

    ZombieTypes[9].type = 9;
    ZombieTypes[9].DMG = 20;
    ZombieTypes[9].HP = 100;
    ZombieTypes[9].speed = 210;
    ZombieTypes[9].attackRate = 0.25;
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
    charachterInitalization();
    zombieInitalization();
    backgroundInitialization();
    player[character].Start();
    player[character].currentXP = 0.0f;
    player[character].currentHealth = player[character].maxHealth;
    player[character].isMoving = false;
    player[character].position = true; // true = left  | false  = right
    player[character].canAttack = true;
    player[character].level = 1;
    player[character].maxXP = 10;
    Zombies.clear();
    xpOrbs.clear();
    ring.angle = 0.0f;
    projectile.active = false;
    isDead = false;
    isPaused = false;
    zombiesKilled = 0;
    healingUpgradeLevel = 0;
    isUpgrading = false;
    walkIndx = 0;
    whipIndx = 0;
    whipCooldownTimer = 0;
    timeSinceLastHit = 999;
    swings = 0;
    upgradeLevel.whipLvl = 1;
    upgradeLevel.healingUpgradeLevel = 0;
    upgradeLevel.defenseUpgradeLevel = 0;
    upgradeLevel.increasePlayerSpeedLevel = 0;
    upgradeLevel.maxHealthLevel = 0;
    upgradeLevel.secondWeaponUpgradeLvl = 0;
    for (int i = 0; i < UPGRADES_NUM; i++) {
        currentUpgradelvl[i] = 0;
    }
    timerClock.restart();
    totalPausedTime = Time::Zero;
    whip.setPosition(5000, 5000);
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
    coinsText.setPosition(1660, 900);
    coinsText.setFillColor(Color(205, 145, 43));
    coinsText.setOutlineColor(Color::Black);
    coinsText.setOutlineThickness(2);
    menuCoin.setPosition(1600, 935);

    xpBarFill.setSize(Vector2f(0, 15));

    player[character].sprite.setPosition(background.getGlobalBounds().width / 2, background.getGlobalBounds().height / 2);
    player[character].sprite.setScale(1, 1);
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
                zombie.HP -= player[character].whipDamage;
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
void createXPOrb(Vector2f position, float xpValue) {
    XPOrb newOrb;
    newOrb.sprite.setTexture(xpOrbTexture);
    newOrb.sprite.setOrigin(newOrb.sprite.getLocalBounds().width / 2, newOrb.sprite.getLocalBounds().height / 2);
    newOrb.sprite.setPosition(position);
    newOrb.xpValue = xpValue;

    if (xpValue == 1.0f) {
        newOrb.sprite.setScale(0.5f, 0.5f);
        newOrb.sprite.setColor(Color::Cyan);
    }
    else if (xpValue == 1.5f) {
        newOrb.sprite.setScale(0.7f, 0.7f);
        newOrb.sprite.setColor(Color::Yellow);
    }
    else if (xpValue == 2.0f) {
        newOrb.sprite.setScale(1.0f, 1.0f);
        newOrb.sprite.setColor(Color(255, 165, 0));
    }
    else if (xpValue == 2.5f) {
        newOrb.sprite.setScale(1.1f, 1.1f);
        newOrb.sprite.setColor(Color(255, 69, 0));
    }
    else if (xpValue == 3.0f) {
        newOrb.sprite.setScale(1.3f, 1.3f);
        newOrb.sprite.setColor(Color::Red);
    }
    else if (xpValue == 5.0f) {
        newOrb.sprite.setScale(1.5f, 1.5f);
        newOrb.sprite.setColor(Color(128, 0, 128));
    }

    xpOrbs.push_back(newOrb);
}
void updateXPOrbs() {
    for (int i = 0; i < xpOrbs.size(); i++) {
        if (xpOrbs[i].sprite.getGlobalBounds().intersects(player[character].sprite.getGlobalBounds())) {
            addXp(xpOrbs[i].xpValue); // Add the XP value of the orb
            xpOrbs.erase(xpOrbs.begin() + i);
        }
    }
}
void gameoverMenu() {

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

    leaderboardSort();

    // compare with lowest score and if higher replace it with current
    if (zombiesKilled > highscores[9].score) {
        highscores[9].score = zombiesKilled;
        highscores[9].time = time();
        if (current.name == "") {
            highscores[9].name = "Anonymous";
        }
        else {
            highscores[9].name = current.name;
        }
    }

    //gameoverText[3].setString("You can do better.");
    //gameoverText[3].setPosition(1920 / 4 + 130, 1080 / 4 + 550);
}
void gameoverMenuHandler() {

    for (int i = 0; i < 4; i++) {
        window.draw(gameoverText[i]);
    }

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
void charachterInitalization()
{

    //Default 0 /player
    player[0].sprite.setTexture(player[0].texture);
    player[0].isProjectileUnlocked = false;
    player[0].isWhipUnlocked = true;
    player[0].isUnlocked = true;
    player[0].isRingUnlocked = false;
    player[0].speed = 200;
    player[0].position = true;
    player[0].animationTimer = 0;
    player[0].level = 1;
    player[0].isMoving = false;
    player[0].canAttack = true;
    player[0].whipDamage = 10;
    player[0].ringDamage = 5;
    player[0].whipCooldown = 1.0f;
    player[0].ProjectileDamage = 10;
    player[0].projectileCooldown = 2.0f;
    player[0].playerWidth = 40;
    player[0].playerHeight = 56;
    player[0].hitboxWidth = 20;
    player[0].hitboxHeight = 40;
    player[0].playerFrameIndx = 16;
    player[0].maxHealth = 100.0f;
    player[0].currentHealth = player[0].maxHealth;
    player[0].maxXP = 10.0f;
    player[0].currentXP = 0.0f;







    //Charachter 1/Adam

    player[1].sprite.setTexture(player[1].texture);
    player[1].playerFrameIndx = 15;
    player[1].isProjectileUnlocked = false;
    player[1].isWhipUnlocked = true;
    player[1].isRingUnlocked = false;
    player[1].speed = 170;
    player[1].position = true;
    player[1].animationTimer = 0;
    player[1].level = 1;
    player[1].isMoving = false;
    player[1].canAttack = true;
    player[1].whipDamage = 10;
    player[1].ringDamage = 5;
    player[1].whipCooldown = 1.0f;
    player[1].ProjectileDamage = 15;
    player[1].projectileCooldown = 2.0f;
    player[1].playerWidth = 50;
    player[1].playerHeight = 56;
    player[1].hitboxWidth = 20;
    player[1].hitboxHeight = 40;
    player[1].maxHealth = 500;
    player[1].currentHealth = player[1].maxHealth;
    player[1].maxXP = 10.0f;
    player[1].currentXP = 0.0f;



    //Charachter 2 /Aly


    player[2].sprite.setTexture(player[2].texture);
    player[2].playerFrameIndx = 15;
    player[2].isProjectileUnlocked = false;
    player[2].isWhipUnlocked = true;
    player[2].isRingUnlocked = false;
    player[2].speed = 230;
    player[2].position = true;
    player[2].animationTimer = 0;
    player[2].level = 1;
    player[2].isMoving = false;
    player[2].canAttack = true;
    player[2].whipDamage = 10;
    player[2].ringDamage = 5;
    player[2].whipCooldown = 1.0f;
    player[2].ProjectileDamage = 15;
    player[2].projectileCooldown = 2.0f;
    player[2].playerWidth = 50;
    player[2].playerHeight = 56;
    player[2].hitboxWidth = 20;
    player[2].hitboxHeight = 40;
    player[2].maxHealth = 100;
    player[2].currentHealth = player[1].maxHealth;
    player[2].maxXP = 10.0f;
    player[2].currentXP = 0.0f;



    //Charchter 3 / Amr


    player[3].sprite.setTexture(player[3].texture);
    player[3].playerFrameIndx = 15;
    player[3].isProjectileUnlocked = false;
    player[3].isWhipUnlocked = true;
    player[3].isRingUnlocked = false;
    player[3].speed = 200;
    player[3].position = true;
    player[3].animationTimer = 0;
    player[3].level = 1;
    player[3].isMoving = false;
    player[3].canAttack = true;
    player[3].whipDamage = 20;
    player[3].ringDamage = 5;
    player[3].whipCooldown = 0.5f;
    player[3].ProjectileDamage = 15;
    player[3].projectileCooldown = 2.0f;
    player[3].playerWidth = 50;
    player[3].playerHeight = 64;
    player[3].hitboxWidth = 20;
    player[3].hitboxHeight = 40;
    player[3].maxHealth = 100;
    player[3].currentHealth = player[1].maxHealth;
    player[3].maxXP = 10.0f;
    player[3].currentXP = 0.0f;



    //Charchter 4 / Logy

    player[4].sprite.setTexture(player[4].texture);
    player[4].playerFrameIndx = 13;
    player[4].isProjectileUnlocked = false;
    player[4].isWhipUnlocked = true;
    player[4].isRingUnlocked = false;
    player[4].speed = 170;
    player[4].position = true;
    player[4].animationTimer = 0;
    player[4].level = 1;
    player[4].isMoving = false;
    player[4].canAttack = true;
    player[4].whipDamage = 10;
    player[4].ringDamage = 5;
    player[4].whipCooldown = 1.0f;
    player[4].ProjectileDamage = 15;
    player[4].projectileCooldown = 2.0f;
    player[4].playerWidth = 50;
    player[4].playerHeight = 58;
    player[4].hitboxWidth = 20;
    player[4].hitboxHeight = 40;
    player[4].maxHealth = 100;
    player[4].currentHealth = player[1].maxHealth;
    player[4].maxXP = 10.0f;
    player[4].currentXP = 0.0f;



    //Charchter 5 / Maritsia

    player[5].sprite.setTexture(player[5].texture);
    player[5].playerFrameIndx = 13;
    player[5].isProjectileUnlocked = false;
    player[5].isWhipUnlocked = true;
    player[5].isRingUnlocked = false;
    player[5].speed = 170;
    player[5].position = true;
    player[5].animationTimer = 0;
    player[5].level = 1;
    player[5].isMoving = false;
    player[5].canAttack = true;
    player[5].whipDamage = 10;
    player[5].ringDamage = 5;
    player[5].whipCooldown = 1.0f;
    player[5].ProjectileDamage = 15;
    player[5].projectileCooldown = 2.0f;
    player[5].playerWidth = 50;
    player[5].playerHeight = 56;
    player[5].hitboxWidth = 20;
    player[5].hitboxHeight = 40;
    player[5].maxHealth = 100;
    player[5].currentHealth = player[1].maxHealth;
    player[5].maxXP = 10.0f;
    player[5].currentXP = 0.0f;


    //Charchter 6 / Yassin

    player[6].sprite.setTexture(player[6].texture);
    player[6].playerFrameIndx = 15;
    player[6].isProjectileUnlocked = false;
    player[6].isWhipUnlocked = true;
    player[6].isRingUnlocked = false;
    player[6].speed = 300;
    player[6].position = true;
    player[6].animationTimer = 0;
    player[6].level = 1;
    player[6].isMoving = false;
    player[6].canAttack = true;
    player[6].whipDamage = 10;
    player[6].ringDamage = 5;
    player[6].whipCooldown = 1.0f;
    player[6].ProjectileDamage = 15;
    player[6].projectileCooldown = 2.0f;
    player[6].playerWidth = 32;
    player[6].playerHeight = 60;
    player[6].hitboxWidth = 20;
    player[6].hitboxHeight = 40;
    player[6].maxHealth = 50;
    player[6].currentHealth = player[1].maxHealth;
    player[6].maxXP = 10.0f;
    player[6].currentXP = 0.0f;

    //Charchter 7 // Marwan


    player[7].sprite.setTexture(player[7].texture);
    player[7].playerFrameIndx = 15;
    player[7].isProjectileUnlocked = false;
    player[7].isWhipUnlocked = true;
    player[7].isRingUnlocked = false;
    player[7].speed = 230;
    player[7].position = true;
    player[7].animationTimer = 0;
    player[7].level = 1;
    player[7].isMoving = false;
    player[7].canAttack = true;
    player[7].whipDamage = 10;
    player[7].ringDamage = 5;
    player[7].whipCooldown = 1.0f;
    player[7].ProjectileDamage = 15;
    player[7].projectileCooldown = 2.0f;
    player[7].playerWidth = 50;
    player[7].playerHeight = 56;
    player[7].hitboxWidth = 20;
    player[7].hitboxHeight = 40;
    player[7].maxHealth = 100;
    player[7].currentHealth = player[1].maxHealth;
    player[7].maxXP = 10.0f;
    player[7].currentXP = 0.0f;

}
void shopWidgets()
{

    backButton.sprite.setPosition(60, 1080 / 2);
    nextButton.sprite.setPosition(1750, 1080 / 2);
    buyButton.sprite.setPosition(1250, 830);
    selectButton.sprite.setPosition(1250, 830);
    backtomenuButton.sprite.setPosition(1920 / 1.2 + 100, 1080 / 5 - 50);
    backtomenuButton.sprite.setScale(1, 1);

    if (gameMode == 0) {
        menuBG[4].sprite.setTexture(menuBG[4].texture);
        backtomenuButton.sprite.setTexture(backtomenuButton.texture);
        backButton.sprite.setTexture(backButton.texture);
        nextButton.sprite.setTexture(nextButton.texture);
        buyButton.sprite.setTexture(buyButton.texture);
        selectButton.sprite.setTexture(selectButton.texture);
    }
    else if (gameMode == 1) {
        menuBG[4].sprite.setTexture(menuBG[4].horrorTexture);
        backtomenuButton.sprite.setTexture(backtomenuButton.horrorTexture);
        backButton.sprite.setTexture(backButton.horrorTexture);
        nextButton.sprite.setTexture(nextButton.horrorTexture);
        buyButton.sprite.setTexture(buyButton.horrorTexture);
        selectButton.sprite.setTexture(selectButton.horrorTexture);
    }
}
void backButtonHandler()
{
    if (backButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        backButton.sprite.setTextureRect(IntRect(0, 108, 102, 108));
        backButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left) && shopPage > 0) {
            clickSound.play();
            shopPage--;

            sleep(milliseconds(200));
        }
    }
    else
    {
        backButton.sprite.setTextureRect(IntRect(0, 0, 102, 108));
        backButton.sprite.setColor(Color::White);
    }
}
void nextButtonHandler()
{
    if (nextButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        nextButton.sprite.setTextureRect(IntRect(0, 108, 102, 108));
        nextButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left) && shopPage < 7) {
            clickSound.play();
            shopPage++;
            sleep(milliseconds(200));
        }
    }
    else
    {
        nextButton.sprite.setTextureRect(IntRect(0, 0, 102, 108));
        nextButton.sprite.setColor(Color::White);
    }
}
void buyButtonHandler()
{
    if (coins >= 500 && !player[shopPage].isUnlocked) {
        if (buyButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
        {
            buyButton.sprite.setTextureRect(IntRect(0, 108, 222, 108));
            buyButton.sprite.setColor(sf::Color(200, 200, 200));
            if (Mouse::isButtonPressed(Mouse::Left)) {
                coinSound.play();
                coins -= 500;
                player[shopPage].isUnlocked = true;
                sleep(milliseconds(200));
            }
        }
        else
        {
            buyButton.sprite.setTextureRect(IntRect(0, 0, 222, 108));
            buyButton.sprite.setColor(Color::White);
        }
    }
    else {
        buyButton.sprite.setTextureRect(IntRect(0, 0, 222, 108));
        buyButton.sprite.setColor(Color(73, 73, 73));
    }

}
void selectButtonHandler()
{
    if (menu == 2) // shop
    {
        if (player[shopPage].isUnlocked) {
            if (selectButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
            {
                selectButton.sprite.setTextureRect(IntRect(0, 108, 222, 108));
                selectButton.sprite.setColor(sf::Color(200, 200, 200));
                if (Mouse::isButtonPressed(Mouse::Left)) {
                    clickSound.play();
                    character = shopPage;
                    sleep(milliseconds(200));
                }

            }
            else
            {
                selectButton.sprite.setTextureRect(IntRect(0, 0, 222, 108));
                selectButton.sprite.setColor(Color::White);
            }
        }
    }

    if (menu == 4) { // options
        if (selectButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
        {

            selectButton.sprite.setTextureRect(IntRect(0, 108, 222, 108));
            selectButton.sprite.setColor(sf::Color(200, 200, 200));
            if (Mouse::isButtonPressed(Mouse::Left)) {
                if (controls == 1) { // from wasd -> arrows
                    controls = 0;
                    selectButton.sprite.setPosition(1920 / 1.7, 800);
                    clickSound.play();
                    sleep(milliseconds(200));
                }
                else if (controls == 0) { // from arrows -> wasd
                    controls = 1;
                    selectButton.sprite.setPosition(1920 / 3.45, 800);
                    clickSound.play();
                    sleep(milliseconds(200));
                }
            }

        }
        else
        {
            selectButton.sprite.setTextureRect(IntRect(0, 0, 222, 108));
            selectButton.sprite.setColor(Color::White);
        }


    }
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
    chestBreakBuffer.loadFromFile("assets/sounds/health_recharge.mp3");
    potionDrinkBuffer.loadFromFile("assets/sounds/potion_drink.mp3");

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
    chestBreakSound.setBuffer(chestBreakBuffer);
    potionDrinkSound.setBuffer(potionDrinkBuffer);

    backgroundMusic.openFromFile("assets/sounds/background.mp3");
    normalLevelMusic.openFromFile("assets/sounds/normal_level.mp3");
    goreModeMusic.openFromFile("assets/sounds/gore_mode.mp3");

    clickSound.setVolume(volume);
    deathSound.setVolume(volume);
    gameOverSound.setVolume(volume);
    footstepsSound.setVolume(volume);
    enemyAppearSound.setVolume(volume);
    enemyHitSound.setVolume(volume);
    playerHitSound.setVolume(volume);
    healthSound.setVolume(volume);
    bossSound.setVolume(volume);
    coinSound.setVolume(volume);
    levelCompleteSound.setVolume(volume);
    chestBreakSound.setVolume(volume);
    potionDrinkSound.setVolume(volume);
    backgroundMusic.setVolume(volume);
    normalLevelMusic.setVolume(volume);
    goreModeMusic.setVolume(volume);
}
void playBackgroundMusic()
{
    backgroundMusic.setLoop(true);
    backgroundMusic.play();
}
void playNormalLevelMusic()
{
    normalLevelMusic.setLoop(true);
    normalLevelMusic.play();
}
void playGoreLevelMusic()
{
    goreModeMusic.setLoop(true);
    goreModeMusic.play();
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
void playChestBreakSound() {
    if (chestBreakSound.getStatus() != Sound::Playing) {
        chestBreakSound.play();
    }
}
void playPotionDrinkSound() {
    potionDrinkSound.play();
}
void playCoinSound() {
    if (coinSound.getStatus() != Sound::Playing) {
        coinSound.play();
    }
}
void setSpawnRate() {
    float timeMinutes = (float)time() / 60.0f;
    float waveTimer = 2; // in minutes
    SpawnDelay = 0.5;
    while (timeMinutes >= waveTimer) {
        timeMinutes -= waveTimer;
        SpawnDelay *= 0.75;
    }
    if (SpawnDelay < MinSpawnDelay) {
        SpawnDelay = MinSpawnDelay;
    }
}
void ringInitialization() {
    ring.sprite.setTexture(ringTexture);
    ring.sprite.setOrigin(34, 34); // Center the sprite (half of 68x68 frame)
    ring.sprite.setTextureRect(IntRect(0, 0, 68, 68)); // First frame

}
void ringAnimation(float deltaTime) {
    ring.animationTimer += deltaTime;
    if (ring.animationTimer >= 0.1f) { // Switch frame every 0.1 seconds
        ring.animationTimer = 0.0f;
        ring.frameIndex = (ring.frameIndex + 1) % 4; // 4 frames in the sprite sheet
        ring.sprite.setTextureRect(IntRect(0, ring.frameIndex * 68, 68, 68));
    }
}
void ringRotation(float deltaTime, const Vector2f& playerPos) {
    ring.angle += ring.rotationSpeed * deltaTime;
    if (ring.angle >= 360.0f)
    {
        ring.angle -= 360.0f;
    }

    float radians = ring.angle * (3.14159f / 180.0f); // Convert to radians
    ring.sprite.setPosition(
        playerPos.x + cos(radians) * ring.radius,
        playerPos.y + sin(radians) * ring.radius
    );
}
void ringCollision() {
    for (auto& zombie : Zombies) {
        if (ring.sprite.getGlobalBounds().intersects(zombie.Shape.getGlobalBounds())) {
            if (zombie.canBeHit) {
                zombie.HP -= player[character].ringDamage; // Use whip damage for the ring aashan ana mkasl (fuck u)
                zombie.canBeHit = false;
                zombie.lastHitTime = 0.0f;
                zombie.Shape.setColor(Color::Red); // Visual feedback
            }
        }
    }
}
void updateRing() {
    if (!player[character].isRingUnlocked)
        return; // Only update if the ring is unlocked for the current character

    Vector2f playerPos = player[character].sprite.getPosition();

    // Update ring components
    ringAnimation(deltaTime);
    ringRotation(deltaTime, playerPos);
    ringCollision();
}
void goreMenuInitialization() {
    yesButton.sprite.setPosition(1920 / 2.5 - 120, 750);
    noButton.sprite.setPosition(1920 / 1.5 - 120, 750);
}
void goreMenuHandler() {
    if (yesButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        yesButton.sprite.setTextureRect(IntRect(0, 108, 222, 108));
        yesButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            bossSound.play();
            menu = 0;
            gameMode = 1;
            menuBG[0].sprite.setTexture(menuBG[0].horrorTexture);
            mainmenuWidgets();
            sleep(milliseconds(200));
        }
    }
    else
    {
        yesButton.sprite.setTextureRect(IntRect(0, 0, 222, 108));
        yesButton.sprite.setColor(Color::White);
    }

    if (noButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        noButton.sprite.setTextureRect(IntRect(0, 108, 222, 108));
        noButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            clickSound.play();
            menu = 0;
            mainmenuWidgets();
            sleep(milliseconds(200));
        }
    }
    else
    {
        noButton.sprite.setTextureRect(IntRect(0, 0, 222, 108));
        noButton.sprite.setColor(Color::White);
    }
}
void backgroundInitialization() {
    if (gameMode == 0) { // normal mode
        background.setTexture(backgroundTexture);
    }
    else if (gameMode == 1) { // horror
        background.setTexture(backgroundHorrorTexture);
    }
}
void drawPowerUpsMenu()
{
    window.draw(menuBG[3].sprite);
    for (int i = 0; i < 3; i++)
    {
        window.draw(allUpgrades[upgradesIndices[i]].sprite);
        window.draw(allUpgrades[upgradesIndices[i]].text);
        window.draw(currentLevelText[upgradesIndices[i]]);
        window.draw(descriptionText[upgradesIndices[i]]);
    }
}
void powerUps()
{
    static bool wasMousePressed = false;  // Track previous mouse state

    for (int j = 0; j < 3; j++)
    {
        int i = upgradesIndices[j];

        allUpgrades[i].sprite.setScale(8, 8);

        // Skip if this upgrade has reached max level
        if (currentUpgradelvl[i] >= upgradeLevel.upgradeMaxLevel)
        {
            continue;
        }
        // Check if the mouse is over the upgrade sprite
        if (allUpgrades[i].sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
        {

            allUpgrades[i].sprite.setScale(7, 7);

            // Check for mouse button release (not press) to ensure one click = one upgrade
            bool isMousePressed = Mouse::isButtonPressed(Mouse::Left);
            if (isMousePressed && !wasMousePressed)  // Only trigger on new press
            {
                clickSound.play();
                //cout << "Clicked upgrade index: " << i << endl;
                switch (i)
                {
                case 0:
                    // Upgrade 0: Increase Max Health
                    if (upgradeLevel.maxHealthLevel < upgradeLevel.upgradeMaxLevel)
                    {
                        //cout << "max before:  " << upgradeLevel.maxHealthLevel << endl;
                        maxHealthIncrease(10);
                        upgradeLevel.maxHealthLevel++;
                        currentUpgradelvl[0] = upgradeLevel.maxHealthLevel;
                        //cout << "max after:  " << upgradeLevel.maxHealthLevel << endl;
                    }
                    break;

                case 1:
                    // Upgrade 1: Defense upgrade
                    if (upgradeLevel.defenseUpgradeLevel < upgradeLevel.upgradeMaxLevel)
                    {
                        //cout << "defense before:  " << upgradeLevel.defenseUpgradeLevel << endl;
                        upgradeLevel.defenseUpgradeLevel++;
                        currentUpgradelvl[1] = upgradeLevel.defenseUpgradeLevel;
                        //cout << "defense after:  " << upgradeLevel.defenseUpgradeLevel << endl;
                    }
                    break;

                case 2:
                    // Upgrade 2: Upgrade whip
                    if (upgradeLevel.whipLvlReal < upgradeLevel.upgradeMaxLevel)
                    {
                        //cout << "whip before:  " << upgradeLevel.whipLvlReal << endl;
                        upgradeLevel.whipLvl = 2;
                        if (upgradeLevel.whipLvlReal > 1)
                        {
                            player[character].whipDamage += 30;
                        }
                        upgradeLevel.whipLvlReal++;
                        currentUpgradelvl[2] = upgradeLevel.whipLvlReal;
                        //cout << "whip after:  " << upgradeLevel.whipLvlReal << endl;
                    }
                    break;

                case 3:
                    // Upgrade 3: Health regeneration
                    if (upgradeLevel.healingUpgradeLevel < upgradeLevel.upgradeMaxLevel)
                    {
                        //cout << "regen before:  " << upgradeLevel.healingUpgradeLevel << endl;
                        upgradeLevel.healingUpgradeLevel++;
                        currentUpgradelvl[3] = upgradeLevel.healingUpgradeLevel;
                        //cout << "regen after:  " << upgradeLevel.healingUpgradeLevel << endl;
                    }
                    break;

                case 4:
                    // Upgrade 4: Unlock second weapon (projectile)
                    if (upgradeLevel.secondWeaponUpgradeLvl < upgradeLevel.upgradeMaxLevel)
                    {
                        //cout << "second before:  " << upgradeLevel.secondWeaponUpgradeLvl << endl;
                        player[character].isProjectileUnlocked = true;
                        if (upgradeLevel.secondWeaponUpgradeLvl > 1)
                        {
                            player[character].ProjectileDamage += 5;
                            player[character].projectileCooldown = 2.0 * pow(1.0 - 0.50, upgradeLevel.secondWeaponUpgradeLvl - 1);
                        }
                        upgradeLevel.secondWeaponUpgradeLvl++;
                        currentUpgradelvl[4] = upgradeLevel.secondWeaponUpgradeLvl;
                        //cout << "second after:  " << upgradeLevel.secondWeaponUpgradeLvl << endl;
                    }
                    break;

                case 5:
                    // Upgrade 5: Increase player speed
                    if (upgradeLevel.increasePlayerSpeedLevel < upgradeLevel.upgradeMaxLevel)
                    {
                        //cout << "speed before:  " << upgradeLevel.increasePlayerSpeedLevel << endl;
                        increasePlayerSpeed(30);
                        upgradeLevel.increasePlayerSpeedLevel++;
                        currentUpgradelvl[5] = upgradeLevel.increasePlayerSpeedLevel;
                        //cout << "speed after:  " << upgradeLevel.increasePlayerSpeedLevel << endl;
                    }
                    break;

                case 6:
                    // Upgrade 6: Unlock Ring
                    if (upgradeLevel.ringUpgradeLevel < upgradeLevel.upgradeMaxLevel)
                    {
                        //cout << "ring before:  " << upgradeLevel.ringUpgradeLevel << endl;
                        player[character].isRingUnlocked = true;
                        if (upgradeLevel.ringUpgradeLevel > 1)
                        {
                            ring.rotationSpeed += 100;
                            player[character].ringDamage += 5;
                        }
                        upgradeLevel.ringUpgradeLevel++;
                        currentUpgradelvl[6] = upgradeLevel.ringUpgradeLevel;
                        //cout << "ring after:  " << upgradeLevel.ringUpgradeLevel << endl;
                    }
                    break;
                }

                // After any upgrade is clicked, we stop the upgrade screen
                isUpgrading = false;

                // Prevent checking more upgrades only one can be selected per click
                break;
            }
        }
    }

    wasMousePressed = Mouse::isButtonPressed(Mouse::Left);  // Update previous mouse state
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
    if (isMenuOpen)
    {
        maxLevelChecker();
        for (int i = 0; i < 3; i++)
        {
            allUpgrades[upgradesIndices[i]].sprite.setPosition((1920 / 2) + offSetX[i], 1080 / 2);
        }
        upgradesTextHandeling();
        upgradeLevelText();
        upgradeDescriptionHandeling();
        isMenuOpen = false;
    }
}
void maxHealthIncrease(int addValue)
{
    player[character].maxHealth += addValue;
}
void increasePlayerSpeed(int value)
{
    player[character].speed += value;
}
void upgradesMenuText()
{
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
    allUpgrades[6].text.setString("Ring of Death");
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

    allUpgrades[6].text.setPosition(allUpgrades[6].sprite.getPosition().x - 210, allUpgrades[6].sprite.getPosition().y + 270);
    allUpgrades[6].text.setCharacterSize(85);
}
void maxLevelChecker()
{
    int validIndex = 0;
    int validCount = 0;
    fill(begin(validUpgrades), end(validUpgrades), 0);     //??? Adam

    for (int i = 0; i < UPGRADES_NUM; i++)
    {
        if (currentUpgradelvl[i] < upgradeLevel.upgradeMaxLevel)
        {
            validUpgrades[validIndex] = i;
            validIndex++;
        }
    }
    validCount = validIndex;
    if (validCount == 2)        //could crash
    {
        validCount = 3;
        validUpgrades[2] = rand() % 6;
        allUpgrades[validUpgrades[2]].sprite.setColor(Color::Black);
    }
    for (int i = 0; i < 3; i++)
    {
        validIndex = rand() % validCount;
        upgradesIndices[i] = validUpgrades[validIndex];
    }
    while (upgradesIndices[0] == upgradesIndices[1])
    {
        validIndex = rand() % validCount;
        upgradesIndices[1] = validUpgrades[validIndex];
    }
    while (upgradesIndices[0] == upgradesIndices[2] || upgradesIndices[1] == upgradesIndices[2])
    {
        validIndex = rand() % validCount;
        upgradesIndices[2] = validUpgrades[validIndex];
    }



}
void upgradeLevelText()
{
    for (int i = 0; i < UPGRADES_NUM; i++)
    {
        currentLevelText[i].setString("Upgrade Level: " + to_string(currentUpgradelvl[i]));
        currentLevelText[i].setPosition(allUpgrades[i].sprite.getPosition().x - 210, allUpgrades[i].sprite.getPosition().y - 240);
    }
}
void upgradeDescriptionText()
{
    for (int i = 0; i < UPGRADES_NUM; i++)
    {
        descriptionText[i].setFont(font);
        descriptionText[i].setFillColor(Color::Black);
        descriptionText[i].setOutlineColor(Color::White);
        descriptionText[i].setOutlineThickness(1);
    }
    upgradeDescriptionNames();
}
void upgradeDescriptionNames()
{
    descriptionText[0].setString("Boosts max health");
    descriptionText[1].setString("Less damage");
    descriptionText[2].setString("Dual strike");
    descriptionText[3].setString("Heal over time");
    descriptionText[4].setString("Upgrades projectile");
    descriptionText[5].setString("Increases speed");
    descriptionText[6].setString("Upgrades ring");
}
void upgradeDescriptionHandeling()
{
    descriptionText[0].setPosition(allUpgrades[0].sprite.getPosition().x - 210, allUpgrades[0].sprite.getPosition().y + 160);
    descriptionText[0].setCharacterSize(60);

    descriptionText[1].setPosition(allUpgrades[1].sprite.getPosition().x - 130, allUpgrades[1].sprite.getPosition().y + 160);
    descriptionText[1].setCharacterSize(60);

    descriptionText[2].setPosition(allUpgrades[2].sprite.getPosition().x - 130, allUpgrades[2].sprite.getPosition().y + 160);
    descriptionText[2].setCharacterSize(60);

    descriptionText[3].setPosition(allUpgrades[3].sprite.getPosition().x - 160, allUpgrades[3].sprite.getPosition().y + 160);
    descriptionText[3].setCharacterSize(60);

    descriptionText[4].setPosition(allUpgrades[4].sprite.getPosition().x - 225, allUpgrades[4].sprite.getPosition().y + 160);
    descriptionText[4].setCharacterSize(60);

    descriptionText[5].setPosition(allUpgrades[5].sprite.getPosition().x - 175, allUpgrades[5].sprite.getPosition().y + 160);
    descriptionText[5].setCharacterSize(60);

    descriptionText[6].setPosition(allUpgrades[6].sprite.getPosition().x - 145, allUpgrades[6].sprite.getPosition().y + 160);
    descriptionText[6].setCharacterSize(60);
}
void notValidUpgradeRemover() {
    for (int i = 0; i < UPGRADES_NUM; i++) {
        if (i != upgradesIndices[0] && i != upgradesIndices[1] && i != upgradesIndices[2]) {
            allUpgrades[i].sprite.setPosition(0, 0);
        }
    }

}
void optionsWidgets()
{
    if (gameMode == 0) {
        if (backgroundMusic.getVolume() > 0) {
            volumeButton.sprite.setTexture(volumeButton.texture);
        }
        else {
            volumeButton.sprite.setTexture(volumeButton.secondForm);
        }

        sliderButton.sprite.setTexture(sliderButton.texture);
        wasd.setTexture(wasdTexture);
        arrows.setTexture(arrowsTexture);
        selectButton.sprite.setTexture(selectButton.texture);
        menuBG[6].sprite.setTexture(menuBG[6].texture);
    }
    else if (gameMode == 1) {
        if (backgroundMusic.getVolume() > 0) {
            volumeButton.sprite.setTexture(volumeButton.horrorTexture);
        }
        else {
            volumeButton.sprite.setTexture(volumeButton.horrorSecondForm);
        }

        sliderButton.sprite.setTexture(sliderButton.horrorTexture);
        wasd.setTexture(wasdHorrorTexture);
        arrows.setTexture(arrowsHorrorTexture);
        selectButton.sprite.setTexture(selectButton.horrorTexture);
        menuBG[6].sprite.setTexture(menuBG[6].horrorTexture);
    }

    volumeButton.sprite.setPosition(1920 / 3.4, 320);
    slider.setPosition(1920 / 2.5, 360);
    slider.setScale(0.87, 1);
    sliderButton.sprite.setPosition(1920 / 2.5 + volume * 5, 350);
    sliderButton.sprite.setScale(2, 2);
    wasd.setPosition(1920 / 1.8, 500);
    arrows.setPosition(1920 / 3.8, 500);

    if (controls == 1) {
        selectButton.sprite.setPosition(1920 / 3.45, 800);
    }
    else if (controls == 0) {
        selectButton.sprite.setPosition(1920 / 1.7, 800);
    }
}
void optionsHandler()
{
    // Handle volume button toggle
    if (volumeButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
    {
        volumeButton.sprite.setTextureRect(IntRect(0, 108, 102, 108));
        volumeButton.sprite.setColor(sf::Color(200, 200, 200));
        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (backgroundMusic.getVolume() > 0) {
                volume = 0;
                sliderButton.sprite.setPosition(1920 / 2.5, 350);
                if (gameMode == 0) {
                    volumeButton.sprite.setTexture(volumeButton.secondForm);
                }
                else if (gameMode == 1) {
                    volumeButton.sprite.setTexture(volumeButton.horrorSecondForm);
                }
            }
            else {
                volume = 100;
                sliderButton.sprite.setPosition(1920 / 2.5 + 500, 350);
                if (gameMode == 0) {
                    volumeButton.sprite.setTexture(volumeButton.texture);
                }
                else if (gameMode == 1) {
                    volumeButton.sprite.setTexture(volumeButton.horrorTexture);
                }
            }
            clickSound.play();
            // Update all sound volumes
            clickSound.setVolume(volume);
            deathSound.setVolume(volume);
            gameOverSound.setVolume(volume);
            footstepsSound.setVolume(volume);
            enemyAppearSound.setVolume(volume);
            enemyHitSound.setVolume(volume);
            playerHitSound.setVolume(volume);
            healthSound.setVolume(volume);
            bossSound.setVolume(volume);
            coinSound.setVolume(volume);
            levelCompleteSound.setVolume(volume);
            chestBreakSound.setVolume(volume);
            potionDrinkSound.setVolume(volume);
            backgroundMusic.setVolume(volume);
            normalLevelMusic.setVolume(volume);
            goreModeMusic.setVolume(volume);
            sleep(milliseconds(200));
        }
    }
    else
    {
        volumeButton.sprite.setTextureRect(IntRect(0, 0, 102, 108));
        volumeButton.sprite.setColor(Color::White);
    }

    // Handle slider movement
    static bool isDragging = false; // Track if the slider is being dragged

    if (sliderButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y) || isDragging)
    {
        sliderButton.sprite.setTextureRect(IntRect(0, 16, 15, 16));
        sliderButton.sprite.setColor(sf::Color(200, 200, 200));

        if (Mouse::isButtonPressed(Mouse::Left)) {
            isDragging = true; // Start dragging

            // Update slider position based on mouse position
            float mouseX = Mouse::getPosition(window).x;
            float sliderMinX = 1920 / 2.5; // Minimum slider position
            float sliderMaxX = 1920 / 2.5 + 500; // Maximum slider position

            // Clamp the slider position within bounds
            if (mouseX < sliderMinX) mouseX = sliderMinX;
            if (mouseX > sliderMaxX) mouseX = sliderMaxX;

            sliderButton.sprite.setPosition(mouseX, 350);

            // Calculate volume based on slider position
            volume = (mouseX - sliderMinX) / 5;

            // Update all sound volumes
            clickSound.setVolume(volume);
            deathSound.setVolume(volume);
            gameOverSound.setVolume(volume);
            footstepsSound.setVolume(volume);
            enemyAppearSound.setVolume(volume);
            enemyHitSound.setVolume(volume);
            playerHitSound.setVolume(volume);
            healthSound.setVolume(volume);
            bossSound.setVolume(volume);
            coinSound.setVolume(volume);
            levelCompleteSound.setVolume(volume);
            chestBreakSound.setVolume(volume);
            potionDrinkSound.setVolume(volume);
            backgroundMusic.setVolume(volume);
            normalLevelMusic.setVolume(volume);
            goreModeMusic.setVolume(volume);

            // Update volume button texture based on volume
            if (volume == 0)
            {

                if (gameMode == 0) {
                    volumeButton.sprite.setTexture(volumeButton.secondForm);
                }
                else if (gameMode == 1) {
                    volumeButton.sprite.setTexture(volumeButton.horrorSecondForm);
                }
            }
            else
            {

                if (gameMode == 0) {
                    volumeButton.sprite.setTexture(volumeButton.texture);
                }
                else if (gameMode == 1) {
                    volumeButton.sprite.setTexture(volumeButton.horrorTexture);
                }
            }
        }
        else {
            isDragging = false; // Stop dragging when the mouse button is released
        }
    }
    else
    {
        sliderButton.sprite.setTextureRect(IntRect(0, 0, 15, 16));
        sliderButton.sprite.setColor(Color::White);
    }

    // Handle WASD/Arrow toggle
    selectButtonHandler();
}
void loadChestAndOrb() {
    srand(static_cast<unsigned>(time(nullptr)));
    // Random position chest
    float x1 = static_cast<float>(rand() % 7680);
    float y1 = static_cast<float>(rand() % 4320);
    float x2 = static_cast<float>(rand() % 7680);
    float y2 = static_cast<float>(rand() % 4320);
    float x3 = static_cast<float>(rand() % 7680);
    float y3 = static_cast<float>(rand() % 4320);

    chest1.sprite.setPosition(x1, y1);
    chest2.sprite.setPosition(x2, y2);
    chest3.sprite.setPosition(x3, y3);
    Vector2f chest1position = chest1.sprite.getPosition();
    Vector2f chest2position = chest2.sprite.getPosition();
    Vector2f chest3position = chest3.sprite.getPosition();
    Vector2f potion1position(x1 + 50, y1);
    Vector2f potion2position(x2 + 50, y2);
    Vector2f potion3position(x3 + 50, y3);
    chest1.itemsprite.setPosition(potion1position);
    chest2.itemsprite.setPosition(potion2position);
    chest3.itemsprite.setPosition(potion3position);
}
void updateChestLogic() {//logy
    // Break chest
    // chest 1

    if (!chest1.chestbroken && whipHitbox.getGlobalBounds().intersects(chest1.sprite.getGlobalBounds())) {
        playChestBreakSound();
        chest1.chestbroken = true;
    }

    if (chest1.chestbroken && !chest1.contentcollected) {
        if (player[character].sprite.getGlobalBounds().intersects(chest1.itemsprite.getGlobalBounds())) {
            if (chest1.type == health) {
                playPotionDrinkSound();
                heal(healthBuff);
            }
            else if (chest1.type == coin) {
                playPotionDrinkSound();
                coins += 100;
            }
            else if (chest1.type == magnet)
            {
                playPotionDrinkSound();
                hasmagnet = true;
                orbdirection();

            }
            chest1.contentcollected = true;
            updateposition1();
            chest1.chestbroken = false;
            chest1.contentcollected = false;
        }
    }

    // chest 2
    if (!chest2.chestbroken && whipHitbox.getGlobalBounds().intersects(chest2.sprite.getGlobalBounds())) {
        playChestBreakSound();
        chest2.chestbroken = true;
    }

    if (chest2.chestbroken && !chest2.contentcollected) {
        if (player[character].sprite.getGlobalBounds().intersects(chest2.itemsprite.getGlobalBounds())) {
            if (chest2.type == health) {
                playPotionDrinkSound();
                heal(healthBuff);

            }
            else if (chest2.type == coin) {
                playPotionDrinkSound();
                coins += 100;
            }
            else if (chest2.type == magnet)
            {
                playPotionDrinkSound();
                hasmagnet = true;
                orbdirection();
            }

            chest2.contentcollected = true;
            updateposition2();
            chest2.chestbroken = false;
            chest2.contentcollected = false;
        }
    }

    // chest 3 
    if (!chest3.chestbroken && whipHitbox.getGlobalBounds().intersects(chest3.sprite.getGlobalBounds())) {
        playChestBreakSound();
        chest3.chestbroken = true;
    }

    if (chest3.chestbroken && !chest3.contentcollected) {
        if (player[character].sprite.getGlobalBounds().intersects(chest3.itemsprite.getGlobalBounds())) {
            if (chest3.type == health) {
                playPotionDrinkSound();
                heal(healthBuff);
            }
            else if (chest3.type == coin) {
                playPotionDrinkSound();
                coins += 10;
            }
            else if (chest3.type == magnet)
            {
                playPotionDrinkSound();
                hasmagnet = true;
                orbdirection();
            }

            chest3.contentcollected = true;
            updateposition3();
            chest3.chestbroken = false;
            chest3.contentcollected = false;
        }
    }
}
void coinAnimation() {
    coinAnimationTimer += deltaTime;



    if (coinAnimationTimer >= COIN_ANIMATION_RATE)
    {
        coinAnimationTimer = 0;
        coinIndx = (coinIndx + 1) % 8;
    }
    chest2.itemsprite.setTextureRect(IntRect(0, coinIndx * 18, 12, 18));
}
void updateposition1() //logy
{

    float x1 = static_cast<float>(rand() % 7680);
    float y1 = static_cast<float>(rand() % 4320);
    chest1.sprite.setPosition(x1, y1);
    Vector2f chest1position = chest1.sprite.getPosition();
    Vector2f potion1position(x1 + 50, y1);
    chest1.itemsprite.setPosition(potion1position);
}
void updateposition2()
{
    float x2 = static_cast<float>(rand() % 7680);
    float y2 = static_cast<float>(rand() % 4320);
    chest2.sprite.setPosition(x2, y2);
    Vector2f chest2position = chest2.sprite.getPosition();
    Vector2f potion2position(x2 + 50, y2);
    chest2.itemsprite.setPosition(potion2position);

}
void updateposition3()
{
    float x3 = static_cast<float>(rand() % 7680);
    float y3 = static_cast<float>(rand() % 4320);
    chest3.sprite.setPosition(x3, y3);
    Vector2f chest3position = chest3.sprite.getPosition();
    Vector2f potion3position(x3 + 50, y3);
    chest3.itemsprite.setPosition(potion3position);

}
void orbdirection()
{
    if (hasmagnet) {
        for (int i = 0; i < xpOrbs.size(); i++)
        {
            xpOrbs[i].sprite.setPosition(player[character].sprite.getPosition());
        }
    }
}
void fontStatsInitalizer()
{

    playerSpeedStats.setFont(font);
    playerHealthStats.setFont(font);
    whipDmgStats.setFont(font);
    ringBaseDmgStats.setFont(font);
    whipCooldownStats.setFont(font);
    projectileDmgStats.setFont(font);
    projectileCooldownStats.setFont(font);
    playerStartingWeapon.setFont(font);

}
void DefaultPlayerStats()
{
    if (menu == 2 && shopPage == 0)
    {
        //Speed
        playerSpeedStats.setCharacterSize(35);
        playerSpeedStats.setFillColor(Color::White);
        playerSpeedStats.setPosition(1100, 300);
        playerSpeedStats.setString("Speed: ");
        window.draw(playerSpeedStats);
        playerSpeedStats.setFillColor(Color(255, 165, 0));
        playerSpeedStats.setPosition(1450, 300);
        playerSpeedStats.setString(to_string((int)player[0].speed));
        window.draw(playerSpeedStats);

        //Health
        playerHealthStats.setCharacterSize(35);
        playerHealthStats.setFillColor(Color::White);
        playerHealthStats.setPosition(1100, 350);
        playerHealthStats.setString("Health: ");
        window.draw(playerHealthStats);
        playerHealthStats.setFillColor(Color(255, 165, 0));
        playerHealthStats.setPosition(1450, 350);
        playerHealthStats.setString(to_string((int)player[0].maxHealth));
        window.draw(playerHealthStats);

        //Whip dmg
        whipDmgStats.setCharacterSize(35);
        whipDmgStats.setFillColor(Color::White);
        whipDmgStats.setPosition(1100, 400);
        whipDmgStats.setString("Whip Damage: ");
        window.draw(whipDmgStats);
        whipDmgStats.setFillColor(Color(255, 165, 0));
        whipDmgStats.setPosition(1450, 400);
        whipDmgStats.setString(to_string((int)player[0].whipDamage));
        window.draw(whipDmgStats);

        //Whip Cooldown
        whipCooldownStats.setCharacterSize(35);
        whipCooldownStats.setFillColor(Color::White);
        whipCooldownStats.setPosition(1100, 450);
        whipCooldownStats.setString("Whip Cooldown: ");
        window.draw(whipCooldownStats);
        whipCooldownStats.setFillColor(Color(255, 165, 0));
        whipCooldownStats.setPosition(1450, 450);
        whipCooldownStats.setString(to_string((int)player[0].whipCooldown));
        window.draw(whipCooldownStats);

        //Ring dmg
        ringBaseDmgStats.setCharacterSize(35);
        ringBaseDmgStats.setFillColor(Color::White);
        ringBaseDmgStats.setPosition(1100, 500);
        ringBaseDmgStats.setString("Ring Damage: ");
        window.draw(ringBaseDmgStats);
        ringBaseDmgStats.setFillColor(Color(255, 165, 0));
        ringBaseDmgStats.setPosition(1450, 500);
        ringBaseDmgStats.setString(to_string((int)player[0].ringDamage));
        window.draw(ringBaseDmgStats);

        //Projectile Dmg
        projectileDmgStats.setCharacterSize(35);
        projectileDmgStats.setFillColor(Color::White);
        projectileDmgStats.setPosition(1100, 550);
        projectileDmgStats.setString("Projectile Damage: ");
        window.draw(projectileDmgStats);
        projectileDmgStats.setFillColor(Color(255, 165, 0));
        projectileDmgStats.setPosition(1450, 550);
        projectileDmgStats.setString(to_string((int)player[0].ProjectileDamage));
        window.draw(projectileDmgStats);

        //Projectile Cooldown
        projectileCooldownStats.setCharacterSize(35);
        projectileCooldownStats.setFillColor(Color::White);
        projectileCooldownStats.setPosition(1100, 600);
        projectileCooldownStats.setString("Projectile Cooldown: ");
        window.draw(projectileCooldownStats);
        projectileCooldownStats.setFillColor(Color(255, 165, 0));
        projectileCooldownStats.setPosition(1450, 600);
        projectileCooldownStats.setString(to_string((int)player[0].projectileCooldown));
        window.draw(projectileCooldownStats);

        //Starting Weapon
        playerStartingWeapon.setCharacterSize(35);
        playerStartingWeapon.setFillColor(Color::White);
        playerStartingWeapon.setPosition(1100, 650);
        playerStartingWeapon.setString("Starting Weapon: ");
        window.draw(playerStartingWeapon);
        playerStartingWeapon.setFillColor(Color(255, 165, 0));
        playerStartingWeapon.setPosition(1450, 650);
        playerStartingWeapon.setString(string(player[0].isWhipUnlocked ? "Whip" : "Pistol"));
        window.draw(playerStartingWeapon);
    }
}
void playerOneStats()
{
    if (menu == 2 && shopPage == 1)
    {
        //Speed
        playerSpeedStats.setCharacterSize(35);
        playerSpeedStats.setFillColor(Color::White);
        playerSpeedStats.setPosition(1100, 300);
        playerSpeedStats.setString("Speed: ");
        window.draw(playerSpeedStats);
        playerSpeedStats.setFillColor(Color(255, 165, 0));
        playerSpeedStats.setPosition(1450, 300);
        playerSpeedStats.setString(to_string((int)player[1].speed));
        window.draw(playerSpeedStats);

        //Health
        playerHealthStats.setCharacterSize(35);
        playerHealthStats.setFillColor(Color::White);
        playerHealthStats.setPosition(1100, 350);
        playerHealthStats.setString("Health: ");
        window.draw(playerHealthStats);
        playerHealthStats.setFillColor(Color(255, 165, 0));
        playerHealthStats.setPosition(1450, 350);
        playerHealthStats.setString(to_string((int)player[1].maxHealth));
        window.draw(playerHealthStats);

        //Whip dmg
        whipDmgStats.setCharacterSize(35);
        whipDmgStats.setFillColor(Color::White);
        whipDmgStats.setPosition(1100, 400);
        whipDmgStats.setString("Whip Damage: ");
        window.draw(whipDmgStats);
        whipDmgStats.setFillColor(Color(255, 165, 0));
        whipDmgStats.setPosition(1450, 400);
        whipDmgStats.setString(to_string((int)player[1].whipDamage));
        window.draw(whipDmgStats);

        //Whip Cooldown
        whipCooldownStats.setCharacterSize(35);
        whipCooldownStats.setFillColor(Color::White);
        whipCooldownStats.setPosition(1100, 450);
        whipCooldownStats.setString("Whip Cooldown: ");
        window.draw(whipCooldownStats);
        whipCooldownStats.setFillColor(Color(255, 165, 0));
        whipCooldownStats.setPosition(1450, 450);
        whipCooldownStats.setString(to_string((int)player[1].whipCooldown));
        window.draw(whipCooldownStats);

        //Ring dmg
        ringBaseDmgStats.setCharacterSize(35);
        ringBaseDmgStats.setFillColor(Color::White);
        ringBaseDmgStats.setPosition(1100, 500);
        ringBaseDmgStats.setString("Ring Damage: ");
        window.draw(ringBaseDmgStats);
        ringBaseDmgStats.setFillColor(Color(255, 165, 0));
        ringBaseDmgStats.setPosition(1450, 500);
        ringBaseDmgStats.setString(to_string((int)player[1].ringDamage));
        window.draw(ringBaseDmgStats);

        //Projectile Dmg
        projectileDmgStats.setCharacterSize(35);
        projectileDmgStats.setFillColor(Color::White);
        projectileDmgStats.setPosition(1100, 550);
        projectileDmgStats.setString("Projectile Damage: ");
        window.draw(projectileDmgStats);
        projectileDmgStats.setFillColor(Color(255, 165, 0));
        projectileDmgStats.setPosition(1450, 550);
        projectileDmgStats.setString(to_string((int)player[1].ProjectileDamage));
        window.draw(projectileDmgStats);

        //Projectile Cooldown
        projectileCooldownStats.setCharacterSize(35);
        projectileCooldownStats.setFillColor(Color::White);
        projectileCooldownStats.setPosition(1100, 600);
        projectileCooldownStats.setString("Projectile Cooldown: ");
        window.draw(projectileCooldownStats);
        projectileCooldownStats.setFillColor(Color(255, 165, 0));
        projectileCooldownStats.setPosition(1450, 600);
        projectileCooldownStats.setString(to_string((int)player[1].projectileCooldown));
        window.draw(projectileCooldownStats);

        //Starting Weapon
        playerStartingWeapon.setCharacterSize(35);
        playerStartingWeapon.setFillColor(Color::White);
        playerStartingWeapon.setPosition(1100, 650);
        playerStartingWeapon.setString("Starting Weapon: ");
        window.draw(playerStartingWeapon);
        playerStartingWeapon.setFillColor(Color(255, 165, 0));
        playerStartingWeapon.setPosition(1450, 650);
        playerStartingWeapon.setString(string(player[1].isWhipUnlocked ? "Whip" : "Pistol"));
        window.draw(playerStartingWeapon);
    }
}
void playerTwoStats()
{
    if (menu == 2 && shopPage == 2)
    {
        //Speed
        playerSpeedStats.setCharacterSize(35);
        playerSpeedStats.setFillColor(Color::White);
        playerSpeedStats.setPosition(1100, 300);
        playerSpeedStats.setString("Speed: ");
        window.draw(playerSpeedStats);
        playerSpeedStats.setFillColor(Color(255, 165, 0));
        playerSpeedStats.setPosition(1450, 300);
        playerSpeedStats.setString(to_string((int)player[2].speed));
        window.draw(playerSpeedStats);

        //Health
        playerHealthStats.setCharacterSize(35);
        playerHealthStats.setFillColor(Color::White);
        playerHealthStats.setPosition(1100, 350);
        playerHealthStats.setString("Health: ");
        window.draw(playerHealthStats);
        playerHealthStats.setFillColor(Color(255, 165, 0));
        playerHealthStats.setPosition(1450, 350);
        playerHealthStats.setString(to_string((int)player[2].maxHealth));
        window.draw(playerHealthStats);

        //Whip dmg
        whipDmgStats.setCharacterSize(35);
        whipDmgStats.setFillColor(Color::White);
        whipDmgStats.setPosition(1100, 400);
        whipDmgStats.setString("Whip Damage: ");
        window.draw(whipDmgStats);
        whipDmgStats.setFillColor(Color(255, 165, 0));
        whipDmgStats.setPosition(1450, 400);
        whipDmgStats.setString(to_string((int)player[2].whipDamage));
        window.draw(whipDmgStats);

        //Whip Cooldown
        whipCooldownStats.setCharacterSize(35);
        whipCooldownStats.setFillColor(Color::White);
        whipCooldownStats.setPosition(1100, 450);
        whipCooldownStats.setString("Whip Cooldown: ");
        window.draw(whipCooldownStats);
        whipCooldownStats.setFillColor(Color(255, 165, 0));
        whipCooldownStats.setPosition(1450, 450);
        whipCooldownStats.setString(to_string((int)player[2].whipCooldown));
        window.draw(whipCooldownStats);

        //Ring dmg
        ringBaseDmgStats.setCharacterSize(35);
        ringBaseDmgStats.setFillColor(Color::White);
        ringBaseDmgStats.setPosition(1100, 500);
        ringBaseDmgStats.setString("Ring Damage: ");
        window.draw(ringBaseDmgStats);
        ringBaseDmgStats.setFillColor(Color(255, 165, 0));
        ringBaseDmgStats.setPosition(1450, 500);
        ringBaseDmgStats.setString(to_string((int)player[2].ringDamage));
        window.draw(ringBaseDmgStats);

        //Projectile Dmg
        projectileDmgStats.setCharacterSize(35);
        projectileDmgStats.setFillColor(Color::White);
        projectileDmgStats.setPosition(1100, 550);
        projectileDmgStats.setString("Projectile Damage: ");
        window.draw(projectileDmgStats);
        projectileDmgStats.setFillColor(Color(255, 165, 0));
        projectileDmgStats.setPosition(1450, 550);
        projectileDmgStats.setString(to_string((int)player[2].ProjectileDamage));
        window.draw(projectileDmgStats);

        //Projectile Cooldown
        projectileCooldownStats.setCharacterSize(35);
        projectileCooldownStats.setFillColor(Color::White);
        projectileCooldownStats.setPosition(1100, 600);
        projectileCooldownStats.setString("Projectile Cooldown: ");
        window.draw(projectileCooldownStats);
        projectileCooldownStats.setFillColor(Color(255, 165, 0));
        projectileCooldownStats.setPosition(1450, 600);
        projectileCooldownStats.setString(to_string((int)player[2].projectileCooldown));
        window.draw(projectileCooldownStats);

        //Starting Weapon
        playerStartingWeapon.setCharacterSize(35);
        playerStartingWeapon.setFillColor(Color::White);
        playerStartingWeapon.setPosition(1100, 650);
        playerStartingWeapon.setString("Starting Weapon: ");
        window.draw(playerStartingWeapon);
        playerStartingWeapon.setFillColor(Color(255, 165, 0));
        playerStartingWeapon.setPosition(1450, 650);
        playerStartingWeapon.setString(string(player[2].isWhipUnlocked ? "Whip" : "Pistol"));
        window.draw(playerStartingWeapon);
    }
}
void playerThreeStats()
{
    if (menu == 2 && shopPage == 3)
    {
        //Speed
        playerSpeedStats.setCharacterSize(35);
        playerSpeedStats.setFillColor(Color::White);
        playerSpeedStats.setPosition(1100, 300);
        playerSpeedStats.setString("Speed: ");
        window.draw(playerSpeedStats);
        playerSpeedStats.setFillColor(Color(255, 165, 0));
        playerSpeedStats.setPosition(1450, 300);
        playerSpeedStats.setString(to_string((int)player[3].speed));
        window.draw(playerSpeedStats);

        //Health
        playerHealthStats.setCharacterSize(35);
        playerHealthStats.setFillColor(Color::White);
        playerHealthStats.setPosition(1100, 350);
        playerHealthStats.setString("Health: ");
        window.draw(playerHealthStats);
        playerHealthStats.setFillColor(Color(255, 165, 0));
        playerHealthStats.setPosition(1450, 350);
        playerHealthStats.setString(to_string((int)player[3].maxHealth));
        window.draw(playerHealthStats);

        //Whip dmg
        whipDmgStats.setCharacterSize(35);
        whipDmgStats.setFillColor(Color::White);
        whipDmgStats.setPosition(1100, 400);
        whipDmgStats.setString("Whip Damage: ");
        window.draw(whipDmgStats);
        whipDmgStats.setFillColor(Color(255, 165, 0));
        whipDmgStats.setPosition(1450, 400);
        whipDmgStats.setString(to_string((int)player[3].whipDamage));
        window.draw(whipDmgStats);

        //Whip Cooldown
        whipCooldownStats.setCharacterSize(35);
        whipCooldownStats.setFillColor(Color::White);
        whipCooldownStats.setPosition(1100, 450);
        whipCooldownStats.setString("Whip Cooldown: ");
        window.draw(whipCooldownStats);
        whipCooldownStats.setFillColor(Color(255, 165, 0));
        whipCooldownStats.setPosition(1450, 450);
        whipCooldownStats.setString(to_string((int)player[3].whipCooldown));
        window.draw(whipCooldownStats);

        //Ring dmg
        ringBaseDmgStats.setCharacterSize(35);
        ringBaseDmgStats.setFillColor(Color::White);
        ringBaseDmgStats.setPosition(1100, 500);
        ringBaseDmgStats.setString("Ring Damage: ");
        window.draw(ringBaseDmgStats);
        ringBaseDmgStats.setFillColor(Color(255, 165, 0));
        ringBaseDmgStats.setPosition(1450, 500);
        ringBaseDmgStats.setString(to_string((int)player[3].ringDamage));
        window.draw(ringBaseDmgStats);

        //Projectile Dmg
        projectileDmgStats.setCharacterSize(35);
        projectileDmgStats.setFillColor(Color::White);
        projectileDmgStats.setPosition(1100, 550);
        projectileDmgStats.setString("Projectile Damage: ");
        window.draw(projectileDmgStats);
        projectileDmgStats.setFillColor(Color(255, 165, 0));
        projectileDmgStats.setPosition(1450, 550);
        projectileDmgStats.setString(to_string((int)player[3].ProjectileDamage));
        window.draw(projectileDmgStats);

        //Projectile Cooldown
        projectileCooldownStats.setCharacterSize(35);
        projectileCooldownStats.setFillColor(Color::White);
        projectileCooldownStats.setPosition(1100, 600);
        projectileCooldownStats.setString("Projectile Cooldown: ");
        window.draw(projectileCooldownStats);
        projectileCooldownStats.setFillColor(Color(255, 165, 0));
        projectileCooldownStats.setPosition(1450, 600);
        projectileCooldownStats.setString(to_string((int)player[3].projectileCooldown));
        window.draw(projectileCooldownStats);

        //Starting Weapon
        playerStartingWeapon.setCharacterSize(35);
        playerStartingWeapon.setFillColor(Color::White);
        playerStartingWeapon.setPosition(1100, 650);
        playerStartingWeapon.setString("Starting Weapon: ");
        window.draw(playerStartingWeapon);
        playerStartingWeapon.setFillColor(Color(255, 165, 0));
        playerStartingWeapon.setPosition(1450, 650);
        playerStartingWeapon.setString(string(player[3].isWhipUnlocked ? "Whip" : "Pistol"));
        window.draw(playerStartingWeapon);
    }
}
void playerFourStats()
{
    if (menu == 2 && shopPage == 4)
    {
        //Speed
        playerSpeedStats.setCharacterSize(35);
        playerSpeedStats.setFillColor(Color::White);
        playerSpeedStats.setPosition(1100, 300);
        playerSpeedStats.setString("Speed: ");
        window.draw(playerSpeedStats);
        playerSpeedStats.setFillColor(Color(255, 165, 0));
        playerSpeedStats.setPosition(1450, 300);
        playerSpeedStats.setString(to_string((int)player[4].speed));
        window.draw(playerSpeedStats);

        //Health
        playerHealthStats.setCharacterSize(35);
        playerHealthStats.setFillColor(Color::White);
        playerHealthStats.setPosition(1100, 350);
        playerHealthStats.setString("Health: ");
        window.draw(playerHealthStats);
        playerHealthStats.setFillColor(Color(255, 165, 0));
        playerHealthStats.setPosition(1450, 350);
        playerHealthStats.setString(to_string((int)player[4].maxHealth));
        window.draw(playerHealthStats);

        //Whip dmg
        whipDmgStats.setCharacterSize(35);
        whipDmgStats.setFillColor(Color::White);
        whipDmgStats.setPosition(1100, 400);
        whipDmgStats.setString("Whip Damage: ");
        window.draw(whipDmgStats);
        whipDmgStats.setFillColor(Color(255, 165, 0));
        whipDmgStats.setPosition(1450, 400);
        whipDmgStats.setString(to_string((int)player[4].whipDamage));
        window.draw(whipDmgStats);

        //Whip Cooldown
        whipCooldownStats.setCharacterSize(35);
        whipCooldownStats.setFillColor(Color::White);
        whipCooldownStats.setPosition(1100, 450);
        whipCooldownStats.setString("Whip Cooldown: ");
        window.draw(whipCooldownStats);
        whipCooldownStats.setFillColor(Color(255, 165, 0));
        whipCooldownStats.setPosition(1450, 450);
        whipCooldownStats.setString(to_string((int)player[4].whipCooldown));
        window.draw(whipCooldownStats);

        //Ring dmg
        ringBaseDmgStats.setCharacterSize(35);
        ringBaseDmgStats.setFillColor(Color::White);
        ringBaseDmgStats.setPosition(1100, 500);
        ringBaseDmgStats.setString("Ring Damage: ");
        window.draw(ringBaseDmgStats);
        ringBaseDmgStats.setFillColor(Color(255, 165, 0));
        ringBaseDmgStats.setPosition(1450, 500);
        ringBaseDmgStats.setString(to_string((int)player[4].ringDamage));
        window.draw(ringBaseDmgStats);

        //Projectile Dmg
        projectileDmgStats.setCharacterSize(35);
        projectileDmgStats.setFillColor(Color::White);
        projectileDmgStats.setPosition(1100, 550);
        projectileDmgStats.setString("Projectile Damage: ");
        window.draw(projectileDmgStats);
        projectileDmgStats.setFillColor(Color(255, 165, 0));
        projectileDmgStats.setPosition(1450, 550);
        projectileDmgStats.setString(to_string((int)player[4].ProjectileDamage));
        window.draw(projectileDmgStats);

        //Projectile Cooldown
        projectileCooldownStats.setCharacterSize(35);
        projectileCooldownStats.setFillColor(Color::White);
        projectileCooldownStats.setPosition(1100, 600);
        projectileCooldownStats.setString("Projectile Cooldown: ");
        window.draw(projectileCooldownStats);
        projectileCooldownStats.setFillColor(Color(255, 165, 0));
        projectileCooldownStats.setPosition(1450, 600);
        projectileCooldownStats.setString(to_string((int)player[4].projectileCooldown));
        window.draw(projectileCooldownStats);

        //Starting Weapon
        playerStartingWeapon.setCharacterSize(35);
        playerStartingWeapon.setFillColor(Color::White);
        playerStartingWeapon.setPosition(1100, 650);
        playerStartingWeapon.setString("Starting Weapon: ");
        window.draw(playerStartingWeapon);
        playerStartingWeapon.setFillColor(Color(255, 165, 0));
        playerStartingWeapon.setPosition(1450, 650);
        playerStartingWeapon.setString(string(player[4].isWhipUnlocked ? "Whip" : "Pistol"));
    }
    window.draw(playerStartingWeapon);
}
void playerFiveStats()
{
    if (menu == 2 && shopPage == 5)
    {
        //Speed
        playerSpeedStats.setCharacterSize(35);
        playerSpeedStats.setFillColor(Color::White);
        playerSpeedStats.setPosition(1100, 300);
        playerSpeedStats.setString("Speed: ");
        window.draw(playerSpeedStats);
        playerSpeedStats.setFillColor(Color(255, 165, 0));
        playerSpeedStats.setPosition(1450, 300);
        playerSpeedStats.setString(to_string((int)player[5].speed));
        window.draw(playerSpeedStats);

        //Health
        playerHealthStats.setCharacterSize(35);
        playerHealthStats.setFillColor(Color::White);
        playerHealthStats.setPosition(1100, 350);
        playerHealthStats.setString("Health: ");
        window.draw(playerHealthStats);
        playerHealthStats.setFillColor(Color(255, 165, 0));
        playerHealthStats.setPosition(1450, 350);
        playerHealthStats.setString(to_string((int)player[5].maxHealth));
        window.draw(playerHealthStats);

        //Whip dmg
        whipDmgStats.setCharacterSize(35);
        whipDmgStats.setFillColor(Color::White);
        whipDmgStats.setPosition(1100, 400);
        whipDmgStats.setString("Whip Damage: ");
        window.draw(whipDmgStats);
        whipDmgStats.setFillColor(Color(255, 165, 0));
        whipDmgStats.setPosition(1450, 400);
        whipDmgStats.setString(to_string((int)player[5].whipDamage));
        window.draw(whipDmgStats);

        //Whip Cooldown
        whipCooldownStats.setCharacterSize(35);
        whipCooldownStats.setFillColor(Color::White);
        whipCooldownStats.setPosition(1100, 450);
        whipCooldownStats.setString("Whip Cooldown: ");
        window.draw(whipCooldownStats);
        whipCooldownStats.setFillColor(Color(255, 165, 0));
        whipCooldownStats.setPosition(1450, 450);
        whipCooldownStats.setString(to_string((int)player[5].whipCooldown));
        window.draw(whipCooldownStats);

        //Ring dmg
        ringBaseDmgStats.setCharacterSize(35);
        ringBaseDmgStats.setFillColor(Color::White);
        ringBaseDmgStats.setPosition(1100, 500);
        ringBaseDmgStats.setString("Ring Damage: ");
        window.draw(ringBaseDmgStats);
        ringBaseDmgStats.setFillColor(Color(255, 165, 0));
        ringBaseDmgStats.setPosition(1450, 500);
        ringBaseDmgStats.setString(to_string((int)player[5].ringDamage));
        window.draw(ringBaseDmgStats);

        //Projectile Dmg
        projectileDmgStats.setCharacterSize(35);
        projectileDmgStats.setFillColor(Color::White);
        projectileDmgStats.setPosition(1100, 550);
        projectileDmgStats.setString("Projectile Damage: ");
        window.draw(projectileDmgStats);
        projectileDmgStats.setFillColor(Color(255, 165, 0));
        projectileDmgStats.setPosition(1450, 550);
        projectileDmgStats.setString(to_string((int)player[5].ProjectileDamage));
        window.draw(projectileDmgStats);

        //Projectile Cooldown
        projectileCooldownStats.setCharacterSize(35);
        projectileCooldownStats.setFillColor(Color::White);
        projectileCooldownStats.setPosition(1100, 600);
        projectileCooldownStats.setString("Projectile Cooldown: ");
        window.draw(projectileCooldownStats);
        projectileCooldownStats.setFillColor(Color(255, 165, 0));
        projectileCooldownStats.setPosition(1450, 600);
        projectileCooldownStats.setString(to_string((int)player[5].projectileCooldown));
        window.draw(projectileCooldownStats);

        //Starting Weapon
        playerStartingWeapon.setCharacterSize(35);
        playerStartingWeapon.setFillColor(Color::White);
        playerStartingWeapon.setPosition(1100, 650);
        playerStartingWeapon.setString("Starting Weapon: ");
        window.draw(playerStartingWeapon);
        playerStartingWeapon.setFillColor(Color(255, 165, 0));
        playerStartingWeapon.setPosition(1450, 650);
        playerStartingWeapon.setString(string(player[5].isWhipUnlocked ? "Whip" : "Pistol"));
        window.draw(playerStartingWeapon);
    }
}
void playerSixStats()
{
    if (menu == 2 && shopPage == 6)
    {
        //Speed
        playerSpeedStats.setCharacterSize(35);
        playerSpeedStats.setFillColor(Color::White);
        playerSpeedStats.setPosition(1100, 300);
        playerSpeedStats.setString("Speed: ");
        window.draw(playerSpeedStats);
        playerSpeedStats.setFillColor(Color(255, 165, 0));
        playerSpeedStats.setPosition(1450, 300);
        playerSpeedStats.setString(to_string((int)player[6].speed));
        window.draw(playerSpeedStats);

        //Health
        playerHealthStats.setCharacterSize(35);
        playerHealthStats.setFillColor(Color::White);
        playerHealthStats.setPosition(1100, 350);
        playerHealthStats.setString("Health: ");
        window.draw(playerHealthStats);
        playerHealthStats.setFillColor(Color(255, 165, 0));
        playerHealthStats.setPosition(1450, 350);
        playerHealthStats.setString(to_string((int)player[6].maxHealth));
        window.draw(playerHealthStats);

        //Whip dmg
        whipDmgStats.setCharacterSize(35);
        whipDmgStats.setFillColor(Color::White);
        whipDmgStats.setPosition(1100, 400);
        whipDmgStats.setString("Whip Damage: ");
        window.draw(whipDmgStats);
        whipDmgStats.setFillColor(Color(255, 165, 0));
        whipDmgStats.setPosition(1450, 400);
        whipDmgStats.setString(to_string((int)player[6].whipDamage));
        window.draw(whipDmgStats);

        //Whip Cooldown
        whipCooldownStats.setCharacterSize(35);
        whipCooldownStats.setFillColor(Color::White);
        whipCooldownStats.setPosition(1100, 450);
        whipCooldownStats.setString("Whip Cooldown: ");
        window.draw(whipCooldownStats);
        whipCooldownStats.setFillColor(Color(255, 165, 0));
        whipCooldownStats.setPosition(1450, 450);
        whipCooldownStats.setString(to_string((int)player[6].whipCooldown));
        window.draw(whipCooldownStats);

        //Ring dmg
        ringBaseDmgStats.setCharacterSize(35);
        ringBaseDmgStats.setFillColor(Color::White);
        ringBaseDmgStats.setPosition(1100, 500);
        ringBaseDmgStats.setString("Ring Damage: ");
        window.draw(ringBaseDmgStats);
        ringBaseDmgStats.setFillColor(Color(255, 165, 0));
        ringBaseDmgStats.setPosition(1450, 500);
        ringBaseDmgStats.setString(to_string((int)player[6].ringDamage));
        window.draw(ringBaseDmgStats);

        //Projectile Dmg
        projectileDmgStats.setCharacterSize(35);
        projectileDmgStats.setFillColor(Color::White);
        projectileDmgStats.setPosition(1100, 550);
        projectileDmgStats.setString("Projectile Damage: ");
        window.draw(projectileDmgStats);
        projectileDmgStats.setFillColor(Color(255, 165, 0));
        projectileDmgStats.setPosition(1450, 550);
        projectileDmgStats.setString(to_string((int)player[6].ProjectileDamage));
        window.draw(projectileDmgStats);

        //Projectile Cooldown
        projectileCooldownStats.setCharacterSize(35);
        projectileCooldownStats.setFillColor(Color::White);
        projectileCooldownStats.setPosition(1100, 600);
        projectileCooldownStats.setString("Projectile Cooldown: ");
        window.draw(projectileCooldownStats);
        projectileCooldownStats.setFillColor(Color(255, 165, 0));
        projectileCooldownStats.setPosition(1450, 600);
        projectileCooldownStats.setString(to_string((int)player[6].projectileCooldown));
        window.draw(projectileCooldownStats);

        //Starting Weapon
        playerStartingWeapon.setCharacterSize(35);
        playerStartingWeapon.setFillColor(Color::White);
        playerStartingWeapon.setPosition(1100, 650);
        playerStartingWeapon.setString("Starting Weapon: ");
        window.draw(playerStartingWeapon);
        playerStartingWeapon.setFillColor(Color(255, 165, 0));
        playerStartingWeapon.setPosition(1450, 650);
        playerStartingWeapon.setString(string(player[6].isWhipUnlocked ? "Whip" : "Pistol"));
        window.draw(playerStartingWeapon);
    }
}
void playerSevenStats()
{
    if (menu == 2 && shopPage == 7)
    {
        //Speed
        playerSpeedStats.setCharacterSize(35);
        playerSpeedStats.setFillColor(Color::White);
        playerSpeedStats.setPosition(1100, 300);
        playerSpeedStats.setString("Speed: ");
        window.draw(playerSpeedStats);
        playerSpeedStats.setFillColor(Color(255, 165, 0));
        playerSpeedStats.setPosition(1450, 300);
        playerSpeedStats.setString(to_string((int)player[7].speed));
        window.draw(playerSpeedStats);

        //Health
        playerHealthStats.setCharacterSize(35);
        playerHealthStats.setFillColor(Color::White);
        playerHealthStats.setPosition(1100, 350);
        playerHealthStats.setString("Health: ");
        window.draw(playerHealthStats);
        playerHealthStats.setFillColor(Color(255, 165, 0));
        playerHealthStats.setPosition(1450, 350);
        playerHealthStats.setString(to_string((int)player[7].maxHealth));
        window.draw(playerHealthStats);

        //Whip dmg
        whipDmgStats.setCharacterSize(35);
        whipDmgStats.setFillColor(Color::White);
        whipDmgStats.setPosition(1100, 400);
        whipDmgStats.setString("Whip Damage: ");
        window.draw(whipDmgStats);
        whipDmgStats.setFillColor(Color(255, 165, 0));
        whipDmgStats.setPosition(1450, 400);
        whipDmgStats.setString(to_string((int)player[7].whipDamage));
        window.draw(whipDmgStats);

        //Whip Cooldown
        whipCooldownStats.setCharacterSize(35);
        whipCooldownStats.setFillColor(Color::White);
        whipCooldownStats.setPosition(1100, 450);
        whipCooldownStats.setString("Whip Cooldown: ");
        window.draw(whipCooldownStats);
        whipCooldownStats.setFillColor(Color(255, 165, 0));
        whipCooldownStats.setPosition(1450, 450);
        whipCooldownStats.setString(to_string((int)player[7].whipCooldown));
        window.draw(whipCooldownStats);

        //Ring dmg
        ringBaseDmgStats.setCharacterSize(35);
        ringBaseDmgStats.setFillColor(Color::White);
        ringBaseDmgStats.setPosition(1100, 500);
        ringBaseDmgStats.setString("Ring Damage: ");
        window.draw(ringBaseDmgStats);
        ringBaseDmgStats.setFillColor(Color(255, 165, 0));
        ringBaseDmgStats.setPosition(1450, 500);
        ringBaseDmgStats.setString(to_string((int)player[7].ringDamage));
        window.draw(ringBaseDmgStats);

        //Projectile Dmg
        projectileDmgStats.setCharacterSize(35);
        projectileDmgStats.setFillColor(Color::White);
        projectileDmgStats.setPosition(1100, 550);
        projectileDmgStats.setString("Projectile Damage: ");
        window.draw(projectileDmgStats);
        projectileDmgStats.setFillColor(Color(255, 165, 0));
        projectileDmgStats.setPosition(1450, 550);
        projectileDmgStats.setString(to_string((int)player[7].ProjectileDamage));
        window.draw(projectileDmgStats);

        //Projectile Cooldown
        projectileCooldownStats.setCharacterSize(35);
        projectileCooldownStats.setFillColor(Color::White);
        projectileCooldownStats.setPosition(1100, 600);
        projectileCooldownStats.setString("Projectile Cooldown: ");
        window.draw(projectileCooldownStats);
        projectileCooldownStats.setFillColor(Color(255, 165, 0));
        projectileCooldownStats.setPosition(1450, 600);
        projectileCooldownStats.setString(to_string((int)player[7].projectileCooldown));
        window.draw(projectileCooldownStats);

        //Starting Weapon
        playerStartingWeapon.setCharacterSize(35);
        playerStartingWeapon.setFillColor(Color::White);
        playerStartingWeapon.setPosition(1100, 650);
        playerStartingWeapon.setString("Starting Weapon: ");
        window.draw(playerStartingWeapon);
        playerStartingWeapon.setFillColor(Color(255, 165, 0));
        playerStartingWeapon.setPosition(1450, 650);
        playerStartingWeapon.setString(string(player[7].isWhipUnlocked ? "Whip" : "Pistol"));
        window.draw(playerStartingWeapon);
    }
}
void saveGameData() {
    ofstream saveFile("save.dat");
    if (saveFile.is_open()) {
        // Save coins
        saveFile << coins << " ";

        // Save unlocked status for each character
        for (int i = 0; i < 8; i++) {
            saveFile << player[i].isUnlocked << " ";
        }
        saveFile << character << " ";
        saveFile << volume << " ";
        saveFile << controls << "\n";

        for (int i = 0; i < 10; i++) {
            saveFile << highscores[i].name << "\t";
            saveFile << highscores[i].score << "\t";
            saveFile << highscores[i].time << "\n";
        }

        saveFile.close();
    }
}
void loadGameData() {
    ifstream saveFile("save.dat");
    if (saveFile.is_open()) {
        // Load coins
        saveFile >> coins;

        // Load unlocked status for each character
        for (int i = 0; i < 8; i++) {
            bool unlocked;
            saveFile >> unlocked;
            player[i].isUnlocked = unlocked;
        }

        saveFile >> character;
        saveFile >> volume;
        saveFile >> controls;


        for (int i = 0; i < 10; i++) {
            saveFile >> highscores[i].name;
            saveFile >> highscores[i].score;
            saveFile >> highscores[i].time;
        }
        saveFile.close();
    }
}
void leaderboardWidgets() {


    if (gameMode == 0) {
        menuBG[7].sprite.setTexture(menuBG[7].texture);
    }
    else if (gameMode == 1) {
        menuBG[7].sprite.setTexture(menuBG[7].horrorTexture);
    }

    backtomenuButton.sprite.setPosition(1920 / 1.2 + 100, 1080 / 5 - 50);
    backtomenuButton.sprite.setScale(1, 1);
    leaderboardSort();

    for (int i = 0; i < 10; i++) {
        leaderboardNames[i].setFont(font);
        leaderboardNames[i].setString(highscores[i].name);
        leaderboardNames[i].setCharacterSize(50);
        leaderboardNames[i].setFillColor(Color(205, 145, 43));
        leaderboardNames[i].setOutlineColor(Color::Black);
        leaderboardNames[i].setOutlineThickness(2);
        leaderboardNames[i].setPosition(350, 360 + 60 * i);

        leaderboardScores[i].setFont(font);
        leaderboardScores[i].setString(scoreFormatHandler(highscores[i].score));
        leaderboardScores[i].setCharacterSize(50);
        leaderboardScores[i].setFillColor(Color(205, 145, 43));
        leaderboardScores[i].setOutlineColor(Color::Black);
        leaderboardScores[i].setOutlineThickness(2);
        leaderboardScores[i].setPosition(910, 360 + 60 * i);

        leaderboardTimes[i].setFont(font);
        leaderboardTimes[i].setString(timerFormatHandler(highscores[i].time));
        leaderboardTimes[i].setCharacterSize(50);
        leaderboardTimes[i].setFillColor(Color(205, 145, 43));
        leaderboardTimes[i].setOutlineColor(Color::Black);
        leaderboardTimes[i].setOutlineThickness(2);
        leaderboardTimes[i].setPosition(1340, 360 + 60 * i);
    }
}
void leaderboardSort() {
    for (int i = 0; i < 9; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < 10; j++) {
            if (highscores[j].score > highscores[maxIndex].score) {
                maxIndex = j;
            }
        }
        // Swap the found maximum element with the first element
        Score temp = highscores[i];
        highscores[i] = highscores[maxIndex];
        highscores[maxIndex] = temp;
    }

    // find minimum to replace it with the score if we get something higher (to shift scores)
}
void leaderboardHandler() {

}
void nameWidgets() {


    if (gameMode == 0) {
        menuBG[8].sprite.setTexture(menuBG[8].texture);
        doneButton.sprite.setTexture(doneButton.texture);
    }
    else if (gameMode == 1) {
        menuBG[8].sprite.setTexture(menuBG[8].horrorTexture);
        doneButton.sprite.setTexture(doneButton.horrorTexture);
    }

    player[character].sprite.setScale(5, 5);
    player[character].sprite.setPosition(1920 / 2, 1080 / 3);
    player[character].sprite.setColor(Color::White);

    doneButton.sprite.setPosition(1920 / 2 - 222, 1080 / 1.8);
    doneButton.sprite.setScale(2, 2);

    currentName.setPosition(400, 880);
    currentName.setFillColor(Color::Black);
    currentName.setCharacterSize(100);
    current.name = "";
}
void nameHandler(Event event) {
    if (menu == 6) {
        if (doneButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
        {
            doneButton.sprite.setTextureRect(IntRect(0, 108, 222, 108));
            doneButton.sprite.setColor(sf::Color(200, 200, 200));
            if (Mouse::isButtonPressed(Mouse::Left)) {
                clickSound.play();
                sleep(milliseconds(200));
                menu = 1;
                backgroundMusic.pause();
                if (gameMode == 0) {
                    playNormalLevelMusic();
                }
                else if (gameMode == 1) {
                    playGoreLevelMusic();
                }
                //RESETING
                resetGame();
            }
        }
        else
        {
            doneButton.sprite.setTextureRect(IntRect(0, 0, 222, 108));
            doneButton.sprite.setColor(Color::White);
        }

        // Text entered
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b') {
                if (!current.name.empty())
                    current.name.pop_back();
            }
            else if (current.name.size() < 21 && event.text.unicode < 128) {
                current.name += static_cast<char>(event.text.unicode);
            }
        }
        currentName.setString(current.name);
    }

}