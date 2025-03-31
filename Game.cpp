#include "Game.h"

// Khởi tạo các thư viện SDL
bool Game::init() {
    bool success = true;

    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        cout << "SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << endl;
        success = false;
    }

    // Khởi tạo SDL_image cho PNG
    if (!(IMG_Init(IMG_INIT_PNG))) {
        cout << "IMG_Init HAS FAILED. SDL_ERROR: " << IMG_GetError() << endl;
        success = false;
    }

    // Khởi tạo SDL_image cho JPG
    if (!(IMG_Init(IMG_INIT_JPG))) {
        cout << "IMG_Init HAS FAILED. SDL_ERROR: " << IMG_GetError << endl;
        success = false;
    }

    // Khởi tạo SDL_ttf
    if (TTF_Init() == -1) {
        cout << "TTF_Init HAS FAILED. SDL_ERROR: " << TTF_GetError() << endl;
        success = false;
    }

    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        success = false;
    }

    // Tạo cửa sổ game nếu khởi tạo thành công
    if (success == true) commonFunc::renderWindow("Tarnished Quest", SCREEN_WIDTH, SCREEN_HEIGHT);

    return success;
}

// Tải các tài nguyên game
bool Game::loadMedia() {
    bool success = true;

    // Tải font chữ
    if (!commonFunc::loadFont("res/Pixel-UniCode.ttf")) success = false;

    // Tải các texture
    knightTex = commonFunc::loadTexture("res/texture/Tarnished.png");
    if (knightTex == NULL) success = false;

    tileTex = commonFunc::loadTexture("res/texture/Tileset.png");
    if (tileTex == NULL) success = false;

    bulletTex = commonFunc::loadTexture("res/texture/Bullet.png");
    if (bulletTex == NULL) success = false;

    skeletonTex = commonFunc::loadTexture("res/texture/Skeleton.png");
    if (skeletonTex == NULL) success = false;

    bgTex = commonFunc::loadTexture("res/texture/MenuBg.png ");
    if (bgTex == NULL) success = false;

    buttonTex = commonFunc::loadTexture("res/texture/Button.png");
    if (buttonTex == NULL) success = false;

    // Tải nhạc nền
    bgMusic = Mix_LoadMUS("res/sfx/Vault in Tower Fortress Soundtrack.mp3");
    if (bgMusic == NULL) success = false;

    // Tải hiệu ứng âm thanh cho player
    playerSFX[0] = Mix_LoadWAV("res/sfx/sfx_sounds_impact12 (hit).wav"); // hit
    if (playerSFX[0] == NULL) success = false;

    playerSFX[1] = Mix_LoadWAV("res/sfx/sfx_movement_jump1.wav"); //jumping
    if (playerSFX[1] == NULL) success = false;

    playerSFX[2] = Mix_LoadWAV("res/sfx/sfx_sounds_impact1 (landing).wav"); //landing
    if (playerSFX[2] == NULL) success = false;

    playerSFX[3] = Mix_LoadWAV("res/sfx/sfx_wpn_laser7.wav"); //shooting
    if (playerSFX[3] == NULL) success = false;

    // Tải hiệu ứng âm thanh cho skeleton
    skeletonSFX[0] = Mix_LoadWAV("res/sfx/sfx_exp_short_hard16.wav"); //died
    if (skeletonSFX[0] == NULL) success = false;

    skeletonSFX[1] = Mix_LoadWAV("res/sfx/sfx_damage_hit2.wav"); //hit
    if (skeletonSFX[1] == NULL) success = false;

    if (!success) cout << "FAILED TO LOAD MEDIA: " << SDL_GetError() << endl;
    return success;
}

// Hiển thị FPS
void Game::FPSCounter() {
    int avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
    if (avgFPS > 2000000) {
        avgFPS = 0;
    }
    timeText.str("");
    timeText << "FPS: " << avgFPS;

    SDL_Color whiteColor = { 255,255,255,255 };
    SDL_Texture* textTex = commonFunc::createText(timeText.str().c_str(), whiteColor);
    Entity text(64*3, 0, textTex);
    commonFunc::renderTexture(text);
    ++countedFrames;
}

// Hiển thị điểm số
void Game::renderScore() {
    // Cập nhật điểm số dựa trên vị trí player
    if (score < playerList.at(0).getX() / TILE_WIDTH) score = playerList.at(0).getX() / TILE_WIDTH;

    scoreText.str("");
    scoreText << "Score: " << score << "m";
    SDL_Color whiteColor = { 255,255,255,255 };
    SDL_Color yellowColor = { 252,226,5,255 };

    // Đọc điểm cao nhất từ file
    ifstream readFile;
    readFile.open("res/highscore.txt");
    if (readFile.fail()) update_hight_score();
    readFile >> highscore;
    highscoreText.str("");
    highscoreText << "High Score: " << highscore << "m";

    // Render điểm số
    SDL_Texture* scoreTex = commonFunc::createText(scoreText.str().c_str(), yellowColor);
    SDL_Texture* highscoreTex = commonFunc::createText(highscoreText.str().c_str(), whiteColor);
    Entity current_score(1100, 30, scoreTex);
    Entity high_score(1100, 0, highscoreTex);
    commonFunc::renderTexture(current_score);
    commonFunc::renderTexture(high_score);
}

// Tạo các map cho game
bool Game::createMap() {
    // Thêm các map vào danh sách
    path_pos Map1({ }, "res/texture/map1.map");
    mapList.push_back(Map1);
    path_pos Map2({ 15,5 }, "res/texture/map2.map");
    mapList.push_back(Map2);
    path_pos Map3({ 1,9,16,6,18,7 }, "res/texture/map3.map");
    mapList.push_back(Map3);
    path_pos Map4({ 9,9 }, "res/texture/map4.map");
    mapList.push_back(Map4);
    path_pos Map5({ 7,12,12,12,17,12 }, "res/texture/map5.map");
    mapList.push_back(Map5);
    path_pos Map6({ 9,12 }, "res/texture/map6.map");
    mapList.push_back(Map6);
    path_pos Map7({ 8,12,15,4,13,3 }, "res/texture/map7.map");
    mapList.push_back(Map7);
    path_pos Map8({ 14,6,16,11 }, "res/texture/map8.map");
    mapList.push_back(Map8);
    path_pos Map9({ 12,13 }, "res/texture/map9.map");
    mapList.push_back(Map9);
    path_pos Map10({ 10,12 }, "res/texture/map10.map");
    mapList.push_back(Map10);
    path_pos Map11({ 6,11 }, "res/texture/map11.map");
    mapList.push_back(Map11);
    path_pos Map12({ 13,12 }, "res/texture/map12.map");
    mapList.push_back(Map12);
    path_pos Map13({ 9,12,11,11,12,10 }, "res/texture/map13.map");
    mapList.push_back(Map13);
    path_pos Map14({ 12,11 }, "res/texture/map14.map");
    mapList.push_back(Map14);
    path_pos MapSpawn({ }, "res/texture/map_spawn.map");
    mapList.push_back(MapSpawn);

    // Kiểm tra số lượng map
    if (mapList.size() < TOTAL_MAP) {
        cout << "FAILED TO LOAD MAP!" << endl;
        return false;
    }
    return true;
}

// Tạo level từ các map
bool Game::createLevel() {
    for (int i = 0; i < TOTAL_LEVEL_PART; i++) {
        // Chọn map ngẫu nhiên
        int random = rand() % (TOTAL_MAP - 1);
        if (i == 0) random = TOTAL_MAP - 1; // Map đầu tiên là map spawn

        // Tạo level part mới
        LevelPart level(i * LEVEL_WIDTH, 0, mapList.at(random).path, tileTex);
        level.setSkeletonPos(mapList.at(random).skeletonPos);
        LevelPartList.push_back(level);
    }

    if (LevelPartList.size() < TOTAL_LEVEL_PART) {
        cout << "FAILED TO CREATE LEVEL!" << endl;
        return false;
    }
    return true;
}

// Tạo skeleton từ vị trí trong map
bool Game::createSkeleton() {
    for (int i = 0; i < LevelPartList.size(); i++) {
        if (LevelPartList.at(i).getSkeletonPos().size() > 0) {
            // Tạo skeleton từ các vị trí đã định
            for (int j = 0; j < LevelPartList.at(i).getSkeletonPos().size() - 1; j += 2) {
                Skeleton* skeleton = new Skeleton(
                    LevelPartList.at(i).getSkeletonPos().at(j) * TILE_WIDTH + LevelPartList.at(i).getX(),
                    LevelPartList.at(i).getSkeletonPos().at(j + 1) * TILE_WIDTH + LevelPartList.at(i).getY(),
                    skeletonTex);
                skeletonList.push_back(skeleton);
            }
        }
    }
    return true;
}

// Tạo player
bool Game::createPlayer() {
    Player knight(64 * 3, LEVEL_HEIGHT - TILE_HEIGHT * 5, knightTex);
    playerList.push_back(knight);
    if (playerList.size() < 0) return false;
    return true;
}

// Tạo menu
bool Game::createMenu() {
    Menu gameMenu(buttonTex, bgTex, bgTex);
    menuList.push_back(gameMenu);
    if (menuList.size() < 0) return false;
    return true;
}

// Phát nhạc nền
void Game::playMusic() {
    if (Mix_PlayingMusic() == 0) {
        // Phát nhạc nếu chưa phát
        Mix_FadeInMusic(bgMusic, -1, 1000);
        Mix_VolumeMusic(50);
    }
    else if (Mix_PausedMusic() == 1) Mix_ResumeMusic(); // Tiếp tục nếu đang tạm dừng
    else if (playerList.at(0).isDead()) Mix_HaltMusic(); // Dừng nếu player chết
}

// Tạm dừng nhạc
void Game::pauseMusicTime() {
    if (Mix_PlayingMusic() == 1) {
        Mix_PauseMusic();
    }
    fpsTimer.pause();
}

// Render và cập nhật level
void Game::render_update_LevelPart() {
    // Kiểm tra nếu cần tạo level mới
    if (LevelPartList.back().getX() + LEVEL_WIDTH - camera.x < SCREEN_WIDTH) {
        // Tạo level mới
        int random = rand() % (TOTAL_MAP - 1);
        float newX = LevelPartList.back().getX() + LEVEL_WIDTH;

        LevelPart newLevel(newX, 0, mapList.at(random).path, tileTex);
        newLevel.setSkeletonPos(mapList.at(random).skeletonPos);

        // Thêm enemy nếu có
        if (newLevel.getSkeletonPos().size() > 0) {
            for (int j = 0; j < newLevel.getSkeletonPos().size() - 1; j += 2) {
                Skeleton* skeleton = new Skeleton(
                    newLevel.getSkeletonPos().at(j) * TILE_WIDTH + newLevel.getX(),
                    newLevel.getSkeletonPos().at(j + 1) * TILE_WIDTH + newLevel.getY(),
                    skeletonTex);
                skeletonList.push_back(skeleton);
            }
        }

        LevelPartList.push_back(newLevel);
    }

    // Xóa các level quá xa về phía sau
    if (LevelPartList.size() > 5) { // Giữ lại 5 level phía sau
        if (LevelPartList.front().getX() + LEVEL_WIDTH < camera.x - SCREEN_WIDTH) {
            LevelPartList.erase(LevelPartList.begin());
        }
    }

    // Render tất cả level
    for (int i = 0; i < LevelPartList.size(); i++) {
        LevelPartList.at(i).render(gTileClips, camera);
    }
}

// Render và cập nhật đạn
void Game::render_update_bullet() {
    vector<Bullet*> bulletList = playerList.at(0).getBulletList();
    for (int i = 0; i < playerList.at(0).getBulletList().size(); i++) {
        if (bulletList.at(i) != NULL) {
            if (bulletList.at(i)->isMoving()) {
                bulletList.at(i)->render(camera, bulletTex);
                bulletList.at(i)->update(LevelPartList);
            }
            else {
                // Xóa đạn nếu không di chuyển
                delete bulletList.at(i);
                bulletList.at(i) = NULL;
                bulletList.erase(bulletList.begin() + i);
                playerList.at(0).setBulletList(bulletList);
            }
        }
    }
}

// Render và cập nhật player
void Game::render_update_player() {
    playerList.at(0).update(LevelPartList, skeletonList, playerSFX, camera);
    playerList.at(0).handleCamera(camera, camVel);
    playerList.at(0).render(camera);
}

// Render và cập nhật skeleton
void Game::render_update_skeleton() {
    for (int i = 0; i < skeletonList.size(); i++) {
        if (skeletonList.at(i) != NULL) {
            if (!skeletonList.at(i)->isDead()) {
                skeletonList.at(i)->render(camera);
                skeletonList.at(i)->update(playerList.at(0), LevelPartList, skeletonSFX, camera);
            }
            else {
                // Xóa skeleton nếu đã chết
                delete skeletonList.at(i);
                skeletonList.at(i) = NULL;
                skeletonList.erase(skeletonList.begin() + i);
            }
        }
    }
}

// Cập nhật điểm cao nhất
void Game::update_hight_score() {
    ofstream outFile;
    outFile.open("res/highscore.txt");
    outFile.clear();
    outFile << highscore;
    outFile.close();
}

// Render và cập nhật toàn bộ game
void Game::render_update_Game() {
    // Bắt đầu đếm thời gian
    fpsTimer.start();
    commonFunc::clearRenderer();

    // Render các thành phần game
    render_update_LevelPart();
    render_update_bullet();
    render_update_player();
    render_update_skeleton();
    FPSCounter();
    renderScore();

    // Xử lý khi player chết
    if (playerList.at(0).isDead()) {
        menuList.at(0).renderRetryMenu();
        if (score > highscore) highscore = score;
        update_hight_score();
    }
    fpsTimer.unpause();

    // Reset game nếu cần
    if (menuList.at(0).need_reseting()) resetGame();
    commonFunc::renderPresent();
}

// Render menu chính
void Game::render_mainMenu() {
    commonFunc::clearRenderer();
    getMenuList().at(0).renderMainMenu();
    commonFunc::renderPresent();
}

// Reset game về trạng thái ban đầu
void Game::resetGame() {
    playerList.at(0).resetPlayer();
    camera.x = 0;
    camera.y = 0;
    camVel = 1.5;

    // Xóa tất cả skeleton
    if(!skeletonList.empty())
        for (int i = skeletonList.size() - 1; i >= 0; i--) {
            delete skeletonList.at(i);
            skeletonList.at(i) = NULL;
            skeletonList.erase(skeletonList.begin() + i);
        }

    // Reset các level
    for (int i = 0; i < LevelPartList.size(); i++) {
        int random = rand() % (TOTAL_MAP - 1);
        if (i == 0) {
            random = TOTAL_MAP - 1;
            LevelPartList.at(i).setLevelX(0);
        }
        else LevelPartList.at(i).setLevelX(LevelPartList.at(i - 1));
        LevelPartList.at(i).setTilesType(mapList.at(random).path);
        LevelPartList.at(i).setSkeletonPos(mapList.at(random).skeletonPos);
    }

    // Tạo lại skeleton và reset các thông số
    createSkeleton();
    menuList.at(0).set_reset(false);
    fpsTimer.stop();
    fpsTimer.start();
    countedFrames = 0;
    score = 0;
}

// Xử lý input game
void Game::handleGameInput(SDL_Event& event) {
    if (event.type == SDL_QUIT) gameRunning = false;
    menuList.at(0).handleInput(event, gameRunning, playerList.at(0));
    if (!menuList.at(0).isMenu() && !menuList.at(0).isPaused()) playerList.at(0).handleInput(event, playerSFX);
}

// Thiết lập các SDL_Rect cho tile
void Game::setSDL_Rect() {
    int m = 0, n = 0;
    for (int i = 0; i < TOTAL_TILE_SPRITES; i++) {
        gTileClips[i].x = n;
        gTileClips[i].y = m;
        gTileClips[i].w = TILE_WIDTH;
        gTileClips[i].h = TILE_HEIGHT;
        n += TILE_WIDTH;
        if (n >= 17 * TILE_WIDTH) {
            n = 0;
            m += TILE_HEIGHT;
        }
    }
}
