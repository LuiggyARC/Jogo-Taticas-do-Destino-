#ifndef TATICAS_DO_DESTINO_GRID_H
#define TATICAS_DO_DESTINO_GRID_H

#include <cmath>
#include <algorithm>
#include "MapManager.h"

namespace rpg {

struct Point {
    int x, y;
};

class Grid {
public:
    static bool isWithinBounds(Point p, int width, int height) {
        return p.x >= 0 && p.x < width && p.y >= 0 && p.y < height;
    }

    // Regra 3.5: A primeira diagonal custa 5 pés (1 quadrado), a segunda 10 pés (2 quadrados).
    // Simplificado para grid: dist = max(dx, dy) + min(dx, dy) / 2
    static int calculateDistance(Point p1, Point p2) {
        int dx = std::abs(p1.x - p2.x);
        int dy = std::abs(p1.y - p2.y);

        int min_d = std::min(dx, dy);
        int max_d = std::max(dx, dy);

        // Em D&D 3.5, cada 2 diagonais contam como 3 quadrados (5ft + 10ft)
        // A cada 2 movimentos diagonais, adicionamos 1 extra ao custo.
        return (max_d - min_d) + (min_d * 3 / 2);
    }

    static bool canMoveTo(Point p1, Point p2, int speedInFeet) {
        int gridSpeed = speedInFeet / 5;
        return calculateDistance(p1, p2) <= gridSpeed;
    }

    // Algoritmo de Bresenham simplificado para verificar Linha de Visão (LoS)
    static bool hasLineOfSight(Point p1, Point p2, const MapConfig& map) {
        int x1 = p1.x;
        int y1 = p1.y;
        int x2 = p2.x;
        int y2 = p2.y;

        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);
        int sx = (x1 < x2) ? 1 : -1;
        int sy = (y1 < y2) ? 1 : -1;
        int err = dx - dy;

        while (true) {
            if (x1 == x2 && y1 == y2) break;

            // Verifica se o tile atual bloqueia visão (exceto o ponto inicial e final)
            if ((x1 != p1.x || y1 != p1.y) && (x1 != p2.x || y1 != p2.y)) {
                if (map.getTile(x1, y1) == TileType::OBSTACLE) return false;
            }

            int e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }
            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }
        return true;
    }
};

} // namespace rpg

#endif // TATICAS_DO_DESTINO_GRID_H
