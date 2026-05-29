#ifndef TATICAS_DO_DESTINO_LOOT_MANAGER_H
#define TATICAS_DO_DESTINO_LOOT_MANAGER_H

#include "EnemyData.h"
#include "ItemData.h"
#include "Dice.h"
#include <vector>
#include <variant>

namespace rpg {

struct LootResult {
    int gold;
    std::string weaponOrArmorName;
    std::optional<Accessory> accessory;
};

class LootManager {
public:
    static LootResult generateLoot(const EnemyTemplate& enemy) {
        LootResult result = {enemy.goldAward, "", std::nullopt};

        // Regra: Somente monstros de Tier 3 (XP 1000) dropam itens raros
        if (enemy.xpAward >= 1000) {
            int roll = Dice::roll(100);
            if (roll > 70) {
                // Drop de Acessório Raro
                result.accessory = ItemData::getRandomRareAccessory();
            } else if (roll > 40) {
                // Drop de Arma Rara (Base)
                static std::vector<std::string> rarePool = {
                    "Espada Larga", "Machado Grande", "Espada Bastarda", "Corrente com Cravos"
                };
                result.weaponOrArmorName = rarePool[rand() % rarePool.size()];
            }
        }

        return result;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_LOOT_MANAGER_H
