// Player.h
#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>

#include "RenderWindow.h"
#include "Entity.h"
#include "Bullet.h"
#include "Skeleton.h"
#include "LevelPart.h"
class Skeleton;
using namespace std;

// Lớp Player đại diện cho nhân vật người chơi
class Player : public Entity {
private:
    // Các hằng số cấu hình player
    const int PLAYER_WIDTH = 64;    // Chiều rộng player
    const int PLAYER_HEIGHT = 64;   // Chiều cao player
    const int PLAYER_VEL = 6;       // Tốc độ di chuyển

    // Biến liên quan đến cơ chế tấn công
    int attackCounter = 0;          // Đếm thời gian tấn công
    bool attacking = false;         // Trạng thái đang tấn công
    const int attackDelay = 10;     // Thời gian delay giữa các lần tấn công

    // Các hằng số animation
    static const int ATTACKING_ANIMATION_FRAMES = 1;  // Số frame animation tấn công
    static const int WALKING_ANIMATION_FRAMES = 8;    // Số frame animation đi bộ
    static const int IDLING_ANIMATION_FRAMES = 4;     // Số frame animation đứng yên
    static const int JUMPING_ANIMATION_FRAMES = 4;    // Số frame animation nhảy
    static const int FALLING_ANIMATION_FRAMES = 4;    // Số frame animation rơi
    static const int DEATH_ANIMATION_FRAMES = 4;      // Số frame animation chết

    // Các mảng lưu clip animation
    SDL_Rect attackingClips[ATTACKING_ANIMATION_FRAMES];
    SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];
    SDL_Rect idlingClips[IDLING_ANIMATION_FRAMES];
    SDL_Rect jumpingClips[JUMPING_ANIMATION_FRAMES];
    SDL_Rect fallingClips[FALLING_ANIMATION_FRAMES];
    SDL_Rect deathClips[DEATH_ANIMATION_FRAMES];

    // Các biến đếm animation
    int idleCounter = 0, walkCounter = 0, jumpCounter = 0,
        fallingCounter = 0, deathCounter = 0;

    // Các trạng thái của player
    bool grounded = false;   // Đang đứng trên mặt đất
    bool running = false;    // Đang chạy
    bool idling = true;      // Đang đứng yên
    bool jumping = false;    // Đang nhảy
    bool falling = true;     // Đang rơi
    bool dead = false;       // Đã chết
    bool beingHit = false;   // Đang bị tấn công

    // Vận tốc
    float xVel = 0, yVel = 0;

    // Thông tin vị trí
    int groundSTT = 1;       // Số thứ tự block đang đứng
    int levelSTT = 1;        // Số thứ tự map hiện tại

    SDL_Rect collision;      // Hình chữ nhật va chạm
    vector<Bullet*> bulletList; // Danh sách đạn
    Mix_Chunk* playerSFX[4]; // Mảng âm thanh

public:
    // Constructor
    Player(float p_x, float p_y, SDL_Texture* p_tex);

    // Enum các hiệu ứng âm thanh
    enum SFX {
        hitSFX = 0,    // Âm thanh khi bị đánh
        jumpSFX = 1,    // Âm thanh khi nhảy
        landSFX = 2,    // Âm thanh khi tiếp đất
        shootSFX = 3,   // Âm thanh khi bắn
    };

    // Các phương thức xử lý
    void handleInput(SDL_Event &events, Mix_Chunk* p_sfx[]); // Xử lý input
    void update(vector<LevelPart>& LevelPartList, vector<Skeleton*> &skeletonList,
              Mix_Chunk* p_sfx[], SDL_Rect& camera); // Cập nhật trạng thái
    void jump();       // Xử lý nhảy
    void gravity();    // Xử lý trọng lực
    void getHit(vector<Skeleton*> &skeletonList, Mix_Chunk* p_sfx[],
              SDL_Rect& camera); // Xử lý khi bị đánh
    bool isDead() { return dead; } // Kiểm tra trạng thái chết
    void knockBack();  // Hiệu ứng bị đẩy lùi
    void handleCamera(SDL_Rect& camera, float& camVel); // Xử lý camera
    void render(SDL_Rect& p_camera); // Render player

    // Reset trạng thái player
    void resetPlayer() {
        x = 64 * 3;
        y = LEVEL_HEIGHT - TILE_HEIGHT * 5;
        xVel = 0;
        yVel = 0;
        dead = false;
        flipType = SDL_FLIP_NONE;
    }

    // Setter danh sách đạn
    void setBulletList(vector<Bullet*> bulletList) { this->bulletList = bulletList; }

    // Các getter
    vector<Bullet*> getBulletList() const { return bulletList; }
    SDL_Rect getCollision() const { return collision; }
};
