#ifndef TATICAS_DO_DESTINO_CAMERA_H
#define TATICAS_DO_DESTINO_CAMERA_H

namespace rpg {

struct Camera {
    float x = 0.0f;
    float y = 0.0f;
    float zoom = 1.0f;

    void pan(float dx, float dy) {
        x += dx / zoom;
        y += dy / zoom;
    }

    void adjustZoom(float delta) {
        zoom += delta;
        if (zoom < 0.1f) zoom = 0.1f;
        if (zoom > 5.0f) zoom = 5.0f;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_CAMERA_H
