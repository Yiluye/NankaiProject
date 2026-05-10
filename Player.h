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
    void TakeDamage(int damage);
    int GetHp() const;
    bool IsAlive() const;
    int GetRadius() const;

    void AddHp(int amount);   // ¼ÓÑª
    void SubHp(int amount);   // ¼õÑª

    void Reset();

};