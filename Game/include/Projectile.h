#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile {
public:
    static const float DEFAULT_PROJECTILE_FIRE_RATE;
    static const float DEFAULT_SIZE;
    static const float DEFAULT_SPEED;
    static const float DEFAULT_RANGE;
    static const int DEFAULT_DAMAGE;
    static const float MISSILE_FIRE_RATE;
    static const int MISSILE_DAMAGE;
    static const float MISSILE_SPEED;
    static const float MISSILE_RANGE;
    static const float MISSILE_SIZE;
    Projectile(float x, float y, float size, float speed, float range, int damage, int direction, bool isMissile = false);
    void update(float deltaTime);
    void render() const;
    bool isExpired() const;
    float getX() const;
    float getY() const;
    int getDamage() const;
    float getSize() const;  // Function to get the size of the projectile
    void markForRemoval();
    bool isMarkedForRemoval() const;



private:
    float x, y;
    float size;
    float speed;
    float range;
    int damage;
    int direction;  // Direction the projectile is moving
    float distanceTravelled;  // How far the projectile has traveled
    bool markedForRemoval = false;  // New private member
    bool isMissile;  // New member to indicate if it's a missile


};

#endif // PROJECTILE_H
