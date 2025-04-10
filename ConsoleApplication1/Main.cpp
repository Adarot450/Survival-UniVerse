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
const int PLAYER_SPEED = 200;
const float PLAYER_ANIMATION_RATE = 0.05; // Indicates how fast are we switching player's texture
const float LOGO_ANIMATION_RATE = 0.06; // Indicates how fast are we switching logo's texture
const int WHIP_TRAVEL_DISTANCE = 2000; //How far the whip moves away from the player
const float WHIP_RATE = 0.2; //how fast are we moving through whip texture
const float WHIP_COOLDOWN = 1.5f;
const float PROJECTILE_SPEED = 400.0f;  // Adjust speed as needed
const float PROJECTILE_COOLDOWN = 0.7f;  // Cooldown between auto shots
const float EnemyAnimationRate = 0.135f;  //  How fast we are switching Enemy's texture

//Global Variables
bool isMoving = false;
bool playerPos = true; // true = left  | false  = right
int walkIndx = 0;
int logoIndx = 0;
float logoAnimationTimer = 0;// Player's texture switch timer (Always set to ZERO)
float playerAnimationTimer = 0;// Player's texture switch timer (Always set to ZERO)
float whipTimer = 0; //whip's animation timer (always set to ZERO)
int  whipIndx = 0;
int menu = 0; // 0 = main menu | 1 = playing
float whipCooldownTimer = 0;
bool canAttack = true;
float timeSinceLastHit = 999;

// Health system
float maxHealth = 100.0f;
float currentHealth = maxHealth;

// XP system
float maxXP = 10.0f;
float currentXP = 0.0f;
int playerLevel = 1;

int bgIndx = 0;


sf::RenderWindow window(sf::VideoMode(1920, 1080), "Survival@Uni-Verse");


// sprite and Textures
Sprite player;
Sprite background;
Sprite whip;
Sprite logo;
Sprite xpBarSprite;  // New XP bar sprite

Texture logoTexture;
Texture menuBackgroundTexture;
Texture playerSheet;
Texture backgroundTexture;
Texture whipSheet;
Texture ZombieSheet;
Texture xpBarTexture;  // New XP bar texture


//Shapes
RectangleShape playerHitbox(Vector2f(20, 40));
RectangleShape whipHitbox(Vector2f(50, 125));
RectangleShape healthBarBackground(Vector2f(60, 5));
RectangleShape healthBarFill(Vector2f(60, 5));
RectangleShape xpBarFill(Vector2f(0, 15));       // Start with 0 width to be empty


//miscellaneous
View view(Vector2f(1920 / 2, 1080 / 2), Vector2f(1920, 1080));
float deltaTime;




//Functions Declerations

void Update();
void Start();
void Draw();

void logoAnimation(); // Aly
void backgroundHandeling(); //Amr
void playerMovement();      //Amr
void loadTextures();
void playerAnimation();     //Amr
void playeCollider();       //Amr
void bleedEffect();         //Yassin and Adam
void playerHitboxHandeling();//Amr
void BorderCollision();      //Amr
void lockViewToBackground(); //Amr
void whipAnimation();        //Amr/Aly
void whipCollider();         //Amr
void whipHitboxHandeling();  //Amr
void mainmenuWidgets();      //Aly
void healthBarHandling();    //Yassin
void addXp(float xpToAdd);   //Yassin
void takeDamage(float damage);//Yassin
void heal(float amount);      //Yassin
void createProjectile(); //Yassin
void updateProjectile(); //Yassin
void SpwaningZombies();      //Adam
void ZombieHandler();        //Adam
Vector2f Normalize(Vector2f vector); //Adam

//structs

struct Background {
    Sprite sprite;
    Texture texture;
};

Background menuBG[10]; // 0:Main menu

struct Projectile {
    Sprite sprite;
    Vector2f direction;
    float speed;
    bool active = false;
};

Projectile projectile;        // Single projectile instead of vector
Texture projectileTexture;
float projectileCooldown = 0.5f;
float projectileTimer = 0.0f;


struct Button {
    Sprite sprite;
    Texture texture;
};

Button playButton;
Button shopButton;
Button optionsButton;
Button quitButton;

struct ZombieType {
    //Adam

    Sprite Shape;
    RectangleShape HitBox;
    Vector2f velocity;
    int DMG = 5;
    int HP = 10;
    int speed = 180;
    int AnimationIndex = 0;
    float AnimtaionTimer = 0;
    float AnimtaionRate = 0.135f;
    float attackTimer = 0;
    float attackRate = 0.25;
    bool isDead = false;

    void Start() {
        Shape.setTexture(ZombieSheet);
        Shape.setTextureRect(IntRect(0, 0, 35, 48));

        HitBox.setSize(Vector2f(35, 48));
        HitBox.setOrigin(HitBox.getLocalBounds().width / 2, HitBox.getLocalBounds().height / 2);
        HitBox.setFillColor(Color::Transparent);
        HitBox.setOutlineColor(Color::Red);
        HitBox.setOutlineThickness(2);

        Shape.setOrigin(Shape.getLocalBounds().width / 2, Shape.getLocalBounds().height / 2);

        Shape.setPosition(player.getPosition().x + 100, player.getPosition().y);
    }

    void Update() {
        CalcDirection();
        walk();
        Animtaion();
        Attack();
        Die();
        HitBox.setPosition(Shape.getPosition());
    }

    void CalcDirection() {
        velocity = player.getPosition() - Shape.getPosition(); //get line from player to zombie
        velocity = Normalize(velocity); //Normalize velocity to mulltiply it by speed
    }

    void walk() {
        Shape.move(velocity.x * speed * deltaTime, velocity.y * speed * deltaTime);
    }

    void Animtaion() {
        int colSize = 3;
        AnimtaionTimer += deltaTime;

        //animate
        if (AnimtaionTimer >= AnimtaionRate) {
            AnimtaionTimer = 0;
            Shape.setTexture(ZombieSheet);
            AnimationIndex = (AnimationIndex + 1) % colSize;
            Shape.setTextureRect(IntRect(AnimationIndex * 35, 0, 35, 48));
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
};

//vectors
vector<ZombieType>Zombies;

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
    //whip.setPosition(player.getPosition().x - 75, player.getPosition().y - 75); // set the start of the animation
    playerHitboxHandeling();
    whipHitboxHandeling();

    // Initialize health bar
    healthBarBackground.setFillColor(Color::Black);
    healthBarFill.setFillColor(Color::Red);
    healthBarBackground.setOrigin(healthBarBackground.getSize().x / 2, healthBarBackground.getSize().y / 2);
    healthBarFill.setOrigin(healthBarFill.getSize().x / 2, healthBarFill.getSize().y / 2);

    // Initialize XP bar
    xpBarFill.setFillColor(Color::Cyan);
    xpBarSprite.setScale(2, 2);
    xpBarFill.setScale(1.85, 2);

    //whip
    whip.setOrigin(whip.getLocalBounds().width / 2, player.getLocalBounds().height / 2);
    whipHitbox.setOrigin(whipHitbox.getLocalBounds().width / 2, whipHitbox.getLocalBounds().height / 2);

    //position setting for xp bars
    xpBarSprite.setPosition((1920 / 50) + 310, 1080 / 100);
    xpBarFill.setPosition((1920 / 50) + 350, (1080 / 50) + 73);

    // Reset XP to 0
    currentXP = 0.0f;
    //xpbar
    xpBarFill.setFillColor(Color(0, 255, 255));

}
void Update()
{


    window.setView(view);

    if (menu == 0) { // main menu

        window.draw(menuBG[0].sprite);
        logoAnimation();

        if (playButton.sprite.getGlobalBounds().contains(Mouse::getPosition(window).x, Mouse::getPosition(window).y))
        {
            playButton.sprite.setTextureRect(IntRect(0, 108, 222, 108));
            playButton.sprite.setColor(sf::Color(200, 200, 200));
            if (Mouse::isButtonPressed(Mouse::Left)) {
                sleep(milliseconds(200)); // little delay before starting game for smooth transition
                menu = 1;
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
    else if (menu == 1) { // game
        BorderCollision();
        lockViewToBackground();
        playerMovement();
        playerAnimation();
        playeCollider();
        whipAnimation();
        whipAnimation();
        whipCollider();
        bleedEffect();

        // Handle projectile
        createProjectile();
        updateProjectile();

        //Enemy
        ZombieHandler();
        SpwaningZombies();
    }




}
void Draw()
{
    window.clear();
    if (menu == 0) { // main menu
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


        // Draw projectile if active
        if (projectile.active)
        {
            window.draw(projectile.sprite);
        }

        if (whipIndx != 3) {
            window.draw(whip);
            window.draw(whipHitbox);
        }

        //Draw Zombies
        for (int i = 0; i < Zombies.size(); i++) {
            window.draw(Zombies[i].HitBox);
            window.draw(Zombies[i].Shape);
        }

        // Draw UI elements (with fixed view)
        sf::View fixedView(sf::FloatRect(0, 0, 1920, 1080));
        window.setView(fixedView);

        // Draw XP bar
        addXp(0.0f);
        window.draw(xpBarFill);
        window.draw(xpBarSprite);

        // Draw health bar (with game view)
        window.setView(view);
        healthBarHandling();
        window.draw(healthBarBackground);
        window.draw(healthBarFill);
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
    shopButton.texture.loadFromFile("assets/shop_button.png");
    optionsButton.texture.loadFromFile("assets/options_button.png");
    quitButton.texture.loadFromFile("assets/quit_button.png");
    menuBG[0].texture.loadFromFile("assets/menu_background.png");
    ZombieSheet.loadFromFile("assets/zombie1.png");
    xpBarTexture.loadFromFile("assets/xpbar.png");
    xpBarSprite.setTexture(xpBarTexture);
    projectileTexture.loadFromFile("assets/projectile.png");
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
}//Yassin Amr
void addXp(float xpToAdd)
{
    // Add XP from orb collection
    currentXP += xpToAdd;
    if (currentXP > maxXP)
    {
        currentXP = 0; // so that it never exceeds max
        maxXP *= 2;
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

Vector2f getDirectionToZombie()
{
    // Get direction from player to zombie
    Vector2f zombiePos = Zombie.getPosition();
    Vector2f playerPos = player.getPosition();
    Vector2f direction = zombiePos - playerPos;

    // Normalize the direction
    float length = sqrt(direction.x * direction.x + direction.y * direction.y); //returning the same vector direction but the  magnitude=1
    return Vector2f(direction.x / length, direction.y / length);
}

void createProjectile()
{
    if (projectileTimer >= PROJECTILE_COOLDOWN && !projectile.active) //projectile.active=false at the start but to enter the function we must turn it true because the && condition is only entered when both conditions are true,but projectile.active is still false going in.
    {
        projectileTimer = 0;  // Reset timer

        projectile.sprite.setTexture(projectileTexture);
        projectile.sprite.setOrigin(projectile.sprite.getLocalBounds().width / 2, projectile.sprite.getLocalBounds().height / 2);
        projectile.sprite.setPosition(player.getPosition());
        projectile.active = true;

        // Get direction to zombie
        projectile.direction = getDirectionToZombie();
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

        // Check collision with zombie
        if (projectile.sprite.getGlobalBounds().intersects(Zombie.getGlobalBounds()))
        {
            projectile.active = false;
            // Damage logic would go here
        }

        // Check if projectile is off screen
        Vector2f pos = projectile.sprite.getPosition();
        if (pos.x < 0 || pos.x > background.getGlobalBounds().width ||
            pos.y < 0 || pos.y > background.getGlobalBounds().height)
        {
            projectile.active = false;
        }
    }

    // Always try to create a new projectile if there isn't one active
    createProjectile();
}

void SpwaningZombies() {
    if (Keyboard::isKeyPressed(Keyboard::Z) && Zombies.size() == 0) {
        ZombieType newZombie;
        newZombie.Start();
        Zombies.push_back(newZombie);
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
            Zombies.erase(Zombies.begin() + i);
            continue;
        }
        Zombies[i].Update();
    }
}