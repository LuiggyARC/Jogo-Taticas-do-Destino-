#ifndef TATICAS_DO_DESTINO_RPG_SYSTEM_H
#define TATICAS_DO_DESTINO_RPG_SYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <cmath>
#include <chrono>

namespace rpg {

enum class AttributeType {
    STR = 0, DEX, CON, INT, WIS, CHA
};

enum class Size {
    FINE, DIMINUTIVE, TINY, SMALL, MEDIUM, LARGE, HUGE, GARGANTUAN, COLOSSAL
};

enum class Condition {
    PRONE,      // Caído: -4 AC vs Melee, +4 AC vs Ranged, -4 Atk
    STUNNED,    // Atordoado: Sem ações, -2 AC, perde Dex na AC
    FLANKED,    // Flanqueado: Atacantes ganham +2 no acerto
    BLINDED,    // Cego: 50% falha, perde Dex na AC, -2 AC
    ENTANGLED,  // Enredado: -2 Atk, -4 Dex
    IN_MELEE,   // Em combate corpo a corpo
    RAGING,     // Bárbaro em Fúria: +4 For/Con, -2 AC
    INSPIRED    // Bardo: +1 Ataque/Dano/Saves contra medo
};

enum class Feat {
    POINT_BLANK_SHOT, // +1 atq/dmg dentro de 30ft
    PRECISE_SHOT,     // Ignora penalidade de -4 por atirar em combate corpo a corpo
    WEAPON_FOCUS,      // +1 no acerto com arma específica
    POWER_ATTACK,      // Troca acerto por dano (precisa de implementação de valor)
    TOUGHNESS,         // +3 HP
    GREAT_FORTITUDE,   // +2 Fortitude
    LIGHTNING_REFLEXES, // +2 Reflexos
    IRON_WILL          // +2 Vontade
};

enum class SpellSchool {
    ABJURATION, CONJURATION, DIVINATION, ENCHANTMENT, EVOCATION, ILLUSION, NECROMANCY, TRANSMUTATION
};

enum class SaveType {
    NONE, FORT, REF, WILL
};

struct Spell {
    std::string name;
    int level;
    SpellSchool school;
    SaveType saveType;
    int range; // em pés
    bool allowSR;
    std::string description;
};

struct Attributes {
    int values[6];

    int getModifier(AttributeType type) const {
        return (values[static_cast<int>(type)] - 10) / 2;
    }
};

enum class RaceType {
    HUMAN, ELF, DWARF, HALFLING, GNOME, HALF_ELF, HALF_ORC,
    AASIMAR, TIEFLING, DROW, OGRE
};

struct Race {
    std::string name;
    RaceType type;
    int attributeModifiers[6];
    int speed;
    int levelAdjustment;
    Size size;
};

enum class SkillType {
    ATHLETICS,   // STR based
    STEALTH,     // DEX based
    SPOT,        // WIS based
    BLUFF,       // CHA based
    KNOWLEDGE_ARCANA,
    KNOWLEDGE_MONSTERS
};

struct DamageDice {
    int count;
    int sides;
};

enum class WeaponCategory {
    MELEE,
    RANGED,
    THROWN
};

struct Weapon {
    std::string name;
    DamageDice mediumDamage;
    WeaponCategory category = WeaponCategory::MELEE;
    int rangeIncrement = 0; // em pés
    bool addsStrDamage = true;
    int enhancement = 0; // +1 a +6
};

struct Armor {
    std::string name;
    int armorBonus;
    int maxDexBonus;
    int armorCheckPenalty; // ACP
    int enhancement = 0; // +1 a +6
};

enum class AccessorySlot {
    HEAD,     // Chapéus/Tiaras
    HANDS,    // Manoplas/Luvas
    WAIST,    // Cintos
    FEET      // Botas
};

struct Accessory {
    std::string name;
    AccessorySlot slot;
    AttributeType bonusAttribute;
    int bonusValue;
};

enum class BabProgression {
    GOOD, AVERAGE, POOR
};

enum class ClassType {
    BARBARIAN, BARD, CLERIC, DRUID, FIGHTER, MONK, PALADIN, RANGER, ROGUE, SORCERER, WIZARD
};

struct CharacterClass {
    std::string name;
    ClassType type;
    int hitDie;
    BabProgression babType;
    bool goodFort, goodRef, goodWill;
};

class RpgData {
public:
    static int getSizeModifier(Size size) {
        switch (size) {
            case Size::FINE: return 8;
            case Size::DIMINUTIVE: return 4;
            case Size::TINY: return 2;
            case Size::SMALL: return 1;
            case Size::MEDIUM: return 0;
            case Size::LARGE: return -1;
            case Size::HUGE: return -2;
            case Size::GARGANTUAN: return -4;
            case Size::COLOSSAL: return -8;
            default: return 0;
        }
    }

    static int getStealthSizeModifier(Size size) {
        switch (size) {
            case Size::FINE: return 16;
            case Size::DIMINUTIVE: return 12;
            case Size::TINY: return 8;
            case Size::SMALL: return 4;
            case Size::MEDIUM: return 0;
            case Size::LARGE: return -4;
            case Size::HUGE: return -8;
            case Size::GARGANTUAN: return -12;
            case Size::COLOSSAL: return -16;
            default: return 0;
        }
    }

    static DamageDice scaleWeaponDamage(DamageDice base, Size currentSize) {
        if (currentSize == Size::MEDIUM) return base;
        if (currentSize == Size::SMALL) {
            if (base.count == 1) {
                if (base.sides == 10) return {1, 8};
                if (base.sides == 8) return {1, 6};
                if (base.sides == 6) return {1, 4};
                if (base.sides == 4) return {1, 3};
            } else if (base.count == 2 && base.sides == 6) return {1, 8};
        } else if (currentSize == Size::LARGE) {
            if (base.count == 1) {
                if (base.sides == 6) return {1, 8};
                if (base.sides == 8) return {2, 6};
                if (base.sides == 10) return {2, 8};
                if (base.sides == 12) return {3, 6};
            } else if (base.count == 2 && base.sides == 6) return {3, 6};
        }
        return base;
    }

    static Race getRace(RaceType type) {
        static std::map<RaceType, Race> races = {
            {RaceType::HUMAN, {"Humano", RaceType::HUMAN, {0,0,0,0,0,0}, 30, 0, Size::MEDIUM}},
            {RaceType::ELF, {"Elfo", RaceType::ELF, {0,2,-2,0,0,0}, 30, 0, Size::MEDIUM}},
            {RaceType::DWARF, {"Anão", RaceType::DWARF, {0,0,2,0,0,-2}, 20, 0, Size::MEDIUM}},
            {RaceType::HALFLING, {"Halfling", RaceType::HALFLING, {-2,2,0,0,0,0}, 20, 0, Size::SMALL}},
            {RaceType::GNOME, {"Gnomo", RaceType::GNOME, {-2,0,2,0,0,0}, 20, 0, Size::SMALL}},
            {RaceType::HALF_ELF, {"Meio-Elfo", RaceType::HALF_ELF, {0,0,0,0,0,0}, 30, 0, Size::MEDIUM}},
            {RaceType::HALF_ORC, {"Meio-Orc", RaceType::HALF_ORC, {2,0,0,-2,0,-2}, 30, 0, Size::MEDIUM}},
            {RaceType::AASIMAR, {"Aasimar", RaceType::AASIMAR, {0,0,0,0,2,2}, 30, 1, Size::MEDIUM}},
            {RaceType::TIEFLING, {"Tiefling", RaceType::TIEFLING, {0,2,0,2,0,-2}, 30, 1, Size::MEDIUM}},
            {RaceType::DROW, {"Drow", RaceType::DROW, {0,2,-2,2,0,2}, 30, 2, Size::MEDIUM}},
            {RaceType::OGRE, {"Ogre", RaceType::OGRE, {10, -2, 4, -4, 0, -2}, 40, 2, Size::LARGE}}
        };
        return races[type];
    }

    static CharacterClass getClass(ClassType type) {
        static std::map<ClassType, CharacterClass> classes = {
            {ClassType::BARBARIAN, {"Bárbaro", ClassType::BARBARIAN, 12, BabProgression::GOOD, true, false, false}},
            {ClassType::FIGHTER, {"Guerreiro", ClassType::FIGHTER, 10, BabProgression::GOOD, true, false, false}},
            {ClassType::ROGUE, {"Ladino", ClassType::ROGUE, 6, BabProgression::AVERAGE, false, true, false}},
            {ClassType::WIZARD, {"Mago", ClassType::WIZARD, 4, BabProgression::POOR, false, false, true}},
            // Adicionar as demais conforme necessário
        };
        return classes[type];
    }
};

struct Character {
    std::string name;
    RaceType raceType;
    std::map<ClassType, int> levels;
    Attributes baseAttributes;
    std::map<SkillType, int> skillRanks;
    std::set<Condition> conditions;
    std::map<Condition, int> conditionDurations; // Duração em rounds
    std::set<Feat> feats;
    std::string focusedWeapon; // Para Weapon Focus
    Armor equippedArmor = {"Nenhuma", 0, 99, 0};
    Weapon equippedWeapon = {"Mãos Nuas", {1, 3}};

    // Acessórios
    std::map<AccessorySlot, Accessory> accessories;

    int x, y;
    float visualX, visualY; // Para animação suave
    int gold = 0;

    // Magia
    std::vector<std::string> spellsKnown;
    std::map<int, int> spellSlotsUsed;

    // Progressão
    int currentXp = 0;
    int currentHp = 0;

    // Sistema de Descanso
    int restsToday = 0;
    long long lastRestFinishTime = 0; // Timestamp em segundos
    long long lastResetDate = 0; // Para resetar restsToday à meia-noite

    void recoverFull() {
        currentHp = getMaxHp();
        spellSlotsUsed.clear();
    }

    int getNextRestDurationMinutes() const {
        // 1, 2, 4, 8, 16...
        return std::pow(2, restsToday);
    }

    int getMaxSpellsKnown(ClassType cls, int spellLevel) const {
        int classLvl = levels.count(cls) ? levels.at(cls) : 0;
        if (classLvl == 0) return 0;

        if (cls == ClassType::WIZARD) {
            if (spellLevel == 1) return std::min(5, 4 + (classLvl - 1));
            if (spellLevel == 2 && classLvl >= 3) return std::min(5, 2 + (classLvl - 3));
            if (spellLevel == 3 && classLvl >= 5) return std::min(5, 1 + (classLvl - 5));
        }
        else if (cls == ClassType::SORCERER) {
            if (spellLevel == 1) return 3 + (classLvl - 1);
            if (spellLevel >= 2) return 1 + (classLvl - (spellLevel * 2 - 1));
        }
        else if (cls == ClassType::DRUID || cls == ClassType::CLERIC) {
            if (spellLevel == 1) return std::min(5, 3 + (classLvl - 1));
            if (spellLevel >= 2) return std::min(5, 1 + (classLvl - (spellLevel * 2 - 1)));
        }
        return 0;
    }

    int getMaxSpellSlots(ClassType cls, int spellLevel) const {
        int classLvl = levels.count(cls) ? levels.at(cls) : 0;
        if (classLvl == 0) return 0;

        // Mago ganha 1 slot por nível até o máximo de 5
        if (cls == ClassType::WIZARD) {
            int startLvl = (spellLevel - 1) * 2 + 1;
            if (classLvl < startLvl) return 0;
            return std::min(5, 1 + (classLvl - startLvl));
        }
        // Feiticeiro, Druida e Clérigo seguem lógica similar de progressão
        int startLvl = (spellLevel == 1) ? 1 : (spellLevel * 2 - 1);
        if (classLvl < startLvl) return 0;
        return std::min(5, 1 + (classLvl - startLvl));
    }

    // Referência para o Companheiro Animal
    struct AnimalCompanion* companion = nullptr;

    bool hasCondition(Condition c) const { return conditions.find(c) != conditions.end(); }
    bool hasFeat(Feat f) const { return feats.find(f) != feats.end(); }

    int getModifierWithRace(AttributeType type) const {
        int val = baseAttributes.values[static_cast<int>(type)];
        val += RpgData::getRace(raceType).attributeModifiers[static_cast<int>(type)];

        // Adiciona bônus de acessórios
        for (auto const& [slot, acc] : accessories) {
            if (acc.bonusAttribute == type) {
                val += acc.bonusValue;
            }
        }

        // Bônus de Fúria (Bárbaro)
        if (hasCondition(Condition::RAGING)) {
            if (type == AttributeType::STR || type == AttributeType::CON) val += 4;
        }

        if (type == AttributeType::DEX && hasCondition(Condition::ENTANGLED)) val -= 4;
        return (val - 10) / 2;
    }

    int getSkillBonus(SkillType skill) const {
        int ranks = skillRanks.count(skill) ? skillRanks.at(skill) : 0;
        int abilityMod = 0;
        int sizeMod = 0;
        int penalty = 0;

        switch (skill) {
            case SkillType::ATHLETICS:
                abilityMod = getModifierWithRace(AttributeType::STR);
                penalty = equippedArmor.armorCheckPenalty;
                break;
            case SkillType::STEALTH:
                abilityMod = getModifierWithRace(AttributeType::DEX);
                sizeMod = RpgData::getStealthSizeModifier(RpgData::getRace(raceType).size);
                penalty = equippedArmor.armorCheckPenalty;
                break;
            case SkillType::SPOT: abilityMod = getModifierWithRace(AttributeType::WIS); break;
            case SkillType::BLUFF: abilityMod = getModifierWithRace(AttributeType::CHA); break;
            case SkillType::KNOWLEDGE_ARCANA:
            case SkillType::KNOWLEDGE_MONSTERS: abilityMod = getModifierWithRace(AttributeType::INT); break;
        }
        return ranks + abilityMod + sizeMod - penalty;
    }

    // CA = 10 + Bônus Armadura + Melhoria + Mod Dex (limitado) + Mod Tamanho + Condições
    int getAC(bool isMelee = true) const {
        int ac = 10 + equippedArmor.armorBonus + equippedArmor.enhancement;

        int dexMod = getModifierWithRace(AttributeType::DEX);
        // Se atordoado ou cego, perde bônus de Destreza
        if (hasCondition(Condition::STUNNED) || hasCondition(Condition::BLINDED)) {
            dexMod = std::min(0, dexMod);
        } else {
            dexMod = std::min(dexMod, equippedArmor.maxDexBonus);
        }

        ac += dexMod;
        ac += RpgData::getSizeModifier(RpgData::getRace(raceType).size);

        // Modificadores de Condição
        if (hasCondition(Condition::PRONE)) ac += isMelee ? -4 : 4;
        if (hasCondition(Condition::STUNNED)) ac -= 2;
        if (hasCondition(Condition::BLINDED)) ac -= 2;
        if (hasCondition(Condition::RAGING)) ac -= 2;

        return ac;
    }

    int getBAB() const {
        int totalBab = 0;
        for (auto const& [type, level] : levels) {
            CharacterClass cls = RpgData::getClass(type);
            if (cls.babType == BabProgression::GOOD) totalBab += level;
            else if (cls.babType == BabProgression::AVERAGE) totalBab += (level * 3) / 4;
            else totalBab += level / 2;
        }
        return totalBab;
    }

    int getMeleeAttackBonus() const {
        int atk = getBAB() + getModifierWithRace(AttributeType::STR) +
                  RpgData::getSizeModifier(RpgData::getRace(raceType).size) +
                  equippedWeapon.enhancement;

        // Habilidade de Lutador (Fighter): Foco em combate (+1 passivo)
        if (levels.count(ClassType::FIGHTER)) atk += 1;

        // Bônus de Inspiração do Bardo
        if (hasCondition(Condition::INSPIRED)) atk += 1;

        if (hasCondition(Condition::PRONE)) atk -= 4;
        if (hasCondition(Condition::ENTANGLED)) atk -= 2;
        return atk;
    }

    int getRangedAttackBonus() const {
        int atk = getBAB() + getModifierWithRace(AttributeType::DEX) +
                  RpgData::getSizeModifier(RpgData::getRace(raceType).size) +
                  equippedWeapon.enhancement;

        // Bônus de Inspiração do Bardo
        if (hasCondition(Condition::INSPIRED)) atk += 1;

        if (hasCondition(Condition::ENTANGLED)) atk -= 2;
        return atk;
    }

    int getBaseSave(bool isGood, int level) const {
        if (level <= 0) return 0;
        if (isGood) return 2 + (level / 2);
        return level / 3;
    }

    int getTotalFortSave() const {
        int total = 0;
        for (auto const& [type, level] : levels) {
            total += getBaseSave(RpgData::getClass(type).goodFort, level);
        }
        if (hasFeat(Feat::GREAT_FORTITUDE)) total += 2;
        return total + getModifierWithRace(AttributeType::CON);
    }

    int getTotalRefSave() const {
        int total = 0;
        for (auto const& [type, level] : levels) {
            total += getBaseSave(RpgData::getClass(type).goodRef, level);
        }
        if (hasFeat(Feat::LIGHTNING_REFLEXES)) total += 2;
        return total + getModifierWithRace(AttributeType::DEX);
    }

    int getTotalWillSave() const {
        int total = 0;
        for (auto const& [type, level] : levels) {
            total += getBaseSave(RpgData::getClass(type).goodWill, level);
        }
        if (hasFeat(Feat::IRON_WILL)) total += 2;
        return total + getModifierWithRace(AttributeType::WIS);
    }

    int getMaxHp() const {
        int totalHp = 0;
        int conMod = getModifierWithRace(AttributeType::CON);
        for (int roll : hpRolls) {
            totalHp += std::max(1, roll + conMod);
        }
        if (hasFeat(Feat::TOUGHNESS)) totalHp += 3;
        return totalHp;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_RPG_SYSTEM_H
