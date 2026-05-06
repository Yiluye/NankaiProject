#pragma once
#include"Gameobject.h"

class Player :public Gameobject {
private:
	int hp;
	int radius;
	double speed;
public:
    Player();
    ~Player() = default;

    void Update();
    void Draw() override;

    int GetHp() const;
    void TakeDamage(int damage);
    bool IsAlive() const;
    int GetRadius() const;

    void Reset();

};