#ifndef TATICAS_DO_DESTINO_MAP_MANAGER_H
#define TATICAS_DO_DESTINO_MAP_MANAGER_H

#include <vector>
#include <string>

namespace rpg {

enum class Difficulty {
    EASY,   // 10x10
    MEDIUM, // 15x15
    HARD    // 20x20
};

enum class TileType {
    FLAT,      // Terreno normal
    OBSTACLE,  // Bloqueia movimento e linha de visão
    COVER,     // Fornece Cobertura Parcial (+2 CA)
    DIFFICULT  // Custo de movimento dobrado
};

struct MapConfig {
    int width;
    int height;
    int minEnemies;
    int maxEnemies;
    std::vector<TileType> tiles; // Grid de tiles

    TileType getTile(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height) return TileType::OBSTACLE;
        return tiles[y * width + x];
    }
};

struct Level {
    std::string name;
    std::string description;
    Difficulty difficulty;
    int levelNumber;
};

class MapManager {
public:
    static MapConfig getMapConfig(Difficulty diff) {
        switch (diff) {
            case Difficulty::EASY:
                return {10, 10, 2, 4};
            case Difficulty::MEDIUM:
                return {15, 15, 5, 8};
            case Difficulty::HARD:
                return {20, 20, 10, 15};
            default:
                return {10, 10, 0, 0};
        }
    }

    // A trilha completa da história (20 níveis)
    static std::vector<Level> getStoryTrail() {
        return {
            {"Cidade Inicial", "Apresentação ao mundo e preparativos.", Difficulty::EASY, 0},
            {"Arredores da Cidade", "Primeiros encontros na estrada.", Difficulty::EASY, 1},
            {"Floresta do Destino", "Onde as sombras das árvores escondem segredos.", Difficulty::EASY, 2},
            {"Cavernas Ecoantes", "Exploração subterrânea perigosa.", Difficulty::MEDIUM, 3},
            {"Retorno à Cidade", "Um breve descanso antes da próxima jornada.", Difficulty::EASY, 4},
            {"Pântano da Bruxa", "Terreno traiçoeiro e magias obscuras.", Difficulty::HARD, 5},
            {"Ruínas do Destino", "Antigas estruturas repletas de perigo.", Difficulty::HARD, 6},
            {"Retorno à Cidade", "Abastecendo provisões para o deserto.", Difficulty::EASY, 7},
            {"Deserto da Perdição", "Calor escaldante e inimigos implacáveis.", Difficulty::MEDIUM, 8},
            {"Ruínas das Lembranças", "Enfrente o passado para salvar o futuro.", Difficulty::HARD, 9},
            {"Torre do Enigma", "Quebra-cabeças e combates mentais.", Difficulty::MEDIUM, 10},
            {"Cidade do Porto", "Conhecendo a vida costeira.", Difficulty::EASY, 11},
            {"Arredores da Cidade", "Exploração da orla marítima.", Difficulty::EASY, 12},
            {"Um Dia de Marujo", "Batalha em alto mar durante uma tempestade.", Difficulty::HARD, 13},
            {"Chegada: Ilha Fantasma", "Névoas espessas e inimigos etéreos.", Difficulty::MEDIUM, 14},
            {"Libertação dos Espíritos", "O confronto contra o medo da morte.", Difficulty::MEDIUM, 15},
            {"Retorno ao Porto", "Relatando os feitos na ilha.", Difficulty::MEDIUM, 16},
            {"O Maior Inimigo", "A sombra que te persegue se revela.", Difficulty::HARD, 17},
            {"A Busca do Poder", "Preparação final para o trono.", Difficulty::HARD, 19},
            {"Batalha pela Glória Final", "O confronto épico que decidirá o destino.", Difficulty::HARD, 20}
        };
    }
};

struct GameState {
    int currentLevelIndex = 0;

    // Seleção
    bool hasSelection = false;
    int selectedX = -1, selectedY = -1;
    void* selectedEntity = nullptr; // Character* ou EnemyInstance*
    bool isEnemySelected = false;

    Level getCurrentLevel() {
        return MapManager::getStoryTrail()[currentLevelIndex];
    }

    MapConfig getCurrentMapConfig() {
        return MapManager::getMapConfig(getCurrentLevel().difficulty);
    }

    void selectTile(int x, int y, Character& player, std::vector<EnemyInstance>& enemies) {
        selectedX = x;
        selectedY = y;
        hasSelection = true;
        selectedEntity = nullptr;

        if (player.x == x && player.y == y) {
            selectedEntity = &player;
            isEnemySelected = false;
        } else {
            for (auto& e : enemies) {
                if (e.x == x && e.y == y && e.currentHp > 0) {
                    selectedEntity = &e;
                    isEnemySelected = true;
                    break;
                }
            }
        }
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_MAP_MANAGER_H
