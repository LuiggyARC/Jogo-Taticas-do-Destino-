#ifndef TATICAS_DO_DESTINO_SHOP_MANAGER_H
#define TATICAS_DO_DESTINO_SHOP_MANAGER_H

#include "RpgSystem.h"
#include "ItemData.h"
#include "UpgradeManager.h"
#include <vector>

namespace rpg {

struct ShopItem {
    std::string name;
    int cost;
    bool isWeapon;
};

class ShopManager {
public:
    static std::vector<ShopItem> getAvailableItems() {
        return {
            {"Adaga", 2, true},
            {"Espada Curta", 10, true},
            {"Espada Longa", 15, true},
            {"Arco Curto", 30, true},
            {"Arco Longo", 75, true},
            {"Machado Grande", 20, true},
            {"Couro", 10, false},
            {"Camisote de Malha", 100, false},
            {"Cota de Malha", 150, false},
            {"Peitoral de Aço", 200, false},
            {"Armadura de Batalha", 1500, false},
            {"Escudo Pesado", 20, false}
        };
    }

    static bool buyItem(Character* character, const ShopItem& item) {
        if (character->gold >= item.cost) {
            character->gold -= item.cost;
            if (item.isWeapon) {
                character->equippedWeapon = ItemData::getWeapon(item.name);
            } else {
                character->equippedArmor = ItemData::getArmor(item.name);
            }
            return true;
        }
        return false;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_SHOP_MANAGER_H
