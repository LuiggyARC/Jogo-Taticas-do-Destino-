#ifndef TATICAS_DO_DESTINO_ENCOUNTER_MANAGER_H
#define TATICAS_DO_DESTINO_ENCOUNTER_MANAGER_H

#include "EnemyData.h"
#include "MapManager.h"
#include "Dice.h"
#include <vector>

namespace rpg {

struct EnemyInstance {
    EnemyTemplate stats;
    int currentHp;
    int x, y;
    float visualX, visualY;
};

class EncounterManager {
public:
    static std::vector<EnemyInstance> spawnEnemies(Difficulty diff, int mapWidth, int mapHeight) {
        MapConfig config = MapManager::getMapConfig(diff);
        int numEnemies = config.minEnemies + (rand() % (config.maxEnemies - config.minEnemies + 1));

        std::vector<EnemyInstance> spawned;
        std::vector<EnemyTemplate> pool;

        if (diff == Difficulty::HARD) {
            // Mistura Médios (Tier 2) e Difíceis (Tier 3)
            auto mid = EnemyData::getEnemiesByTier(2);
            auto high = EnemyData::getEnemiesByTier(3);
            pool.insert(pool.end(), mid.begin(), mid.end());
            pool.insert(pool.end(), high.begin(), high.end());
        } else {
            // Mistura Fáceis (Tier 1) e Médios (Tier 2) para EASY e MEDIUM
            auto low = EnemyData::getEnemiesByTier(1);
            auto mid = EnemyData::getEnemiesByTier(2);
            pool.insert(pool.end(), low.begin(), low.end());
            pool.insert(pool.end(), mid.begin(), mid.end());
        }

        for (int i = 0; i < numEnemies; ++i) {
            EnemyTemplate t = pool[rand() % pool.size()];

            // Spawn na metade oposta do mapa (ex: x entre width/2 e width-1)
            int spawnX = (mapWidth / 2) + (rand() % (mapWidth / 2));
            int spawnY = rand() % mapHeight;

            spawned.push_back({t, t.hp, spawnX, spawnY});
        }

        return spawned;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_ENCOUNTER_MANAGER_H
