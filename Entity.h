#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

using namespace std;

// Lớp cơ sở Entity đại diện cho mọi đối tượng có thể render trong game
class Entity {
protected:
    float x, y;                     // Vị trí (tọa độ) của entity
    SDL_Rect currentFrame;          // Khung hình hiện tại (vị trí và kích thước)
    SDL_Texture* tex = NULL;        // Texture chứa hình ảnh
    SDL_RendererFlip flipType = SDL_FLIP_NONE; // Kiểu lật hình (mặc định không lật)

public:
    // Constructor - Khởi tạo entity với vị trí và texture
    Entity(float p_x, float p_y, SDL_Texture* p_tex);

    // Lấy tọa độ X
    float getX();

    // Lấy tọa độ Y
    float getY();

    // Lấy khung hình hiện tại
    SDL_Rect getCurrentFrame();

    // Lấy texture
    SDL_Texture* getTex();

    // Lấy kiểu lật hình
    SDL_RendererFlip getFlipType();
};
