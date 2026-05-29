#ifndef TATICAS_DO_DESTINO_REST_MANAGER_H
#define TATICAS_DO_DESTINO_REST_MANAGER_H

#include "RpgSystem.h"
#include <chrono>

namespace rpg {

class RestManager {
public:
    static long long getCurrentTimeSeconds() {
        return std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
    }

    static bool isPlayerResting(Character* character) {
        return getCurrentTimeSeconds() < character->lastRestFinishTime;
    }

    static long long getRemainingRestTime(Character* character) {
        long long remaining = character->lastRestFinishTime - getCurrentTimeSeconds();
        return remaining > 0 ? remaining : 0;
    }

    static void checkDailyReset(Character* character) {
        long long now = getCurrentTimeSeconds();
        // 86400 segundos = 24 horas
        // Simplificado: se mudou o dia (GMT)
        time_t currentTime = (time_t)now;
        struct tm * timeinfo = gmtime(&currentTime);
        int today = timeinfo->tm_yday;

        time_t lastTime = (time_t)character->lastResetDate;
        struct tm * lastInfo = gmtime(&lastTime);
        int lastDay = lastInfo->tm_yday;

        if (today != lastDay) {
            character->restsToday = 0;
            character->lastResetDate = now;
        }
    }

    static bool startRest(Character* character) {
        if (isPlayerResting(character)) return false;

        checkDailyReset(character);

        int durationMinutes = character->getNextRestDurationMinutes();
        character->lastRestFinishTime = getCurrentTimeSeconds() + (durationMinutes * 60);
        character->restsToday++;

        // A recuperação acontece no início ou no fim?
        // Geralmente no fim, mas para facilitar a lógica de "impossibilitado",
        // vamos aplicar ao terminar.
        return true;
    }

    static void update(Character* character) {
        if (!isPlayerResting(character) && character->currentHp < character->getMaxHp()) {
            // Se o tempo de descanso acabou e ele ainda não recuperou, recupera agora
            if (character->lastRestFinishTime > 0) {
                 character->recoverFull();
                 character->lastRestFinishTime = 0; // Reset para evitar múltiplas recuperações
            }
        }
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_REST_MANAGER_H
