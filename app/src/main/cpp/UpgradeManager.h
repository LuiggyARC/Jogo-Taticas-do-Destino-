#ifndef TATICAS_DO_DESTINO_UPGRADE_MANAGER_H
#define TATICAS_DO_DESTINO_UPGRADE_MANAGER_H

#include "RpgSystem.h"
#include <cmath>

namespace rpg {

class UpgradeManager {
public:
    static int getEnhancementCost(int currentEnhancement) {
        if (currentEnhancement >= 6) return -1; // Máximo atingido
        // +1: 300, +2: 600, +3: 1200...
        return 300 * std::pow(2, currentEnhancement);
    }

    static bool upgradeWeapon(Character* character) {
        int cost = getEnhancementCost(character->equippedWeapon.enhancement);
        if (cost > 0 && character->gold >= cost) {
            character->gold -= cost;
            character->equippedWeapon.enhancement++;
            return true;
        }
        return false;
    }

    static bool upgradeArmor(Character* character) {
        int cost = getEnhancementCost(character->equippedArmor.enhancement);
        if (cost > 0 && character->gold >= cost) {
            character->gold -= cost;
            character->equippedArmor.enhancement++;
            return true;
        }
        return false;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_UPGRADE_MANAGER_H
