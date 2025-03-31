/**
 * Description: Header file chứa khai báo các hàm và hằng số liên quan đến render và quản lý cửa sổ game
 */

#pragma once // Đảm bảo file chỉ được include một lần

// Các thư viện cần thiết
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>

// Các file header tự viết
#include "Entity.h"
#include "LevelPart.h"
#include "Tile.h"
class LevelPart;
class Tile;

using namespace std;

// Các hằng số vật lý
const float GRAVITY = 0.3;       // Giá trị trọng lực
const float MAX_GRAVITY = 15;    // Giá trị trọng lực tối đa

// Kích thước màn hình
const int SCREEN_WIDTH = 1280;    // Chiều rộng màn hình
const int SCREEN_HEIGHT = 720;    // Chiều cao màn hình

// Kích thước level
const int LEVEL_WIDTH = 1344;     // Chiều rộng level
const int LEVEL_HEIGHT = 1024;    // Chiều cao level

// Các hằng số về tile
const int TILE_WIDTH = 64;        // Chiều rộng tile
const int TILE_HEIGHT = 64;       // Chiều cao tile
const int TOTAL_LEVEL_PART = 3;   // Tổng số phần level
const int TOTAL_MAP = 15;         // Tổng số map
const int TOTAL_TILES = 336;      // Tổng số tile
const int TOTAL_TILE_SPRITES = 187; // Tổng số sprite tile

// Các biến toàn cục
static TTF_Font* font = NULL;     // Font chữ
static SDL_Window* window = NULL;  // Cửa sổ game
static SDL_Renderer* renderer = NULL; // Renderer
static SDL_Surface* surface = NULL;   // Surface

/**
 * Cấu trúc lưu đường dẫn và vị trí skeleton
 */
struct path_pos {
    const char* path;            // Đường dẫn file
    vector<float> skeletonPos;   // Vị trí skeleton

    // Constructor
    path_pos(vector<float> p_skeletonPos, const char* p_path) {
        path = p_path;
        skeletonPos = p_skeletonPos;
    }
};

/**
 * Namespace chứa các hàm tiện ích chung
 */
namespace commonFunc {
    // Window
    void renderWindow(const char* p_title, int p_width, int p_height); // Tạo cửa sổ

    // Texture
    SDL_Texture* loadTexture(const char* p_filePath); // Tải texture từ file
    void renderTexture(Entity& entity, SDL_Rect *rec = NULL, SDL_Rect *camera = NULL,
                      double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE); // Render entity
    void renderTexture(SDL_Texture* p_tex, float p_x, float p_y, float p_w = 0, float p_h = 0,
                      SDL_Rect* rec = NULL, SDL_Rect* camera = NULL, double angle = 0.0,
                      SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE); // Render texture
    void renderTile(Entity& entity, SDL_Rect& rec, SDL_Rect& camera); // Render tile
    void renderPlayer(Entity& entity, SDL_Rect& camera); // Render player
    void renderAnimation(SDL_Texture* p_tex, float p_x, float p_y, SDL_Rect& p_clip,
                        SDL_Rect& p_camera, double p_angle = 0.0, SDL_Point* p_center = NULL,
                        SDL_RendererFlip p_flip = SDL_FLIP_NONE); // Render animation

    // Collision
    bool checkCollision(SDL_Rect a, SDL_Rect b); // Kiểm tra va chạm giữa 2 hình chữ nhật
    bool touchesWall(SDL_Rect& box, vector<LevelPart>& LevelPartList); // Kiểm tra va chạm với tường
    bool touchesWall(SDL_Rect& box, vector<LevelPart>& LevelPartList, bool& grounded,
                    int& groundSTT, int& levelSTT); // Kiểm tra va chạm với tường (nâng cao)

    // Font
    bool loadFont(const char* filePath); // Tải font
    SDL_Texture* createText(string p_text, SDL_Color p_textColor); // Tạo texture từ text

    // Renderer
    void Color(); // Tô màu cửa sổ
    void clearRenderer(); // Xóa renderer
    void renderPresent(); // Hiển thị renderer
    void cleanUp(); // Dọn dẹp tài nguyên
}
