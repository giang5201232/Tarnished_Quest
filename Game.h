#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <sstream>
#include <fstream>
#include <ctime>
#include <cstdlib>

// Các file header tự viết
#include "RenderWindow.h"
#include "Entity.h"
#include "Player.h"
#include "Skeleton.h"
#include "Timer.h"
#include "Tile.h"
#include "Menu.h"
#include "LevelPart.h"
using namespace std;

// Lớp chính quản lý toàn bộ game
class Game {
private:
    // Các texture trong game
    SDL_Texture* knightTex = NULL;    // Texture nhân vật chính
    SDL_Texture* skeletonTex = NULL;  // Texture quái vật skeleton
    SDL_Texture* tileTex = NULL;      // Texture các tile map
    SDL_Texture* bulletTex = NULL;    // Texture đạn
    SDL_Texture* bgTex = NULL;        // Texture background
    SDL_Texture* buttonTex = NULL;    // Texture nút menu

    // Âm thanh
    Mix_Music* bgMusic = NULL;        // Nhạc nền
    Mix_Chunk* playerSFX[4] = { NULL };  // Hiệu ứng âm thanh cho player
    Mix_Chunk* skeletonSFX[2] = { NULL }; // Hiệu ứng âm thanh cho skeleton

    // Thời gian và điểm số
    LTimer fpsTimer;                  // Timer đếm FPS
    stringstream timeText;            // Hiển thị thời gian
    stringstream scoreText;           // Hiển thị điểm
    stringstream highscoreText;       // Hiển thị điểm cao nhất
    int score = 0;                    // Điểm hiện tại
    int highscore = 0;                // Điểm cao nhất
    int countedFrames = 0;            // Đếm số frame

    // Tile và camera
    SDL_Rect gTileClips[TOTAL_TILE_SPRITES]; // Các loại tile
    SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // Camera
    float camVel = 1.5;               // Tốc độ camera
    int exCounter = 0;                // Counter hiệu ứng

    // Trạng thái game
    bool gameRunning = true;          // Game đang chạy hay không

    // Các danh sách đối tượng trong game
    vector<path_pos> mapList;         // Danh sách vị trí map
    vector<LevelPart> LevelPartList;  // Danh sách các phần map
    vector<Skeleton*> skeletonList;   // Danh sách quái skeleton
    vector<Player> playerList;        // Danh sách player
    vector<Menu> menuList;            // Danh sách menu

public:
    Game() {
        setSDL_Rect();  // Khởi tạo các SDL_Rect
    }

    // Các phương thức khởi tạo
    bool init();        // Khởi tạo SDL
    bool loadMedia();   // Tải các tài nguyên
    void FPSCounter();  // Đếm và hiển thị FPS

    // Các phương thức hiển thị
    void renderScore(); // Hiển thị điểm số
    void render_update_LevelPart(); // Render và update map
    void render_update_bullet();    // Render và update đạn
    void render_update_player();    // Render và update player
    void render_update_skeleton();  // Render và update skeleton
    void render_update_Game();      // Render và update toàn bộ game
    void render_mainMenu();         // Render menu chính

    // Các phương thức tạo đối tượng
    bool createMap();       // Tạo map
    bool createLevel();     // Tạo level
    bool createSkeleton();  // Tạo skeleton
    bool createPlayer();    // Tạo player
    bool createMenu();      // Tạo menu

    // Các phương thức xử lý âm thanh
    void start_ExplosionSFX(); // Hiệu ứng nổ
    void playMusic();          // Phát nhạc
    void pauseMusicTime();     // Tạm dừng nhạc

    // Các phương thức xử lý game
    void update_hight_score(); // Cập nhật điểm cao
    void resetGame();          // Reset game
    void handleGameInput(SDL_Event& event); // Xử lý input

    // Getter/Setter
    bool isRunning() { return gameRunning; } // Kiểm tra game đang chạy
    vector<Menu> getMenuList() { return menuList; } // Lấy danh sách menu

    // Các phương thức hỗ trợ
    void setSDL_Rect(); // Thiết lập các SDL_Rect

    // Giải phóng tài nguyên
    void destroy() {
        SDL_DestroyTexture(knightTex);
        SDL_DestroyTexture(skeletonTex);
        SDL_DestroyTexture(tileTex);
        SDL_DestroyTexture(bulletTex);
        SDL_DestroyTexture(bgTex);
        SDL_DestroyTexture(buttonTex);
        SDL_DestroyTexture(bgTex);
    }
};
