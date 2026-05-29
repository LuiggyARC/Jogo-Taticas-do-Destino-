#ifndef TATICAS_DO_DESTINO_SAVE_MANAGER_H
#define TATICAS_DO_DESTINO_SAVE_MANAGER_H

#include <fstream>
#include <iostream>
#include "RpgSystem.h"
#include "MapManager.h"
#include "AndroidOut.h"

namespace rpg {

class SaveManager {
public:
    // O caminho do arquivo deve vir da JNI (FilesDir do Android)
    static bool saveGame(const std::string& filePath, const Character& player, const GameState& state) {
        std::ofstream file(filePath);
        if (!file.is_open()) {
            aout << "Erro ao abrir arquivo para salvar: " << filePath << std::endl;
            return false;
        }

        // Salva Dados Básicos
        file << player.name << "\n";
        file << static_cast<int>(player.raceType) << "\n";
        file << player.gold << "\n";
        file << player.currentXp << "\n";
        file << player.currentHp << "\n";
        file << state.currentLevelIndex << "\n";

        // Salva Níveis
        file << player.levels.size() << "\n";
        for (auto const& [type, lvl] : player.levels) {
            file << static_cast<int>(type) << " " << lvl << "\n";
        }

        // Salva Equipamentos
        file << player.equippedWeapon.name << " " << player.equippedWeapon.enhancement << "\n";
        file << player.equippedArmor.name << " " << player.equippedArmor.enhancement << "\n";

        // Salva Atributos Base (6 valores)
        for(int i=0; i<6; ++i) file << player.baseAttributes.values[i] << " ";
        file << "\n";

        // Salva Talentos
        file << player.feats.size() << "\n";
        for (auto f : player.feats) {
            file << static_cast<int>(f) << " ";
        }
        file << "\n";

        file.close();
        aout << "Jogo salvo com sucesso em: " << filePath << std::endl;
        return true;
    }

    static bool loadGame(const std::string& filePath, Character& player, GameState& state) {
        std::ifstream file(filePath);
        if (!file.is_open()) {
            aout << "Arquivo de save não encontrado: " << filePath << std::endl;
            return false;
        }

        // Carrega Dados Básicos
        file >> player.name;
        int raceIdx;
        file >> raceIdx; player.raceType = static_cast<RaceType>(raceIdx);
        file >> player.gold;
        file >> player.currentXp;
        file >> player.currentHp;
        file >> state.currentLevelIndex;

        // Carrega Níveis
        size_t numLevels;
        file >> numLevels;
        player.levels.clear();
        for (size_t i = 0; i < numLevels; ++i) {
            int typeIdx, lvl;
            file >> typeIdx >> lvl;
            player.levels[static_cast<ClassType>(typeIdx)] = lvl;
        }

        // Carrega Equipamentos (Simplificado para o nome base)
        std::string wName, aName;
        int wEnhance, aEnhance;
        file >> wName >> wEnhance;
        file >> aName >> aEnhance;
        // Aqui você usaria ItemData para pegar as stats do item pelo nome
        // e então aplicaria o enhancement salvo.

        // Carrega Atributos
        for(int i=0; i<6; ++i) file >> player.baseAttributes.values[i];

        // Carrega Talentos
        size_t numFeats;
        file >> numFeats;
        player.feats.clear();
        for (size_t i = 0; i < numFeats; ++i) {
            int fIdx;
            file >> fIdx;
            player.feats.insert(static_cast<Feat>(fIdx));
        }

        file.close();
        aout << "Jogo carregado de: " << filePath << std::endl;
        return true;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_SAVE_MANAGER_H
