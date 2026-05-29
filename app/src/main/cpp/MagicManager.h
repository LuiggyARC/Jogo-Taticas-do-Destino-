#ifndef TATICAS_DO_DESTINO_MAGIC_MANAGER_H
#define TATICAS_DO_DESTINO_MAGIC_MANAGER_H

#include "RpgSystem.h"
#include "Dice.h"
#include "SpellData.h"

namespace rpg {

class MagicManager {
public:
    static int getSpellDC(Character* caster, const Spell& spell) {
        int abilityMod = 0;

        // Determina atributo chave
        if (caster->levels.count(ClassType::WIZARD))
            abilityMod = caster->getModifierWithRace(AttributeType::INT);
        else if (caster->levels.count(ClassType::CLERIC) || caster->levels.count(ClassType::DRUID))
            abilityMod = caster->getModifierWithRace(AttributeType::WIS);
        else if (caster->levels.count(ClassType::SORCERER) || caster->levels.count(ClassType::BARD))
            abilityMod = caster->getModifierWithRace(AttributeType::CHA);

        return 10 + spell.level + abilityMod;
    }

    struct SpellResult {
        bool success;
        bool saved;
        int damageOrHealing;
        std::string effect;
    };

    static SpellResult castSpell(Character* caster, Character* target, const std::string& spellName) {
        Spell spell = SpellData::getSpell(spellName);
        SpellResult result = {true, false, 0, ""};

        // Verificação de Resistência à Magia (SR) - Simplificado
        if (spell.allowSR) {
            int srRoll = Dice::roll(20) + caster->getTotalLevel();
            int targetSR = 10; // Base SR para testes, pode ser expandido
            if (srRoll < targetSR) {
                result.success = false;
                result.effect = "Resistido por SR";
                return result;
            }
        }

        // Teste de Resistência (Saving Throw)
        if (spell.saveType != SaveType::NONE) {
            int dc = getSpellDC(caster, spell);
            int saveRoll = Dice::roll(20);
            int saveBonus = 0;

            if (spell.saveType == SaveType::FORT) saveBonus = target->getTotalFortSave();
            else if (spell.saveType == SaveType::REF) saveBonus = target->getTotalRefSave();
            else if (spell.saveType == SaveType::WILL) saveBonus = target->getTotalWillSave();

            if (saveRoll + saveBonus >= dc) {
                result.saved = true;
            }
        }

        // Efeitos Específicos (Exemplos)
        int sorcererBonus = 0;
        if (caster->levels.count(ClassType::SORCERER)) {
            sorcererBonus = caster->levels.at(ClassType::SORCERER);
        }

        if (spellName == "Mísseis Mágicos") {
            int missiles = (caster->getTotalLevel() + 1) / 2;
            if (missiles > 5) missiles = 5;
            for(int i=0; i<missiles; ++i) result.damageOrHealing += Dice::roll(4) + 1;
            result.damageOrHealing += sorcererBonus;
        }
        else if (spellName == "Bola de Fogo") {
            int dice = caster->getTotalLevel();
            if (dice > 10) dice = 10;
            result.damageOrHealing = Dice::rollMultiple(dice, 6) + sorcererBonus;
            if (result.saved) result.damageOrHealing /= 2;
        }
        else if (spellName == "Curar Ferimentos Leves") {
            int bonus = std::min(5, caster->getTotalLevel());
            result.damageOrHealing = Dice::roll(8) + bonus;
        }

        return result;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_MAGIC_MANAGER_H
