#ifndef TATICAS_DO_DESTINO_ANIMAL_COMPANION_H
#define TATICAS_DO_DESTINO_ANIMAL_COMPANION_H

#include "RpgSystem.h"
#include <string>
#include <vector>
#include <cmath>

namespace rpg {

struct BaseAnimal {
    std::string species;
    int baseHD;
    int baseNaturalArmor;
    Attributes baseStats;
    Size size;
    DamageDice attackDamage;
};

struct AnimalCompanion {
    BaseAnimal base;
    int bonusHD = 0;
    int naturalArmorBonus = 0;
    int strDexBonus = 0;

    // Status atuais do animal
    int currentHP;
    int x, y;

    int getEffectiveStr() const { return base.baseStats.values[static_cast<int>(AttributeType::STR)] + strDexBonus; }
    int getEffectiveDex() const { return base.baseStats.values[static_cast<int>(AttributeType::DEX)] + strDexBonus; }
    int getEffectiveCon() const { return base.baseStats.values[static_cast<int>(AttributeType::CON)]; }

    int getAC() const {
        int dexMod = (getEffectiveDex() - 10) / 2;
        int sizeMod = RpgData::getSizeModifier(base.size);
        return 10 + dexMod + sizeMod + base.baseNaturalArmor + naturalArmorBonus;
    }

    int getBAB() const {
        // Animais têm BAB de 3/4 do total de HD (Base + Bônus)
        int totalHD = base.baseHD + bonusHD;
        return (totalHD * 3) / 4;
    }

    int getMeleeAttackBonus() const {
        int strMod = (getEffectiveStr() - 10) / 2;
        int sizeMod = RpgData::getSizeModifier(base.size);
        return getBAB() + strMod + sizeMod;
    }
};

class CompanionData {
public:
    static BaseAnimal getBaseAnimal(const std::string& name) {
        static std::map<std::string, BaseAnimal> animals = {
            {"Lobo", {"Lobo", 2, 2, {{13, 15, 15, 2, 12, 6}}, Size::MEDIUM, {1, 6}}},
            {"Cão de Montaria", {"Cão de Montaria", 2, 2, {{15, 15, 15, 2, 12, 6}}, Size::MEDIUM, {1, 6}}},
            {"Águia", {"Águia", 1, 1, {{10, 15, 12, 2, 14, 6}}, Size::SMALL, {1, 4}}},
            {"Coruja", {"Coruja", 1, 2, {{6, 17, 10, 2, 14, 4}}, Size::TINY, {1, 4}}},
            {"Teixugo", {"Teixugo", 1, 0, {{8, 17, 15, 2, 12, 6}}, Size::TINY, {1, 2}}},
            {"Cobra Víbora Média", {"Cobra Víbora Média", 2, 3, {{10, 17, 11, 2, 12, 2}}, Size::MEDIUM, {1, 4}}}
        };
        if (animals.count(name)) return animals[name];
        return animals["Lobo"];
    }

    static void updateCompanionStats(Character* master) {
        if (!master->companion) return;

        int druidLvl = master->levels.count(ClassType::DRUID) ? master->levels[ClassType::DRUID] : 0;
        int rangerLvl = master->levels.count(ClassType::RANGER) ? master->levels[ClassType::RANGER] : 0;

        // Nível Efetivo de Druida: Druida puro ou Ranger - 3
        int effectiveLvl = druidLvl + std::max(0, rangerLvl - 3);

        if (effectiveLvl <= 0) return;

        AnimalCompanion* c = master->companion;

        // Tabela de Progressão 3.5
        if (effectiveLvl >= 18) { c->bonusHD = 12; c->naturalArmorBonus = 12; c->strDexBonus = 6; }
        else if (effectiveLvl >= 15) { c->bonusHD = 10; c->naturalArmorBonus = 10; c->strDexBonus = 5; }
        else if (effectiveLvl >= 12) { c->bonusHD = 8; c->naturalArmorBonus = 8; c->strDexBonus = 4; }
        else if (effectiveLvl >= 9) { c->bonusHD = 6; c->naturalArmorBonus = 6; c->strDexBonus = 3; }
        else if (effectiveLvl >= 6) { c->bonusHD = 4; c->naturalArmorBonus = 4; c->strDexBonus = 2; }
        else if (effectiveLvl >= 3) { c->bonusHD = 2; c->naturalArmorBonus = 2; c->strDexBonus = 1; }
        else { c->bonusHD = 0; c->naturalArmorBonus = 0; c->strDexBonus = 0; }
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_ANIMAL_COMPANION_H
