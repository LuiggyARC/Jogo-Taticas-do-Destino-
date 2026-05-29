#ifndef TATICAS_DO_DESTINO_ABILITY_MANAGER_H
#define TATICAS_DO_DESTINO_ABILITY_MANAGER_H

#include "RpgSystem.h"
#include "Combat.h"

namespace rpg {

class AbilityManager {
public:
    // Bárbaro: Inicia Fúria
    static bool useRage(Character* character) {
        if (character->levels.count(ClassType::BARBARIAN) && !character->hasCondition(Condition::RAGING)) {
            character->conditions.insert(Condition::RAGING);
            // Duração: 3 + Modificador de CON (após o bônus de fúria ser aplicado)
            int conMod = character->getModifierWithRace(AttributeType::CON);
            character->conditionDurations[Condition::RAGING] = 3 + conMod;
            return true;
        }
        return false;
    }

    // Bardo: Inspirar Coragem
    static void useInspireCourage(Character* bardo, std::vector<Character*>& allies) {
        if (bardo->levels.count(ClassType::BARD)) {
            for (auto* ally : allies) {
                ally->conditions.insert(Condition::INSPIRED);
                ally->conditionDurations[Condition::INSPIRED] = 5; // Duração padrão 5 rounds
            }
        }
    }

    // Monge: Rajada de Golpes (Flurry of Blows)
    // Retorna true se o monge puder fazer o ataque extra com penalidade de -2
    static bool canUseFlurry(Character* monk) {
        return monk->levels.count(ClassType::MONK);
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_ABILITY_MANAGER_H
