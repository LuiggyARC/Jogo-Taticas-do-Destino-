#ifndef TATICAS_DO_DESTINO_AUDIO_MANAGER_H
#define TATICAS_DO_DESTINO_AUDIO_MANAGER_H

#include <string>
#include <map>

namespace rpg {

enum class SoundEvent {
    ATTACK_MELEE,
    ATTACK_RANGED,
    SPELL_CAST,
    TAKE_DAMAGE,
    CHARACTER_DEATH,
    LEVEL_UP,
    BUTTON_CLICK,
    RAGE_START,
    GOLD_GAIN
};

class AudioManager {
public:
    // Esta classe define o mapeamento. A execução real dependeria de uma
    // biblioteca como Oboe ou OpenSL ES integrada ao Android.
    static std::string getSoundResource(SoundEvent event) {
        static std::map<SoundEvent, std::string> soundMap = {
            {SoundEvent::ATTACK_MELEE, "sword_clash.wav"},
            {SoundEvent::ATTACK_RANGED, "arrow_shot.wav"},
            {SoundEvent::SPELL_CAST, "magic_flare.wav"},
            {SoundEvent::TAKE_DAMAGE, "hit_grunt.wav"},
            {SoundEvent::CHARACTER_DEATH, "death_cry.wav"},
            {SoundEvent::LEVEL_UP, "fanfare.wav"},
            {SoundEvent::BUTTON_CLICK, "ui_click.wav"},
            {SoundEvent::RAGE_START, "barbarian_roar.wav"},
            {SoundEvent::GOLD_GAIN, "coins_jingle.wav"}
        };
        return soundMap[event];
    }

    // Função de interface para ser chamada pela lógica de jogo
    static void playSound(SoundEvent event) {
        std::string res = getSoundResource(event);
        // Aqui chamaria a JNI ou Oboe para tocar o som
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_AUDIO_MANAGER_H
