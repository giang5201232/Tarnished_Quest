#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <math.h>
#include "RenderWindow.h"
#include "Player.h"
#include "Entity.h"
#include "LevelPart.h"
class Player;
using namespace std;

// Lớp quái vật Skeleton kế thừa từ Entity
class Skeleton : public Entity {
private:
    const int SKELETON_WIDTH = 64;    // Chiều rộng sprite
    const int SKELETON_HEIGHT = 64;   // Chiều cao sprite
    const int SKELETON_VEL = 4;       // Tốc độ di chuyển

    // Số frame animation cho các trạng thái
    static const int WALKING_ANIMATION_FRAMES = 4;
    static const int IDLING_ANIMATION_FRAMES = 4;
    static const int FALLING_ANIMATION_FRAMES = 4;
    static const int ATTACKING_ANIMATION_FRAMES = 4;
    static const int BEINGHIT_ANIMATION_FRAMES = 4;

    // Các khung hình animation
    SDL_Rect walkingClips[WALKING_ANIMATION_FRAMES];  // Khung hình di chuyển
    SDL_Rect idlingClips[IDLING_ANIMATION_FRAMES];    // Khung hình đứng yên
    SDL_Rect fallingClips[FALLING_ANIMATION_FRAMES];  // Khung hình rơi
    SDL_Rect attackingClips[FALLING_ANIMATION_FRAMES]; // Khung hình tấn công
    SDL_Rect beingHitClips[BEINGHIT_ANIMATION_FRAMES]; // Khung hình bị đánh

    // Bộ đếm animation
    int idleCounter = 0;      // Đếm frame đứng yên
    int walkCounter = 0;      // Đếm frame di chuyển
    int fallingCounter = 0;   // Đếm frame rơi
    int attackingCounter = 0; // Đếm frame tấn công
    int beingHitCounter = 0;  // Đếm frame bị đánh

    // Trạng thái
    bool grounded = true;   // Đang đứng trên đất
    bool walking = false;   // Đang di chuyển
    bool idling = true;     // Đang đứng yên
    bool falling = false;   // Đang rơi
    bool attacking = false; // Đang tấn công
    bool beingHit = false;  // Đang bị đánh
    bool dead = false;      // Đã chết

    float xVel = 0, yVel = 0;  // Vận tốc
    int maxHealth = 3;         // Máu tối đa
    int groundSTT = 1;         // Block đang đứng
    int levelSTT = 1;          // Màn hiện tại
    float distanceToPlayer;    // Khoảng cách tới player
    SDL_Rect collision;       // Hình va chạm

public:
    Skeleton(float p_x, float p_y, SDL_Texture* p_tex); // Khởi tạo skeleton

    void update(Player& p_player, vector<LevelPart>& LevelPartList, Mix_Chunk* p_sfx[], SDL_Rect& camera); // Cập nhật trạng thái
    void gravity();          // Xử lý trọng lực
    void autoMovement(vector<LevelPart>& LevelPartList); // Di chuyển tự động
    void moveToPlayer(Player& p_player, vector<LevelPart>& LevelPartList); // Đuổi theo player
    bool isDead() { return dead; } // Kiểm tra đã chết
    bool isAttacking();      // Kiểm tra đang tấn công
    void getHit(Player& p_player, Mix_Chunk* p_sfx[], SDL_Rect& camera); // Xử lý khi bị đánh
    void knockBack();        // Hiệu ứng bị đẩy lùi
    void render(SDL_Rect& p_camera); // Vẽ skeleton

    SDL_Rect getCollision() const { return collision; } // Lấy vùng va chạm
    float getDistance() { return distanceToPlayer; }    // Lấy khoảng cách tới player
};
