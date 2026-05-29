#ifndef TATICAS_DO_DESTINO_ENEMY_DATA_H
#define TATICAS_DO_DESTINO_ENEMY_DATA_H

#include "RpgSystem.h"
#include <string>
#include <vector>
#include <map>

namespace rpg {

enum class MonsterAbility {
    NONE,
    REGENERATION,   // Troll: recupera HP por turno
    POISON_STING,   // Wyvern: dano extra e condição
    EYE_RAY,        // Beholder: raios oculares
    REACH_ATTACK,   // Gigantes: ataque com alcance maior
    POUNCE,         // Lobo/Orc: investida
    ETHEREAL        // Fantasma: ignora obstáculos e LoS
};

struct EnemyTemplate {
    std::string name;
    int cr;
    int hp;
    int ac;
    int attackBonus;
    DamageDice damage;
    int xpAward;
    int goldAward;
    MonsterAbility specialAbility;
    Size size;
};

class EnemyData {
public:
    static std::vector<EnemyTemplate> getEnemiesByTier(int tier) {
        static std::map<int, std::vector<EnemyTemplate>> tiers = {
            {1, { // CR 1-3 (Fácil) - 100 XP, 10 Gold
                {"Goblin", 1, 5, 15, 2, {1, 4}, 100, 10, MonsterAbility::NONE, Size::SMALL},
                {"Orc", 1, 6, 13, 4, {1, 8}, 100, 10, MonsterAbility::POUNCE, Size::MEDIUM},
                {"Lobo Atroz", 3, 45, 14, 11, {1, 8}, 100, 10, MonsterAbility::POUNCE, Size::LARGE},
                {"Bugbear", 2, 16, 17, 5, {1, 8}, 100, 10, MonsterAbility::NONE, Size::MEDIUM},
                {"Ogre", 3, 29, 16, 8, {2, 8}, 100, 10, MonsterAbility::REACH_ATTACK, Size::LARGE}
            }},
            {2, { // CR 4-7 (Médio) - 500 XP, 35 Gold
                {"Minotauro", 4, 39, 14, 9, {3, 6}, 500, 35, MonsterAbility::REACH_ATTACK, Size::LARGE},
                {"Troll", 5, 63, 16, 9, {1, 6}, 500, 35, MonsterAbility::REGENERATION, Size::LARGE},
                {"Wyvern", 6, 59, 18, 10, {2, 8}, 500, 35, MonsterAbility::POISON_STING, Size::LARGE},
                {"Gigante das Colinas", 7, 102, 20, 16, {2, 8}, 500, 35, MonsterAbility::REACH_ATTACK, Size::LARGE}
            }},
            {3, { // CR 8-13 (Difícil) - 1000 XP, 50 Gold + Item
                {"Gigante do Gelo", 9, 133, 21, 20, {3, 8}, 1000, 50, MonsterAbility::REACH_ATTACK, Size::LARGE},
                {"Gigante do Fogo", 10, 142, 23, 20, {3, 8}, 1000, 50, MonsterAbility::REACH_ATTACK, Size::LARGE},
                {"Vrock", 9, 115, 22, 15, {2, 6}, 1000, 50, MonsterAbility::POUNCE, Size::LARGE},
                {"Beholder", 13, 65, 26, 7, {2, 4}, 1000, 50, MonsterAbility::EYE_RAY, Size::LARGE}
            }}
        };
        return tiers[tier];
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_ENEMY_DATA_H
