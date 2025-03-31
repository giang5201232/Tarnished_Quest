#include <iostream>
#include <SDL.h>
#include <SDL_image.h>      // Thư viện xử lý ảnh
#include <SDL_ttf.h>        // Thư viện xử lý font chữ
#include <SDL_mixer.h>       // Thư viện xử lý âm thanh
#include <ctime>            // Thư viện thời gian để khởi tạo số ngẫu nhiên
#include <cstdlib>          // Thư viện tiêu chuẩn C

#include "RenderWindow.h"    // Header file cho lớp cửa sổ render
#include "Game.h"           // Header file cho lớp game chính
using namespace std;

/**
 * Hàm main - điểm vào chương trình
 * @param argc Số lượng tham số dòng lệnh
 * @param argv Mảng chứa các tham số dòng lệnh
 */
int main(int argc, char* argv[]) {
    Game game; // Tạo đối tượng game chính

    // Khởi tạo seed cho hàm random
    srand(time(NULL));

    // Khởi tạo SDL và các thư viện phụ trợ
    if (!game.init()) {
        return 0; // Thoát nếu khởi tạo thất bại
    }
    else {
        // Tải các tài nguyên media (ảnh, âm thanh, font chữ)
        if (!game.loadMedia()) {
            return 0; // Thoát nếu tải media thất bại
        }
        else {
            // Tạo các thành phần của game: bản đồ, level, menu, người chơi, enemy
            if (!game.createMap() || !game.createLevel() || !game.createMenu() || !game.createPlayer() || !game.createSkeleton()) {
                cout << "FAILED TO CREATE GAME ELEMENTS!" << endl;
                return 0; // Thoát nếu tạo các thành phần game thất bại
            }

            // Vòng lặp game chính
            while (game.isRunning()) {
                SDL_Event event;

                // Xử lý các sự kiện đầu vào
                while (SDL_PollEvent(&event)) {
                    game.handleGameInput(event);
                }

                // Logic hiển thị và cập nhật game
                if (game.getMenuList().at(0).isMenu()) {
                    game.render_mainMenu(); // Hiển thị menu chính
                }
                else if (game.getMenuList().at(0).isPaused()) {
                    game.pauseMusicTime(); // Tạm dừng nhạc khi game pause
                }
                else {
                    game.playMusic(); // Phát nhạc game
                    game.render_update_Game(); // Render và cập nhật game
                }
            }
        }
    }

    // Dọn dẹp tài nguyên khi thoát game
    game.destroy();
    commonFunc::cleanUp();

    return 0;
}
