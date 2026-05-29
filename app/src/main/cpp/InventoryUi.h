#ifndef TATICAS_DO_DESTINO_INVENTORY_UI_H
#define TATICAS_DO_DESTINO_INVENTORY_UI_H

#include "SpriteRenderer.h"
#include "RpgSystem.h"
#include <vector>

namespace rpg {

struct UiRect {
    float x, y, w, h;
};

class InventoryUi {
public:
    InventoryUi(std::shared_ptr<SpriteRenderer> renderer) : renderer_(std::move(renderer)) {}

    void renderInventory(const Character& player, const std::map<std::string, std::shared_ptr<TextureAsset>>& icons, const TextureAsset& bgTex) {
        // Fundo do Painel de Inventário (Centralizado)
        renderer_->drawSprite(bgTex, -0.8f, -0.8f, 1.6f, 1.6f);

        // Renderiza Slots de Equipamento
        drawEquippedSlot("Arma", player.equippedWeapon.name, -0.6f, 0.4f, icons);
        drawEquippedSlot("Armadura", player.equippedArmor.name, -0.6f, 0.1f, icons);

        // Renderiza Acessórios
        float accY = -0.2f;
        for (auto const& [slot, acc] : player.accessories) {
            drawEquippedSlot("Acessório", acc.name, -0.6f, accY, icons);
            accY -= 0.3f;
        }

        // Informações de Gold e XP (Simbolizado por ícones)
        if (icons.count("gold_icon")) {
            renderer_->drawSprite(*icons.at("gold_icon"), 0.4f, 0.6f, 0.1f, 0.1f);
        }
    }

private:
    void drawEquippedSlot(const std::string& label, const std::string& itemName, float x, float y,
                         const std::map<std::string, std::shared_ptr<TextureAsset>>& icons) {
        float slotSize = 0.25f;

        // Se temos um ícone para o item, desenha-o
        if (icons.count(itemName)) {
            renderer_->drawSprite(*icons.at(itemName), x, y, slotSize, slotSize);
        } else {
            // Desenha um slot vazio ou ícone padrão
            if (icons.count("empty_slot")) {
                renderer_->drawSprite(*icons.at("empty_slot"), x, y, slotSize, slotSize);
            }
        }
    }

    std::shared_ptr<SpriteRenderer> renderer_;
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_INVENTORY_UI_H
