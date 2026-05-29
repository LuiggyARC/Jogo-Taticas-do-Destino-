#ifndef TATICAS_DO_DESTINO_SPRITE_RENDERER_H
#define TATICAS_DO_DESTINO_SPRITE_RENDERER_H

#include <GLES3/gl3.h>
#include <vector>
#include <memory>
#include "Shader.h"
#include "TextureAsset.h"
#include "Utility.h"

namespace rpg {

struct SpriteVertex {
    float x, y, z;
    float u, v;
};

class SpriteRenderer {
public:
    SpriteRenderer(std::shared_ptr<Shader> shader) : shader_(std::move(shader)) {
        glGenVertexArrays(1, &vao_);
        glGenBuffers(1, &vbo_);
        glGenBuffers(1, &ibo_);

        glBindVertexArray(vao_);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        // Pos
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)0);
        // UV
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)(sizeof(float) * 3));

        std::vector<uint16_t> indices = {0, 1, 2, 0, 2, 3};
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint16_t), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    ~SpriteRenderer() {
        glDeleteVertexArrays(1, &vao_);
        glDeleteBuffers(1, &vbo_);
        glDeleteBuffers(1, &ibo_);
    }

    void drawSprite(const TextureAsset& texture, float x, float y, float width, float height) {
        SpriteVertex vertices[4] = {
            {x + width, y + height, 0.0f, 1.0f, 0.0f}, // Top Right
            {x,         y + height, 0.0f, 0.0f, 0.0f}, // Top Left
            {x,         y,          0.0f, 0.0f, 1.0f}, // Bottom Left
            {x + width, y,          0.0f, 1.0f, 1.0f}  // Bottom Right
        };

        shader_->activate();
        glBindTexture(GL_TEXTURE_2D, texture.getTextureId());

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
    }

private:
    std::shared_ptr<Shader> shader_;
    GLuint vao_, vbo_, ibo_;
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_SPRITE_RENDERER_H
