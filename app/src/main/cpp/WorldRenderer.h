#ifndef TATICAS_DO_DESTINO_WORLD_RENDERER_H
#define TATICAS_DO_DESTINO_WORLD_RENDERER_H

#include "MapManager.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include <map>

namespace rpg {

class WorldRenderer {
public:
    WorldRenderer(std::shared_ptr<SpriteRenderer> spriteRenderer)
        : spriteRenderer_(std::move(spriteRenderer)) {}

    // Atualiza as posições visuais para animação suave
    void updateAnimations(Character& player, std::vector<EnemyInstance>& enemies, float deltaTime) {
        float speed = 5.0f * deltaTime; // Velocidade de interpolação

        player.visualX += (player.x - player.visualX) * speed;
        player.visualY += (player.y - player.visualY) * speed;

        for (auto& e : enemies) {
            e.visualX += (e.x - e.visualX) * speed;
            e.visualY += (e.y - e.visualY) * speed;
        }
    }

    void renderMap(const MapConfig& map, const std::map<TileType, std::shared_ptr<TextureAsset>>& textures, const Camera& cam, const GameState& state) {
        float tileSize = 1.0f;

        for (int y = 0; y < map.height; ++y) {
            for (int x = 0; x < map.width; ++x) {
                TileType type = map.getTile(x, y);
                if (textures.count(type)) {
                    // Aplica transformações da câmera
                    float renderX = (x * tileSize - cam.x) * cam.zoom;
                    float renderY = (y * tileSize - cam.y) * cam.zoom;
                    float renderSize = tileSize * cam.zoom;

                    spriteRenderer_->drawSprite(*textures.at(type), renderX, renderY, renderSize, renderSize);

                    // Desenha cursor de seleção
                    if (state.hasSelection && state.selectedX == x && state.selectedY == y) {
                        // cursor render logic
                    }
                }
            }
        }
    }

    void renderRange(const Character& player, const Camera& cam, const TextureAsset& highlightTex) {
        float tileSize = 1.0f;
        int moveRange = player.speed / 5;

        for (int dy = -moveRange; dy <= moveRange; ++dy) {
            for (int dx = -moveRange; dx <= moveRange; ++dx) {
                int tx = player.x + dx;
                int ty = player.y + dy;

                if (Grid::calculateDistance({player.x, player.y}, {tx, ty}) <= moveRange) {
                    float renderX = (tx * tileSize - cam.x) * cam.zoom;
                    float renderY = (ty * tileSize - cam.y) * cam.zoom;
                    float renderSize = tileSize * cam.zoom;
                    spriteRenderer_->drawSprite(highlightTex, renderX, renderY, renderSize, renderSize);
                }
            }
        }
    }

    void renderCharacters(const std::vector<EnemyInstance>& enemies, Character& player,
                        std::shared_ptr<TextureAsset> playerTex,
                        std::shared_ptr<TextureAsset> enemyTex,
                        std::shared_ptr<TextureAsset> whitePixelTex, // Para barras de HP
                        const Camera& cam) {
        float size = 0.8f;
        float offset = 0.1f;

        // Desenha Jogador
        drawCharacterWithHP(player.visualX + offset, player.visualY + offset, size,
                           player.currentHp, player.getMaxHp(), *playerTex, *whitePixelTex, cam);

        // Desenha Inimigos
        for (const auto& e : enemies) {
            if (e.currentHp > 0) {
                drawCharacterWithHP(e.visualX + offset, e.visualY + offset, size,
                                   e.currentHp, e.stats.hp, *enemyTex, *whitePixelTex, cam);
            }
        }
    }

private:
    void drawCharacterWithHP(float x, float y, float size, int hp, int maxHp,
                            const TextureAsset& sprite, const TextureAsset& whitePixel, const Camera& cam) {

        float renderX = (x - cam.x) * cam.zoom;
        float renderY = (y - cam.y) * cam.zoom;
        float renderSize = size * cam.zoom;

        // Sprite do Personagem
        spriteRenderer_->drawSprite(sprite, renderX, renderY, renderSize, renderSize);

        // Barra de Vida (Background Vermelho)
        float hpBarWidth = renderSize;
        float hpBarHeight = 0.1f * cam.zoom;
        float hpBarY = renderY + renderSize + (0.05f * cam.zoom);

        // Fundo (Dano/Vazio)
        spriteRenderer_->drawSprite(whitePixel, renderX, hpBarY, hpBarWidth, hpBarHeight); // Aqui você poderia mudar a cor via shader se suportado

        // Vida Atual (Verde)
        float currentHpWidth = hpBarWidth * (static_cast<float>(hp) / maxHp);
        if (currentHpWidth < 0) currentHpWidth = 0;
        spriteRenderer_->drawSprite(whitePixel, renderX, hpBarY, currentHpWidth, hpBarHeight);
    }

    std::shared_ptr<SpriteRenderer> spriteRenderer_;
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_WORLD_RENDERER_H
