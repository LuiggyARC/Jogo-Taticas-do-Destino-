#ifndef TATICAS_DO_DESTINO_ITEM_DATA_H
#define TATICAS_DO_DESTINO_ITEM_DATA_H

#include "RpgSystem.h"
#include <map>
#include <string>

namespace rpg {

class ItemData {
public:
    static Armor getArmor(const std::string& name) {
        static std::map<std::string, Armor> armors = {
            // Leves
            {"Acolchoada", {"Acolchoada", 1, 8, 0}},
            {"Couro", {"Couro", 2, 6, 0}},
            {"Couro Batido", {"Couro Batido", 3, 5, -1}},
            {"Camisote de Malha", {"Camisote de Malha", 4, 4, -2}},
            // Médias
            {"Couro de Animais", {"Couro de Animais", 3, 4, -3}},
            {"Cota de Escamas", {"Cota de Escamas", 4, 3, -4}},
            {"Cota de Malha", {"Cota de Malha", 5, 2, -5}},
            {"Peitoral de Aço", {"Peitoral de Aço", 5, 3, -4}},
            // Pesadas
            {"Cota de Talas", {"Cota de Talas", 6, 0, -7}},
            {"Cota de Bandas", {"Cota de Bandas", 7, 1, -6}},
            {"Meia-Armadura", {"Meia-Armadura", 7, 0, -7}},
            {"Armadura de Batalha", {"Armadura de Batalha", 8, 1, -6}},
            // Escudos
            {"Broquel", {"Broquel", 1, 99, -1}},
            {"Escudo Leve", {"Escudo Leve", 1, 99, -1}},
            {"Escudo Pesado", {"Escudo Pesado", 2, 99, -2}},
            {"Escudo de Corpo", {"Escudo de Corpo", 4, 2, -10}}
        };
        if (armors.count(name)) return armors[name];
        return {"Nenhuma", 0, 99, 0};
    }

    static Weapon getWeapon(const std::string& name) {
        // ... (existing code)
    }

    static Accessory getRandomRareAccessory() {
        static std::vector<Accessory> accessories = {
            {"Tiara da Inteligência", AccessorySlot::HEAD, AttributeType::INT, 2},
            {"Chapéu do Carisma", AccessorySlot::HEAD, AttributeType::CHA, 2},
            {"Manoplas de Força", AccessorySlot::HANDS, AttributeType::STR, 2},
            {"Luvas de Destreza", AccessorySlot::HANDS, AttributeType::DEX, 2},
            {"Cinto da Constituição", AccessorySlot::WAIST, AttributeType::CON, 2},
            {"Botas de Agilidade", AccessorySlot::FEET, AttributeType::DEX, 2}
        };
        return accessories[rand() % accessories.size()];
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_ITEM_DATA_H
