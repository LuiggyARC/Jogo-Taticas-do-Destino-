#ifndef TATICAS_DO_DESTINO_AI_MANAGER_H
#define TATICAS_DO_DESTINO_AI_MANAGER_H

#include "RpgSystem.h"
#include "EncounterManager.h"
#include "Grid.h"
#include "Combat.h"
#include "MapManager.h"

namespace rpg {

class AiManager {
public:
    static void executeEnemyTurn(EnemyInstance& enemy, Character& player, const MapConfig& map) {
        int dist = Grid::calculateDistance({enemy.x, enemy.y}, {player.x, player.y});
        int speed = 6; // 30 pés = 6 quadrados

        // Verifica Linha de Visão para ataques
        bool hasLoS = (enemy.stats.specialAbility == MonsterAbility::ETHEREAL) ||
                      Grid::hasLineOfSight({enemy.x, enemy.y}, {player.x, player.y}, map);

        // 1. TÁTICA DE SOBREVIVÊNCIA E REGENERAÇÃO
        if (enemy.stats.specialAbility == MonsterAbility::REGENERATION && enemy.currentHp < (enemy.stats.hp / 2)) {
            moveToNearestCover(enemy, player, map, speed);
            return;
        }

        // 2. TÁTICA DE ÁREA (Ex: Beholder ou Magos Inimigos)
        if (enemy.stats.specialAbility == MonsterAbility::EYE_RAY && dist <= 12) {
            if (hasLoS) {
                // Se tiver LoS, ele ataca. Se não, se move para ganhar LoS.
                // Lógica de ataque seria chamada aqui no fluxo do jogo
            } else {
                moveToGainLoS(enemy, player, map, speed);
            }
            return;
        }

        // 3. TÁTICA DE DISTÂNCIA E TERRENO
        if (enemy.stats.specialAbility == MonsterAbility::POISON_STING) {
            if (dist < 4) {
                moveAwayAndFindCover(enemy, player, map, speed);
            } else if (!hasLoS) {
                moveToGainLoS(enemy, player, map, speed);
            }
            return;
        }

        // 4. TÁTICA DE TAMANHO E ALCANCE
        if (enemy.stats.specialAbility == MonsterAbility::REACH_ATTACK) {
            if (dist > 2) moveTowards(enemy, player, speed, map);
            else if (dist < 2) moveAway(enemy, player, 1, map);
            return;
        }

        // 5. TÁTICA DE AGRESSÃO
        moveTowards(enemy, player, enemy.stats.specialAbility == MonsterAbility::POUNCE ? speed + 2 : speed, map);
    }

    // Encontra o melhor ponto para soltar uma magia de área (ex: Bola de Fogo)
    // para atingir o jogador sem atingir outros inimigos
    static Point findBestAreaTarget(const EnemyInstance& caster, const Character& player,
                                   const std::vector<EnemyInstance>& allies, int radius) {
        Point target = {player.x, player.y};

        // Verifica se atingiria algum aliado
        for (const auto& ally : allies) {
            if (&ally == &caster) continue;
            int distToAlly = Grid::calculateDistance(target, {ally.x, ally.y});
            if (distToAlly <= radius) {
                // Se atingir aliado, tenta mover o centro da área ligeiramente
                // para longe do aliado mas ainda pegando o player
                if (target.x < ally.x) target.x -= 1; else target.x += 1;
                if (target.y < ally.y) target.y -= 1; else target.y += 1;
            }
        }
        return target;
    }

private:
    static void moveToGainLoS(EnemyInstance& enemy, const Character& target, const MapConfig& map, int speed) {
        // Tenta se mover lateralmente para contornar obstáculos e ganhar visão
        for (int i = 0; i < speed; ++i) {
            if (Grid::hasLineOfSight({enemy.x, enemy.y}, {target.x, target.y}, map)) break;

            // Move-se na direção do alvo mas com um leve desvio lateral
            int dx = (target.x > enemy.x) ? 1 : -1;
            int dy = (target.y > enemy.y) ? 1 : -1;

            // Tenta mover em Y se X estiver bloqueado e vice-versa (contorno)
            if (map.getTile(enemy.x + dx, enemy.y) != TileType::OBSTACLE) enemy.x += dx;
            else if (map.getTile(enemy.x, enemy.y + dy) != TileType::OBSTACLE) enemy.y += dy;
        }
    }
    static void moveTowards(EnemyInstance& enemy, const Character& target, int totalMovementPoints, const MapConfig& map) {
        int pointsSpent = 0;
        bool isEthereal = (enemy.stats.specialAbility == MonsterAbility::ETHEREAL);

        while (pointsSpent < totalMovementPoints) {
            int nextX = enemy.x + (enemy.x < target.x ? 1 : (enemy.x > target.x ? -1 : 0));
            int nextY = enemy.y + (enemy.y < target.y ? 1 : (enemy.y > target.y ? -1 : 0));

            if (nextX == enemy.x && nextY == enemy.y) break;

            TileType tile = map.getTile(nextX, nextY);
            int cost = (tile == TileType::DIFFICULT && !isEthereal) ? 2 : 1;

            if (pointsSpent + cost > totalMovementPoints) break;

            if (isEthereal || tile != TileType::OBSTACLE) {
                enemy.x = nextX;
                enemy.y = nextY;
                pointsSpent += cost;
            } else {
                break;
            }
        }
    }

    static void moveAway(EnemyInstance& enemy, const Character& target, int totalMovementPoints, const MapConfig& map) {
        int pointsSpent = 0;
        bool isEthereal = (enemy.stats.specialAbility == MonsterAbility::ETHEREAL);

        while (pointsSpent < totalMovementPoints) {
            int nextX = enemy.x + (enemy.x < target.x ? -1 : 1);
            int nextY = enemy.y + (enemy.y < target.y ? -1 : 1);

            if (!Grid::isWithinBounds({nextX, nextY}, map.width, map.height)) break;

            TileType tile = map.getTile(nextX, nextY);
            int cost = (tile == TileType::DIFFICULT && !isEthereal) ? 2 : 1;

            if (pointsSpent + cost > totalMovementPoints) break;

            if (isEthereal || tile != TileType::OBSTACLE) {
                enemy.x = nextX;
                enemy.y = nextY;
                pointsSpent += cost;
            } else {
                break;
            }
        }
    }

    static void moveToNearestCover(EnemyInstance& enemy, const Character& target, const MapConfig& map, int speed) {
        // Busca simples por tile de COVER ao redor
        for (int dx = -speed; dx <= speed; ++dx) {
            for (int dy = -speed; dy <= speed; ++dy) {
                int tx = enemy.x + dx;
                int ty = enemy.y + dy;
                if (map.getTile(tx, ty) == TileType::COVER) {
                    enemy.x = tx;
                    enemy.y = ty;
                    return;
                }
            }
        }
        moveAway(enemy, target, speed, map);
    }

    static void moveAwayAndFindCover(EnemyInstance& enemy, const Character& target, const MapConfig& map, int speed) {
        moveToNearestCover(enemy, target, map, speed);
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_AI_MANAGER_H
