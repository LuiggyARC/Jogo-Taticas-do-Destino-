#ifndef TATICAS_DO_DESTINO_TURN_MANAGER_H
#define TATICAS_DO_DESTINO_TURN_MANAGER_H

#include "RpgSystem.h"
#include "EncounterManager.h"
#include "Combat.h"
#include <vector>
#include <variant>

namespace rpg {

struct TurnEntity {
    bool isPlayer;
    void* entityPtr; // Ponteiro para Character ou EnemyInstance
    int initiative;

    Character* asCharacter() { return isPlayer ? static_cast<Character*>(entityPtr) : nullptr; }
    EnemyInstance* asEnemy() { return !isPlayer ? static_cast<EnemyInstance*>(entityPtr) : nullptr; }
};

class TurnManager {
public:
    std::vector<TurnEntity> initiativeOrder;
    int currentTurnIndex = 0;
    int roundNumber = 1;

    void startCombat(Character* player, std::vector<EnemyInstance>& enemies) {
        initiativeOrder.clear();

        // Rola iniciativa do jogador
        int pInit = Dice::roll(20) + player->getModifierWithRace(AttributeType::DEX);
        initiativeOrder.push_back({true, player, pInit});

        // Rola iniciativa dos inimigos
        for (auto& enemy : enemies) {
            int eInit = Dice::roll(20) + (enemy.stats.attackBonus / 2); // Simplificado
            initiativeOrder.push_back({false, &enemy, eInit});
        }

        // Ordena por iniciativa
        std::sort(initiativeOrder.begin(), initiativeOrder.end(), [](const TurnEntity& a, const TurnEntity& b) {
            return a.initiative > b.initiative;
        });

        currentTurnIndex = 0;
        roundNumber = 1;
    }

    TurnEntity getCurrentEntity() {
        return initiativeOrder[currentTurnIndex];
    }

    void nextTurn() {
        // Reduz duração de condições do ser que acabou de agir
        TurnEntity entity = getCurrentEntity();
        Character* c = entity.asCharacter();
        if (c) {
            auto it = c->conditionDurations.begin();
            while (it != c->conditionDurations.end()) {
                it->second--;
                if (it->second <= 0) {
                    c->conditions.erase(it->first);
                    it = c->conditionDurations.erase(it);
                } else {
                    ++it;
                }
            }
        }

        currentTurnIndex++;
        if (currentTurnIndex >= initiativeOrder.size()) {
            currentTurnIndex = 0;
            roundNumber++;
            // Aqui poderíamos processar efeitos de início de round (regeneração, etc)
        }
    }

    bool isCombatOver(const Character& player, const std::vector<EnemyInstance>& enemies) {
        if (player.currentHp <= 0) return true;
        for (const auto& e : enemies) {
            if (e.currentHp > 0) return false;
        }
        return true;
    }

    struct TurnViewInfo {
        std::string name;
        bool isPlayer;
        int initiative;
        bool isCurrent;
    };

    std::vector<TurnViewInfo> getTurnOrderView() {
        std::vector<TurnViewInfo> view;
        for (int i = 0; i < initiativeOrder.size(); ++i) {
            auto& entity = initiativeOrder[i];
            std::string name = entity.isPlayer ? "Jogador" : static_cast<EnemyInstance*>(entity.entityPtr)->stats.name;
            view.push_back({name, entity.isPlayer, entity.initiative, i == currentTurnIndex});
        }
        return view;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_TURN_MANAGER_H
