/**
 * Triển khai các hàm xử lý đồ họa và quản lý cửa sổ game
 * Bao gồm toàn bộ chức năng render, tải tài nguyên và xử lý va chạm
 */

#include "RenderWindow.h"
using namespace std;

// ==============================================
// HÀM KHỞI TẠO CỬA SỔ VÀ RENDERER
// ==============================================

/**
 * Tạo cửa sổ game và renderer
 * @param p_title Tiêu đề cửa sổ
 * @param p_width Chiều rộng cửa sổ (pixel)
 * @param p_height Chiều cao cửa sổ (pixel)
 */
void commonFunc::renderWindow(const char* p_title, int p_width, int p_height) {
    // Tạo cửa sổ game ở vị trí giữa màn hình
    window = SDL_CreateWindow(p_title,
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            p_width,
                            p_height,
                            SDL_WINDOW_SHOWN);

    if (window == NULL) {
        cout << "Không thể tạo cửa sổ: " << SDL_GetError() << endl;
    }

    // Tạo renderer với tăng tốc phần cứng và đồng bộ dọc
    renderer = SDL_CreateRenderer(window,
                                -1,
                                SDL_RENDERER_ACCELERATED |
                                SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        cout << "Không thể tạo renderer: " << SDL_GetError() << endl;
    }
}

// ==============================================
// CÁC HÀM XỬ LÝ TEXTURE
// ==============================================

/**
 * Tải texture từ file ảnh
 * @param p_filePath Đường dẫn đến file ảnh
 * @return Con trỏ SDL_Texture hoặc NULL nếu thất bại
 */
SDL_Texture* commonFunc::loadTexture(const char* p_filePath) {
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL) {
        cout << "Không thể tải texture: " << IMG_GetError() << endl;
    }

    return texture;
}

/**
 * Render một Entity lên màn hình
 * @param entity Entity cần render
 * @param rec Khung hình cần render (NULL để render toàn bộ)
 * @param camera Vị trí camera để tính toán tọa độ
 * @param angle Góc xoay (độ)
 * @param center Điểm xoay (tâm)
 * @param flip Hướng lật hình
 */
void commonFunc::renderTexture(Entity& entity, SDL_Rect* rec, SDL_Rect* camera,
                             double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect dst = {
        entity.getX(),
        entity.getY(),
        entity.getCurrentFrame().w,
        entity.getCurrentFrame().h
    };

    if (rec != NULL) {
        dst.w = rec->w;
        dst.h = rec->h;
    }

    if (camera != NULL) {
        dst.x -= camera->x;
        dst.y -= camera->y;
    }

    SDL_RenderCopyEx(renderer, entity.getTex(), rec, &dst, angle, center, flip);
}

/**
 * Render một texture bình thường
 * @param p_tex Texture cần render
 * @param p_x Tọa độ x
 * @param p_y Tọa độ y
 * @param p_w Chiều rộng
 * @param p_h Chiều cao
 * @param rec Khung hình cắt
 * @param camera Vị trí camera
 * @param angle Góc xoay
 * @param center Điểm xoay
 * @param flip Hướng lật
 */
void commonFunc::renderTexture(SDL_Texture* p_tex, float p_x, float p_y,
                             float p_w, float p_h, SDL_Rect* rec,
                             SDL_Rect* camera, double angle,
                             SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect dst = { p_x, p_y, p_w, p_h };

    if (rec != NULL) {
        dst.w = rec->w;
        dst.h = rec->h;
    }

    if (camera != NULL) {
        dst.x -= camera->x;
        dst.y -= camera->y;
    }

    SDL_RenderCopyEx(renderer, p_tex, rec, &dst, angle, center, flip);
}

/**
 * Render một tile
 * @param entity Tile entity
 * @param rec Khung hình cắt
 * @param camera Vị trí camera
 */
void commonFunc::renderTile(Entity& entity, SDL_Rect& rec, SDL_Rect& camera) {
    SDL_Rect dst = {
        entity.getX() - camera.x,
        entity.getY() - camera.y,
        rec.w,
        rec.h
    };
    SDL_RenderCopy(renderer, entity.getTex(), &rec, &dst);
}

/**
 * Render animation
 * @param p_tex Texture chứa animation
 * @param p_x Tọa độ x
 * @param p_y Tọa độ y
 * @param p_clip Khung hình cắt
 * @param p_camera Vị trí camera
 * @param p_angle Góc xoay
 * @param p_center Điểm xoay
 * @param p_flip Hướng lật
 */
void commonFunc::renderAnimation(SDL_Texture* p_tex, float p_x, float p_y,
                               SDL_Rect& p_clip, SDL_Rect& p_camera,
                               double p_angle, SDL_Point* p_center,
                               SDL_RendererFlip p_flip) {
    SDL_Rect src = { p_clip.x, p_clip.y, p_clip.w, p_clip.h };
    SDL_Rect dst = {
        p_x - p_camera.x,
        p_y - p_camera.y,
        p_clip.w,
        p_clip.h
    };
    SDL_RenderCopyEx(renderer, p_tex, &p_clip, &dst, p_angle, p_center, p_flip);
}

/**
 * Render player
 * @param entity Player entity
 * @param camera Vị trí camera
 */
void commonFunc::renderPlayer(Entity& entity, SDL_Rect& camera) {
    SDL_Rect src = { 0, 0, entity.getCurrentFrame().w, entity.getCurrentFrame().h };
    SDL_Rect dst = {
        entity.getX() - camera.x,
        entity.getY() - camera.y,
        entity.getCurrentFrame().w,
        entity.getCurrentFrame().h
    };
    SDL_RenderCopy(renderer, entity.getTex(), &src, &dst);
}

// ==============================================
// CÁC HÀM XỬ LÝ VA CHẠM
// ==============================================

/**
 * Kiểm tra va chạm giữa 2 hình chữ nhật
 * @param a Hình chữ nhật thứ nhất
 * @param b Hình chữ nhật thứ hai
 * @return true nếu có va chạm, false nếu không
 */
bool commonFunc::checkCollision(SDL_Rect a, SDL_Rect b) {
    // Tính toán các cạnh của hình a
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;

    // Tính toán các cạnh của hình b
    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;

    // Kiểm tra các điều kiện không va chạm
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;

    return true;
}

/**
 * Kiểm tra va chạm với tường (đơn giản)
 * @param box Hình chữ nhật cần kiểm tra
 * @param LevelPartList Danh sách các phần level
 * @return true nếu có va chạm, false nếu không
 */
bool commonFunc::touchesWall(SDL_Rect& box, vector<LevelPart>& LevelPartList) {
    for (int i = 0; i < LevelPartList.size(); i++) {
        // Kiểm tra nếu box nằm trong phạm vi level part
        if (box.x > LevelPartList.at(i).getX() &&
            box.x + box.w + 13 < LevelPartList.at(i).getX() + LEVEL_WIDTH &&
            box.y >= 0 &&
            box.y < LEVEL_HEIGHT - TILE_HEIGHT) {

            // Tính toán vị trí các tile xung quanh
            int cot_left = (box.x - LevelPartList.at(i).getX()) / TILE_WIDTH;
            int cot_rigth = cot_left + 1;
            int dong_up = box.y / TILE_HEIGHT;
            int dong_down = dong_up + 1;

            // Tính chỉ số các tile cần kiểm tra
            int stt1 = dong_up * 21 + cot_rigth;
            int stt2 = dong_down * 21 + cot_rigth;
            int stt3 = dong_up * 21 + cot_left;
            int stt4 = dong_down * 21 + cot_left;

            // Kiểm tra va chạm với từng tile
            if ((LevelPartList.at(i).getTilesList().at(stt1)->getType() >= 0) &&
                (LevelPartList.at(i).getTilesList().at(stt1)->getType() <= 84))
                if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt1)->getCollision()))
                    return true;

            if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() >= 0) &&
                (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84))
                if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt2)->getCollision()))
                    return true;

            if ((LevelPartList.at(i).getTilesList().at(stt3)->getType() >= 0) &&
                (LevelPartList.at(i).getTilesList().at(stt3)->getType() <= 84))
                if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt3)->getCollision()))
                    return true;

            if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() >= 0) &&
                (LevelPartList.at(i).getTilesList().at(stt4)->getType() <= 84))
                if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt4)->getCollision()))
                    return true;
        }
    }
    return false;
}

/**
 * Kiểm tra va chạm với tường (nâng cao)
 * @param box Hình chữ nhật cần kiểm tra
 * @param LevelPartList Danh sách các phần level
 * @param grounded Tham chiếu trạng thái đứng trên mặt đất
 * @param groundSTT Tham chiếu chỉ số tile đang đứng
 * @param levelSTT Tham chiếu chỉ số level part
 * @return true nếu có va chạm, false nếu không
 */
bool commonFunc::touchesWall(SDL_Rect& box, vector<LevelPart>& LevelPartList,
                           bool& grounded, int& groundSTT, int& levelSTT) {
    bool check = false;
    for (int i = 0; i < LevelPartList.size(); i++) {
        if (box.x + box.w + 12 >= LevelPartList.at(i).getX() &&
            box.x <= LevelPartList.at(i).getX() + LEVEL_WIDTH &&
            box.y >= 0 &&
            box.y < LEVEL_HEIGHT - TILE_HEIGHT) {

            // Tính toán vị trí các tile xung quanh
            int cot_left = (box.x - LevelPartList.at(i).getX()) / TILE_WIDTH;
            int cot_rigth = cot_left + 1;
            int dong_up = box.y / TILE_HEIGHT;
            int dong_down = dong_up + 1;

            // Tính chỉ số các tile cần kiểm tra
            int stt1 = dong_up * 21 + cot_rigth;
            int stt2 = dong_down * 21 + cot_rigth;
            int stt3 = dong_up * 21 + cot_left;
            int stt4 = dong_down * 21 + cot_left;

            // Kiểm tra nếu ở rìa level part
            if (box.x <= LevelPartList.at(i).getX() &&
                box.x + box.w + 12 >= LevelPartList.at(i).getX() ||
                box.x <= LevelPartList.at(i).getX() + LEVEL_WIDTH &&
                box.x + box.w + 12 >= LevelPartList.at(i).getX() + LEVEL_WIDTH) {
                grounded = false;
            }
            else {
                // Kiểm tra va chạm với từng tile
                if ((LevelPartList.at(i).getTilesList().at(stt1)->getType() >= 0) &&
                    (LevelPartList.at(i).getTilesList().at(stt1)->getType() <= 84))
                    if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt1)->getCollision()))
                        check = true;

                if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() >= 0) &&
                    (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84))
                    if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt2)->getCollision()))
                        check = true;

                if ((LevelPartList.at(i).getTilesList().at(stt3)->getType() >= 0) &&
                    (LevelPartList.at(i).getTilesList().at(stt3)->getType() <= 84))
                    if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt3)->getCollision()))
                        check = true;

                if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() >= 0) &&
                    (LevelPartList.at(i).getTilesList().at(stt4)->getType() <= 84))
                    if (checkCollision(box, LevelPartList.at(i).getTilesList().at(stt4)->getCollision()))
                        check = true;

                // Xử lý trạng thái đứng trên mặt đất
                if ((LevelPartList.at(i).getTilesList().at(stt2)->getType() > 84) &&
                    (LevelPartList.at(i).getTilesList().at(stt4)->getType() > 84))
                    grounded = false;

                if ((LevelPartList.at(i).getTilesList().at(stt4)->getType() > 84) &&
                    (LevelPartList.at(i).getTilesList().at(stt2)->getType() <= 84) &&
                    box.x + box.w <= LevelPartList.at(i).getTilesList().at(stt2)->getX())
                    grounded = false;
            }

            // Cập nhật thông tin tile và level part
            groundSTT = stt4;
            levelSTT = i;
        }
    }
    return check;
}

// ==============================================
// CÁC HÀM XỬ LÝ FONT CHỮ
// ==============================================

/**
 * Tải font chữ từ file
 * @param filePath Đường dẫn file font
 * @return true nếu thành công, false nếu thất bại
 */
bool commonFunc::loadFont(const char* filePath) {
    // Đóng font hiện tại nếu có
    TTF_CloseFont(font);

    // Mở font mới với size 28
    font = TTF_OpenFont(filePath, 28);

    if (font == NULL) {
        printf("Không thể tải font: %s\n", TTF_GetError());
        return false;
    }

    return true;
}

/**
 * Tạo texture từ chuỗi văn bản
 * @param p_text Chuỗi văn bản
 * @param p_textColor Màu sắc văn bản
 * @return Texture chứa văn bản hoặc NULL nếu thất bại
 */
SDL_Texture* commonFunc::createText(string p_text, SDL_Color p_textColor) {
    // Tạo surface từ chuỗi văn bản
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, p_text.c_str(), p_textColor);
    SDL_Texture* texture = NULL;

    if (textSurface == NULL) {
        printf("Không thể tạo surface từ text: %s\n", TTF_GetError());
    }
    else {
        // Tạo texture từ surface
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == NULL) {
            printf("Không thể tạo texture từ surface: %s\n", SDL_GetError());
        }

        // Giải phóng surface sau khi sử dụng
        SDL_FreeSurface(textSurface);
        return texture;
    }

    return NULL;
}

// ==============================================
// CÁC HÀM TIỆN ÍCH RENDERER
// ==============================================

/**
 * Tô màu cửa sổ (chủ yếu để test)
 */
void commonFunc::Color() {
    surface = SDL_GetWindowSurface(window);
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 255, 0, 0));
    SDL_UpdateWindowSurface(window);
}

/**
 * Xóa renderer với màu trắng
 */
void commonFunc::clearRenderer() {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
}

/**
 * Cập nhật renderer lên màn hình
 */
void commonFunc::renderPresent() {
    SDL_RenderPresent(renderer);
}

/**
 * Dọn dẹp và giải phóng tất cả tài nguyên
 */
void commonFunc::cleanUp() {
    // Hủy cửa sổ và renderer
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = NULL;
    renderer = NULL;

    // Đóng các thư viện SDL
    Mix_Quit();  // Thư viện âm thanh
    TTF_Quit();  // Thư viện font chữ
    IMG_Quit();  // Thư viện hình ảnh
    SDL_Quit();  // Thư viện SDL chính
}
