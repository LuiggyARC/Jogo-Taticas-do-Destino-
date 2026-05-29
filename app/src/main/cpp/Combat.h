#ifndef TATICAS_DO_DESTINO_COMBAT_H
#define TATICAS_DO_DESTINO_COMBAT_H

#include "RpgSystem.h"
#include "Dice.h"
#include "Grid.h"
#include <vector>
#include <algorithm>

namespace rpg {

enum class CoverType {
    NONE,
    PARTIAL, // +2 AC
    TOTAL    // +4 AC
};

struct Combatant {
    Character* character;
    int initiative;

    bool operator>(const Combatant& other) const {
        return initiative > other.initiative;
    }
};

class CombatManager {
public:
    std::vector<Combatant> order;

    void rollInitiative(std::vector<Character*>& characters) {
        order.clear();
        for (auto* c : characters) {
            int roll = Dice::roll(20) + c->getModifierWithRace(AttributeType::DEX);
            order.push_back({c, roll});
        }
        // Ordena do maior para o menor
        std::sort(order.begin(), order.end(), std::greater<Combatant>());
    }

    struct AttackResult {
        bool hit;
        int roll;
        int damage;
    };

    static AttackResult resolveMeleeAttack(Character* attacker, Character* target, Weapon weapon, int powerAttackValue = 0) {
        int roll = Dice::roll(20);

        // Se atacante está atordoado, não pode atacar
        if (attacker->hasCondition(Condition::STUNNED)) {
            return {false, 0, 0};
        }

        int totalAttack = roll + attacker->getMeleeAttackBonus();

        // Bônus por flanquear
        if (target->hasCondition(Condition::FLANKED)) {
            totalAttack += 2;
        }

        // Talentos
        if (attacker->hasFeat(Feat::WEAPON_FOCUS) && attacker->focusedWeapon == weapon.name) {
            totalAttack += 1;
        }

        if (attacker->hasFeat(Feat::POWER_ATTACK)) {
            totalAttack -= powerAttackValue;
        }

        int targetAC = target->getAC(true); // true para ataque corpo a corpo

        AttackResult result;
        result.roll = roll;
        result.hit = (totalAttack >= targetAC);

        if (result.hit) {
            DamageDice scaledDamage = RpgData::scaleWeaponDamage(
                weapon.mediumDamage,
                RpgData::getRace(attacker->raceType).size
            );

            int damage = Dice::rollMultiple(scaledDamage.count, scaledDamage.sides) +
                             attacker->getModifierWithRace(AttributeType::STR);

            // Bônus de Inspiração do Bardo no dano
            if (attacker->hasCondition(Condition::INSPIRED)) damage += 1;

            // Ataque Furtivo (Rogue): +1d6 por cada 2 níveis se estiver flanqueando
            if (attacker->levels.count(ClassType::ROGUE) && target->hasCondition(Condition::FLANKED)) {
                int rogueLvl = attacker->levels.at(ClassType::ROGUE);
                int sneakDice = (rogueLvl + 1) / 2;
                damage += Dice::rollMultiple(sneakDice, 6);
            }

            // Destruir o Mal (Paladin): +Nível no dano
            if (attacker->levels.count(ClassType::PALADIN)) {
                damage += attacker->levels.at(ClassType::PALADIN);
            }

            if (attacker->hasFeat(Feat::POWER_ATTACK)) {
                damage += powerAttackValue; // Simplificado: 1 para 1
            }

            result.damage = std::max(1, damage);
        } else {
            result.damage = 0;
        }

        return result;
    }

    static AttackResult resolveRangedAttack(Character* attacker, Character* target, Weapon weapon, CoverType cover = CoverType::NONE) {
        int roll = Dice::roll(20);

        if (attacker->hasCondition(Condition::STUNNED)) {
            return {false, 0, 0};
        }

        // Cálculo de Distância no Grid
        int distance = Grid::calculateDistance({attacker->x, attacker->y}, {target->x, target->y}) * 5; // em pés

        int rangePenalty = 0;
        if (weapon.rangeIncrement > 0) {
            // Penalidade de -2 para cada incremento de distância além do primeiro
            rangePenalty = (distance / weapon.rangeIncrement) * 2;
        }

        int featAttackBonus = 0;
        int featDamageBonus = 0;

        // Point Blank Shot: +1 acerto/dano se estiver dentro de 30 pés
        if (attacker->hasFeat(Feat::POINT_BLANK_SHOT) && distance <= 30) {
            featAttackBonus += 1;
            featDamageBonus += 1;
        }

        // Penalidade de -4 por atirar em combate corpo a corpo
        int meleePenalty = 0;
        if (target->hasCondition(Condition::IN_MELEE) && !attacker->hasFeat(Feat::PRECISE_SHOT)) {
            meleePenalty = 4;
        }

        // Cobertura
        int coverBonus = 0;
        if (cover == CoverType::PARTIAL) coverBonus = 2;
        else if (cover == CoverType::TOTAL) coverBonus = 4;

        int totalAttack = roll + attacker->getRangedAttackBonus() - rangePenalty + featAttackBonus - meleePenalty;
        int targetAC = target->getAC(false) + coverBonus;

        AttackResult result;
        result.roll = roll;
        result.hit = (totalAttack >= targetAC);

        if (result.hit) {
            DamageDice scaledDamage = RpgData::scaleWeaponDamage(
                weapon.mediumDamage,
                RpgData::getRace(attacker->raceType).size
            );

            int damage = Dice::rollMultiple(scaledDamage.count, scaledDamage.sides) + featDamageBonus;

            // Adiciona bônus de Força se a arma permitir (Compostos, Fundas, Arremessáveis)
            if (weapon.addsStrDamage) {
                damage += attacker->getModifierWithRace(AttributeType::STR);
            }

            result.damage = std::max(1, damage);
        } else {
            result.damage = 0;
        }

        return result;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_COMBAT_H
