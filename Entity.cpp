#include "Entity.h"

// Constructor implementation
Entity::Entity(float p_x, float p_y, SDL_Texture* p_tex)
    :x(p_x), y(p_y), tex(p_tex)  // Khởi tạo các giá trị
{
    currentFrame.x = 0;          // Mặc định vị trí x trong texture
    currentFrame.y = 0;          // Mặc định vị trí y trong texture
    // Lấy kích thước của texture
    SDL_QueryTexture(p_tex, NULL, NULL, &currentFrame.w, &currentFrame.h);
}

// Trả về tọa độ X
float Entity::getX() {
    return x;
}

// Trả về tọa độ Y
float Entity::getY() {
    return y;
}

// Trả về texture
SDL_Texture* Entity::getTex() {
    return tex;
}

// Trả về khung hình hiện tại
SDL_Rect Entity::getCurrentFrame() {
    return currentFrame;
}

// Trả về kiểu lật hình
SDL_RendererFlip Entity::getFlipType() {
    return flipType;
}
