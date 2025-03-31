#include "Bullet.h"

// Constructor - Khởi tạo viên đạn với vị trí và texture
Bullet::Bullet(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
    // Thiết lập kích thước vùng va chạm
    collision.w = DEFAULTBULLET_W;
    collision.h = DEFAULTBULLET_H;

    // Căn giữa vùng va chạm so với vị trí render
    collision.x = x + (DEFAULTBULLET_W - collision.w)/2;
    collision.y = y + (DEFAULTBULLET_H - collision.h)/2;

    // Khởi tạo các frame animation cho đạn
    for (int i = 0; i < BULLET_ANIMATION_FRAMES; i++) {
        bulletClips[i].x = i * DEFAULTBULLET_W;  // Vị trí x theo từng frame
        bulletClips[i].y = 0;                    // Vị trí y cố định
        bulletClips[i].w = DEFAULTBULLET_W;      // Chiều rộng frame
        bulletClips[i].h = DEFAULTBULLET_H;      // Chiều cao frame
    }
}

// Cập nhật trạng thái đạn mỗi frame
void Bullet::update(vector<LevelPart>& LevelPartList) {
    // Chỉ xử lý khi đạn còn hoạt động (trong 3 frame đầu)
    if (bulletCounter/delay < 3) {
        xVel += BULLETSPEED;  // Tăng tốc độ đạn

        // Di chuyển đạn theo hướng bắn
        if (getFlipType() == SDL_FLIP_HORIZONTAL) x -= xVel;  // Bắn sang trái
        else if (getFlipType() == SDL_FLIP_NONE) x += xVel;   // Bắn sang phải

        // Cập nhật vị trí va chạm
        collision.x = x + (DEFAULTBULLET_W - collision.w)/2;
        collision.y = y + (DEFAULTBULLET_H - collision.h)/2;

        // Xử lý khi đạn chạm biên trái màn hình
        if (getX() < 0) {
            x = 0;
            collision.x = x + (DEFAULTBULLET_W - collision.w)/2;
        }

        // Xử lý va chạm với tường
        if (commonFunc::touchesWall(collision, LevelPartList)) {
            // Đẩy lùi đạn khi chạm tường
            if (getFlipType() == SDL_FLIP_HORIZONTAL) x += xVel;
            else if (getFlipType() == SDL_FLIP_NONE) x -= xVel;
            collision.x = x + (DEFAULTBULLET_W - collision.w)/2;
        }
    }

    // Tự hủy đạn khi hết animation
    if (bulletCounter/delay == BULLET_ANIMATION_FRAMES) setMove(false);
}

// Thiết lập hướng bắn (trái/phải)
void Bullet::setFlipType(SDL_RendererFlip p_PlayerflipType) {
    flipType = p_PlayerflipType;
}

// Thiết lập vị trí và kích thước đạn khi bắn
void Bullet::setSize_Position(const int& p_width, const int& p_height,
                            const int& p_playerX, const int& p_playerY) {
    // Đặt vị trí x dựa trên hướng bắn
    if (getFlipType() == SDL_FLIP_HORIZONTAL) {
        x = p_playerX - DEFAULTBULLET_W/2;  // Bắn sang trái
    } else {
        x = p_playerX + p_width - DEFAULTBULLET_W/2;  // Bắn sang phải
    }

    // Đặt vị trí y (hơi cao hơn trung tâm nhân vật)
    y = p_playerY + p_height / 8;

    // Cập nhật vùng va chạm (70% kích thước thật để mềm mại)
    collision.x = x + (DEFAULTBULLET_W - collision.w)/2;
    collision.y = y + (DEFAULTBULLET_H - collision.h)/2;
    collision.w = DEFAULTBULLET_W * 0.7f;
    collision.h = DEFAULTBULLET_H * 0.7f;
}

// Render đạn lên màn hình
void Bullet::render(SDL_Rect &camera, SDL_Texture* p_tex) {
    // Render animation đạn với frame hiện tại
    commonFunc::renderAnimation(p_tex, getX(), getY(),
                              bulletClips[bulletCounter/ delay],
                              camera, 0, NULL, getFlipType());
    bulletCounter++;  // Tăng bộ đếm frame
}
