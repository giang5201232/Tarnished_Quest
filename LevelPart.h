#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vector>
#include <fstream>

#include "RenderWindow.h"
#include "Tile.h"
class Tile;
using namespace std;

class LevelPart {
private:
    float x, y;  // Tọa độ của level part
    vector<Tile*> tilesList;  // Danh sách các tile trong level
    vector<float> skeletonPos;  // Vị trí các skeleton trong level

public:
    LevelPart(float p_x, float p_y, const char* p_path, SDL_Texture* p_tileTex);  // Khởi tạo level part
    void render(SDL_Rect p_TileClips[], SDL_Rect& p_camera);  // Vẽ level part lên màn hình

    void setLevelX(LevelPart& p_level);  // Đặt vị trí X từ level khác
    void setLevelX(float p_x);  // Đặt trực tiếp vị trí X
    void setTilesType(const char* p_path);  // Thiết lập loại tile từ file
    void setSkeletonPos(vector<float>& p_skeletonPos) { skeletonPos = p_skeletonPos; }  // Đặt vị trí skeleton

    vector<Tile*> getTilesList() const { return tilesList; }  // Lấy danh sách tile
    vector<float> getSkeletonPos() const { return skeletonPos; }  // Lấy vị trí skeleton
    int getX() const { return x; }  // Lấy tọa độ X
    int getY() const { return y; }  // Lấy tọa độ Y
};
