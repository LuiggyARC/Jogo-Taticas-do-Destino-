#ifndef TATICAS_DO_DESTINO_SPELL_DATA_H
#define TATICAS_DO_DESTINO_SPELL_DATA_H

#include "RpgSystem.h"
#include <map>
#include <vector>

namespace rpg {

class SpellData {
public:
    static Spell getSpell(const std::string& name) {
        static std::map<std::string, Spell> spells = {
            // Nível 0 - Arcanas
            {"Raio de Gelo", {"Raio de Gelo", 0, SpellSchool::EVOCATION, SaveType::NONE, 30, true, "1d3 de dano de frio"}},
            {"Mãos Mágicas", {"Mãos Mágicas", 0, SpellSchool::TRANSMUTATION, SaveType::NONE, 30, false, "Move objetos leves"}},
            {"Ler Magia", {"Ler Magia", 0, SpellSchool::DIVINATION, SaveType::NONE, 0, false, "Lê pergaminhos e grimórios"}},
            {"Detectar Magia", {"Detectar Magia", 0, SpellSchool::DIVINATION, SaveType::NONE, 60, false, "Detecta auras mágicas"}},
            {"Luz", {"Luz", 0, SpellSchool::EVOCATION, SaveType::NONE, 0, false, "Objeto brilha como tocha"}},

            // Nível 1 - Arcanas
            {"Mísseis Mágicos", {"Mísseis Mágicos", 1, SpellSchool::EVOCATION, SaveType::NONE, 150, true, "1d4+1 de dano por míssil"}},
            {"Armadura Arcana", {"Armadura Arcana", 1, SpellSchool::CONJURATION, SaveType::NONE, 0, false, "+4 bônus de armadura"}},
            {"Escudo", {"Escudo", 1, SpellSchool::ABJURATION, SaveType::NONE, 0, false, "+4 bônus de escudo, ignora mísseis mágicos"}},
            {"Sono", {"Sono", 1, SpellSchool::ENCHANTMENT, SaveType::WILL, 150, true, "Põe criaturas em sono profundo"}},
            {"Mãos Flamejantes", {"Mãos Flamejantes", 1, SpellSchool::EVOCATION, SaveType::REF, 15, true, "1d4 de dano de fogo/nível"}},

            // Nível 1 - Divinas (Clérigo)
            {"Curar Ferimentos Leves", {"Curar Ferimentos Leves", 1, SpellSchool::CONJURATION, SaveType::WILL, 0, true, "Cura 1d8+1/nível (máx +5)"}},
            {"Bênção", {"Bênção", 1, SpellSchool::ENCHANTMENT, SaveType::NONE, 50, false, "+1 ataque e saves contra medo"}},
            {"Comando", {"Comando", 1, SpellSchool::ENCHANTMENT, SaveType::WILL, 30, true, "Dá uma ordem de uma palavra"}},
            {"Escudo da Fé", {"Escudo da Fé", 1, SpellSchool::ABJURATION, SaveType::NONE, 0, true, "+2 bônus de desvio (+1 a cada 6 níveis)"}},
            {"Causar Ferimentos Leves", {"Causar Ferimentos Leves", 1, SpellSchool::NECROMANCY, SaveType::WILL, 0, true, "Dano 1d8+1/nível (máx +5)"}},

            // Adicionais Clérigo
            {"Resistência", {"Resistência", 0, SpellSchool::ABJURATION, SaveType::WILL, 0, true, "+1 nos testes de resistência"}},
            {"Virtude", {"Virtude", 0, SpellSchool::ABJURATION, SaveType::NONE, 0, false, "Alvo ganha 1 PV temporário"}},
            {"Orientação", {"Orientação", 0, SpellSchool::DIVINATION, SaveType::NONE, 0, false, "+1 num teste de habilidade/perícia"}},
            {"Curar Ferimentos Moderados", {"Curar Ferimentos Moderados", 2, SpellSchool::CONJURATION, SaveType::WILL, 0, true, "Cura 2d8+1/nível (máx +10)"}},
            {"Ajuda", {"Ajuda", 2, SpellSchool::ENCHANTMENT, SaveType::NONE, 0, false, "+1 ataque/saves, 1d8 PV temp"}},
            {"Imobilizar Pessoa", {"Imobilizar Pessoa", 2, SpellSchool::ENCHANTMENT, SaveType::WILL, 150, true, "Paralisa humanoide"}},
            {"Arma Espiritual", {"Arma Espiritual", 2, SpellSchool::EVOCATION, SaveType::NONE, 150, true, "Arma mágica ataca sozinha"}},
            {"Restauração Menor", {"Restauração Menor", 2, SpellSchool::CONJURATION, SaveType::NONE, 0, false, "Cura dano de atributo"}},
            {"Curar Ferimentos Graves", {"Curar Ferimentos Graves", 3, SpellSchool::CONJURATION, SaveType::WILL, 0, true, "Cura 3d8+1/nível (máx +15)"}},
            {"Oração", {"Oração", 3, SpellSchool::ENCHANTMENT, SaveType::NONE, 40, true, "+1 ataque/saves/dano aliados, -1 inimigos"}},
            {"Luz Cegante", {"Luz Cegante", 3, SpellSchool::EVOCATION, SaveType::REF, 150, true, "1d8 dano/2 níveis, cega alvos"}},
            {"Remover Maldição", {"Remover Maldição", 3, SpellSchool::ABJURATION, SaveType::WILL, 0, true, "Liberta de maldições"}},

            // Adicionais Druida
            {"Pele de Árvore", {"Pele de Árvore", 2, SpellSchool::TRANSMUTATION, SaveType::NONE, 0, false, "+2 a +5 armadura natural"}},
            {"Esfera Flamejante", {"Esfera Flamejante", 2, SpellSchool::EVOCATION, SaveType::REF, 150, true, "Bola de fogo móvel causa 2d6"}},
            {"Patas de Aranha", {"Patas de Aranha", 2, SpellSchool::TRANSMUTATION, SaveType::WILL, 0, true, "Alvo escala paredes"}},
            {"Invocação de Enxames", {"Invocação de Enxames", 2, SpellSchool::CONJURATION, SaveType::NONE, 30, false, "Convoca aranhas/morcegos/ratos"}},
            {"Convocar Relâmpagos", {"Convocar Relâmpagos", 3, SpellSchool::EVOCATION, SaveType::REF, 600, true, "Relâmpagos caem do céu (3d6)"}},
            {"Canino Mágico Maior", {"Canino Mágico Maior", 3, SpellSchool::TRANSMUTATION, SaveType::WILL, 0, true, "+1 ataque/dano por 3 níveis"}},
            {"Veneno", {"Veneno", 3, SpellSchool::NECROMANCY, SaveType::FORT, 0, true, "1d10 dano de CON"}},
            {"Crescer Espinhos", {"Crescer Espinhos", 3, SpellSchool::TRANSMUTATION, SaveType::REF, 600, true, "Terreno causa 1d4 dano e lentidão"}},

            // Nível 2 - Arcanas
            {"Raio Ardente", {"Raio Ardente", 2, SpellSchool::EVOCATION, SaveType::NONE, 30, true, "4d6 de dano de fogo"}},
            {"Imagens Espelhadas", {"Imagens Espelhadas", 2, SpellSchool::ILLUSION, SaveType::NONE, 0, false, "Cria cópias ilusórias"}},
            {"Teia", {"Teia", 2, SpellSchool::CONJURATION, SaveType::REF, 150, false, "Prende criaturas em teias pegajosas"}},
            {"Invisibilidade", {"Invisibilidade", 2, SpellSchool::ILLUSION, SaveType::WILL, 0, true, "Alvo fica invisível"}},
            {"Força de Touro", {"Força de Touro", 2, SpellSchool::TRANSMUTATION, SaveType::WILL, 0, true, "+4 de Força por 1 min/nível"}},

            // Nível 3 - Arcanas
            {"Bola de Fogo", {"Bola de Fogo", 3, SpellSchool::EVOCATION, SaveType::REF, 600, true, "1d6 de dano de fogo/nível"}},
            {"Velocidade", {"Velocidade", 3, SpellSchool::TRANSMUTATION, SaveType::FORT, 30, true, "Extra ataque, +1 ataque/AC/Ref"}},
            {"Voo", {"Voo", 3, SpellSchool::TRANSMUTATION, SaveType::WILL, 0, true, "Alvo pode voar"}},
            {"Relâmpago", {"Relâmpago", 3, SpellSchool::EVOCATION, SaveType::REF, 120, true, "1d6 de dano elétrico/nível"}},
            {"Dissipar Magia", {"Dissipar Magia", 3, SpellSchool::ABJURATION, SaveType::NONE, 150, false, "Cancela efeitos mágicos"}},

            // Nível 1 - Druida
            {"Emaranhar", {"Emaranhar", 1, SpellSchool::TRANSMUTATION, SaveType::REF, 600, false, "Plantas prendem criaturas"}},
            {"Produzir Fama", {"Produzir Fama", 1, SpellSchool::EVOCATION, SaveType::NONE, 0, true, "Fogo na mão, pode arremessar"}},
            {"Clava Mística", {"Clava Mística", 1, SpellSchool::TRANSMUTATION, SaveType::NONE, 0, false, "Bordão/Clava causa mais dano"}},
            {"Fogo Fátuo", {"Fogo Fátuo", 1, SpellSchool::EVOCATION, SaveType::NONE, 600, true, "Luz contorna alvos, cancela ocultação"}},
            {"Passos Longos", {"Passos Longos", 1, SpellSchool::TRANSMUTATION, SaveType::NONE, 0, false, "+10 pés de movimento"}}
        };
        if (spells.count(name)) return spells[name];
        return {"Nenhuma", 0, SpellSchool::ABJURATION, SaveType::NONE, 0, false, ""};
    }

    static std::vector<std::string> getPopularSpells(ClassType classType, int level) {
        // Retorna as 5 magias sugeridas por classe e nível
        if (classType == ClassType::WIZARD || classType == ClassType::SORCERER) {
            if (level == 0) return {"Raio de Gelo", "Mãos Mágicas", "Ler Magia", "Detectar Magia", "Luz"};
            if (level == 1) return {"Mísseis Mágicos", "Armadura Arcana", "Escudo", "Sono", "Mãos Flamejantes"};
            if (level == 2) return {"Raio Ardente", "Imagens Espelhadas", "Teia", "Invisibilidade", "Força de Touro"};
            if (level == 3) return {"Bola de Fogo", "Velocidade", "Voo", "Relâmpago", "Dissipar Magia"};
        }
        if (classType == ClassType::CLERIC) {
            if (level == 0) return {"Luz", "Detectar Magia", "Ler Magia", "Resistência", "Virtude"};
            if (level == 1) return {"Curar Ferimentos Leves", "Bênção", "Comando", "Escudo da Fé", "Causar Ferimentos Leves"};
            if (level == 2) return {"Curar Ferimentos Moderados", "Ajuda", "Imobilizar Pessoa", "Arma Espiritual", "Restauração Menor"};
            if (level == 3) return {"Curar Ferimentos Graves", "Dissipar Magia", "Oração", "Luz Cegante", "Remover Maldição"};
        }
        if (classType == ClassType::DRUID) {
            if (level == 0) return {"Luz", "Detectar Magia", "Ler Magia", "Resistência", "Orientação"};
            if (level == 1) return {"Curar Ferimentos Leves", "Emaranhar", "Produzir Fama", "Clava Mística", "Fogo Fátuo"};
            if (level == 2) return {"Pele de Árvore", "Esfera Flamejante", "Restauração Menor", "Patas de Aranha", "Invocação de Enxames"};
            if (level == 3) return {"Convocar Relâmpagos", "Curar Ferimentos Moderados", "Canino Mágico Maior", "Veneno", "Crescer Espinhos"};
        }
        return {};
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_SPELL_DATA_H
