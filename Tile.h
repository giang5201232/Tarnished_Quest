// Tile.h
#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "RenderWindow.h"
#include "Entity.h"
using namespace std;

// Lớp Tile kế thừa từ Entity
class Tile : public Entity {
private:
    int tileType;       // Loại tile (dùng để xác định hình ảnh và thuộc tính)
    SDL_Rect collision; // Hình chữ nhật va chạm của tile

public:
    // Constructor - Khởi tạo tile với vị trí, texture và loại tile
    Tile(float p_x, float p_y, SDL_Texture* p_tex, int p_type);

    // Getter - Lấy loại tile
    int getType();

    // Setter - Đặt lại vị trí X cho tile
    void setX(int p_x);

    // Setter - Đặt loại tile mới
    void setType(int p_type) { tileType = p_type; }

    // Getter - Lấy hình chữ nhật va chạm
    SDL_Rect getCollision();
};
