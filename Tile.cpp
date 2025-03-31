#include "Tile.h"

// Constructor implementation
Tile::Tile(float p_x, float p_y, SDL_Texture* p_tex, int p_type)
    : Entity(p_x, p_y, p_tex)  // Gọi constructor của lớp cha Entity
{
    // Thiết lập hình chữ nhật va chạm
    collision.x = p_x;
    collision.y = p_y;
    collision.w = TILE_WIDTH;   // Sử dụng kích thước tile mặc định
    collision.h = TILE_HEIGHT;

    // Gán loại tile
    tileType = p_type;
}

// Trả về loại tile
int Tile::getType() {
    return tileType;
}

// Trả về hình chữ nhật va chạm
SDL_Rect Tile::getCollision() {
    return collision;
}

// Cập nhật vị trí X mới cho tile
void Tile::setX(int p_x) {
    x = p_x;                   // Cập nhật vị trí X (kế thừa từ Entity)
    collision.x = getX();      // Đồng bộ vị trí va chạm
}
