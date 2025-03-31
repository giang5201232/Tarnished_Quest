// Bullet.h
#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Entity.h"
#include "RenderWindow.h"
#include "LevelPart.h"

using namespace std;

// Các hằng số cấu hình đạn
const int DEFAULTBULLET_W = 128;  // Chiều rộng mặc định của đạn
const int DEFAULTBULLET_H = 128;  // Chiều cao mặc định của đạn
const float BULLETSPEED = 1;     // Tốc độ cơ bản của đạn

// Lớp Bullet kế thừa từ Entity, quản lý đạn bắn từ nhân vật
class Bullet : public Entity {
private:
    float xVel = 7;              // Vận tốc di chuyển theo trục X
    bool moving = false;         // Trạng thái đang di chuyển
    int bulletType;              // Loại đạn
    SDL_Rect collision;          // Hình chữ nhật va chạm

    // Animation đạn
    static const int BULLET_ANIMATION_FRAMES = 5;  // Tổng số frame animation
    SDL_Rect bulletClips[BULLET_ANIMATION_FRAMES]; // Mảng chứa các frame animation
    int bulletCounter = 0;       // Bộ đếm frame hiện tại
    const int delay = 4;         // Độ trễ giữa các frame

public:
    // Các loại đạn
    enum bulletType {
        NONE = 0,    // Không có đạn
        NORMAL = 1,   // Đạn thường
    };

    // Constructor
    Bullet(float p_x, float p_y, SDL_Texture* p_tex);

    // Cập nhật trạng thái đạn
    void update(vector<LevelPart>& LevelPartList);

    // Vẽ đạn lên màn hình
    void render(SDL_Rect& camera, SDL_Texture* p_tex);

    // ============ CÁC PHƯƠNG THỨC TRUY XUẤT ============

    // Lấy loại đạn
    int getType() const { return bulletType; }

    // Kiểm tra đạn có đang di chuyển không
    bool isMoving() const { return moving; }

    // Lấy vùng va chạm
    SDL_Rect getCollision() { return collision; }

    // ============ CÁC PHƯƠNG THỨC THIẾT LẬP ============

    // Thiết lập hướng bắn (trái/phải)
    void setFlipType(SDL_RendererFlip p_PlayerflipType);

    // Đặt loại đạn
    void setType(const int& p_type) { bulletType = p_type; }

    // Bật/tắt trạng thái di chuyển
    void setMove(bool p_move) { moving = p_move; }

    // Thiết lập kích thước và vị trí ban đầu khi bắn
    void setSize_Position(const int &p_width, const int &p_height,
                        const int &p_playerX, const int& p_playerY);
};
