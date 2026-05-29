#ifndef TATICAS_DO_DESTINO_DICE_H
#define TATICAS_DO_DESTINO_DICE_H

#include <random>
#include <vector>
#include <algorithm>

namespace rpg {

class Dice {
public:
    static int roll(int sides) {
        if (sides <= 0) return 0;
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, sides);
        return dis(gen);
    }

    static int rollMultiple(int count, int sides) {
        int total = 0;
        for (int i = 0; i < count; ++i) total += roll(sides);
        return total;
    }

    // Método 4d6 (descarta o menor) para geração de atributos
    static int rollAttribute() {
        std::vector<int> rolls;
        for(int i = 0; i < 4; ++i) {
            rolls.push_back(roll(6));
        }
        std::sort(rolls.begin(), rolls.end());
        // Soma os 3 maiores (os índices 1, 2 e 3 após o sort)
        return rolls[1] + rolls[2] + rolls[3];
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_DICE_H
