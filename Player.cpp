#include "Player.h"

// Constructor: Khởi tạo player với vị trí và texture
Player::Player(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = getX() + PLAYER_WIDTH;  // Thiết lập vùng va chạm
	collision.y = getY() + PLAYER_HEIGHT;
	collision.w = PLAYER_WIDTH - 12; // Điều chỉnh cho vừa với hình dáng nhân vật
	collision.h = PLAYER_HEIGHT;

	// Thiết lập các khung hình animation đi bộ
	for (int i = 0; i < WALKING_ANIMATION_FRAMES; i++) {
		walkingClips[i].x = i * (getCurrentFrame().w/4);
		if (i >= 4) {  // Các frame đi bộ ở hàng thứ 3
			walkingClips[i].x = (i-4) * (getCurrentFrame().w/4);
			walkingClips[i].y = (getCurrentFrame().h/7)*2;
		}
		else walkingClips[i].y = getCurrentFrame().h/7;  // Các frame đi bộ ở hàng đầu
		walkingClips[i].w = getCurrentFrame().w/4;
		walkingClips[i].h = getCurrentFrame().h/7;
	}

	// Thiết lập các khung hình animation đứng yên
	for (int i = 0; i < IDLING_ANIMATION_FRAMES; i++) {
		idlingClips[i].x = i * (getCurrentFrame().w/4);
		idlingClips[i].y = 0;  // Hàng đầu tiên
		idlingClips[i].w = getCurrentFrame().w/4;
		idlingClips[i].h = getCurrentFrame().h/7;
	}

	// Thiết lập các khung hình animation nhảy
	for (int i = 0; i < JUMPING_ANIMATION_FRAMES; i++) {
		jumpingClips[i].x = i * (getCurrentFrame().w / 4);
		jumpingClips[i].y = (getCurrentFrame().h/ 7) * 3;  // Hàng thứ 4
		jumpingClips[i].w = getCurrentFrame().w / 4;
		jumpingClips[i].h = getCurrentFrame().h / 7;
	}

	// Thiết lập các khung hình animation rơi
	for (int i = 0; i < FALLING_ANIMATION_FRAMES; i++) {
		fallingClips[i].x = i * (getCurrentFrame().w / 4);
		fallingClips[i].y = (getCurrentFrame().h / 7) * 4;  // Hàng thứ 5
		fallingClips[i].w = getCurrentFrame().w / 4;
		fallingClips[i].h = getCurrentFrame().h / 7;
	}

	// Thiết lập các khung hình animation chết
	for (int i = 0; i < DEATH_ANIMATION_FRAMES; i++) {
		deathClips[i].x = i * (getCurrentFrame().w / 4);
		deathClips[i].y = (getCurrentFrame().h / 7) * 5;  // Hàng thứ 6
		deathClips[i].w = getCurrentFrame().w / 4;
		deathClips[i].h = getCurrentFrame().h / 7;
	}

	// Thiết lập các khung hình animation tấn công
	for (int i = 0; i < ATTACKING_ANIMATION_FRAMES; i++) {
		attackingClips[i].x = 0;  // Chỉ có 1 frame
		attackingClips[i].y = (getCurrentFrame().h / 7) * 6;  // Hàng cuối cùng
		attackingClips[i].w = getCurrentFrame().w / 4;
		attackingClips[i].h = getCurrentFrame().h / 7;
	}
}

// Xử lý input từ người chơi
void Player::handleInput(SDL_Event &events, Mix_Chunk* p_sfx[]) {
	if (!isDead()) {  // Chỉ xử lý input khi chưa chết
		if (events.type == SDL_KEYDOWN && events.key.repeat == 0) {  // Khi nhấn phím
			switch (events.key.keysym.sym) {
			case SDLK_a:  // Di chuyển sang trái
				xVel -= PLAYER_VEL;
				break;
			case SDLK_d:  // Di chuyển sang phải
				xVel += PLAYER_VEL;
				break;
			case SDLK_SPACE:  // Nhảy
				if (grounded) {
					jump();
					Mix_PlayChannel(-1, p_sfx[jumpSFX], 0);  // Phát âm thanh nhảy
				}
				break;
			}
		}
		else if (events.type == SDL_KEYUP && events.key.repeat == 0) {  // Khi thả phím
			switch (events.key.keysym.sym) {
			case SDLK_a:  // Dừng di chuyển trái
				xVel += PLAYER_VEL;
				break;
			case SDLK_d:  // Dừng di chuyển phải
				xVel -= PLAYER_VEL;
				break;
			case SDLK_SPACE:  // Giảm độ cao nhảy nếu thả sớm
				if (!grounded && jumping) {
					yVel *= .5f;
				}
				break;
			}
		}
		else if (events.type == SDL_MOUSEBUTTONDOWN && events.key.repeat == 0) {  // Khi click chuột
			if (events.button.button == SDL_BUTTON_LEFT) {  // Tấn công bằng chuột trái
				attacking = true;
				attackCounter = 0;
				// Tạo đạn
				Bullet* bullet = new Bullet(getCollision().x + PLAYER_WIDTH * 1.25, getCollision().y, NULL);
				Mix_PlayChannel(-1, p_sfx[shootSFX], 0);  // Âm thanh bắn
				bullet->setFlipType(getFlipType());
				bullet->setSize_Position(DEFAULTBULLET_W, DEFAULTBULLET_H, getX(), getY());
				bullet->setType(Bullet::NORMAL);
				bullet->setMove(true);
				bulletList.push_back(bullet);
			}
		}
	}
}

// Cập nhật trạng thái player mỗi frame
void Player::update(vector<LevelPart>& LevelPartList, vector<Skeleton*> &skeletonList, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
	gravity();  // Áp dụng trọng lực
	if(!dead) getHit(skeletonList, p_sfx, camera);  // Kiểm tra va chạm với quái

	// Cập nhật trạng thái player
	if (xVel == 0 && grounded && !dead) idling = true;  // Đứng yên
	else idling = false;

	if (xVel != 0 && grounded && !dead) running = true;  // Chạy
	else running = false;

	if (yVel > 0 && !grounded && !dead) falling = true;  // Rơi
	else falling = false;

	if (yVel <= 0 && !grounded && !dead) jumping = true;  // Nhảy
	else jumping = false;

	// Xử lý hướng quay nhân vật
	if (!beingHit) {
		if (xVel < 0) flipType = SDL_FLIP_HORIZONTAL;  // Quay trái
		if (xVel > 0) flipType = SDL_FLIP_NONE;       // Quay phải
	}

	// Xử lý trạng thái tấn công
	if (attacking) {
		idling = running = jumping = falling = false;  // Ưu tiên animation tấn công
	}
	if (attacking && attackCounter >= attackDelay) {
		attacking = false;  // Kết thúc tấn công sau delay
	}

	// Di chuyển theo trục X
	if (!dead) {
		x += xVel;
		collision.x = getX() + PLAYER_WIDTH;

		// Giới hạn di chuyển trái
		if (getX() + PLAYER_WIDTH < 0) {
			x = -PLAYER_WIDTH;
			collision.x = getX() + PLAYER_WIDTH;
		}
		// Xử lý va chạm tường
		if (commonFunc::touchesWall(collision, LevelPartList)) {
			x -= xVel;
			collision.x = getX() + PLAYER_WIDTH;
		}
	}

	// Di chuyển theo trục Y
	y += yVel;
	collision.y = getY() + PLAYER_HEIGHT;
	// Giới hạn di chuyển lên
	if (getY() + PLAYER_HEIGHT < 0) {
		y = -PLAYER_HEIGHT;
		collision.y = getY() + PLAYER_HEIGHT;
	}
	// Xử lý va chạm với mặt đất/trần
	if (commonFunc::touchesWall(collision, LevelPartList, grounded, groundSTT, levelSTT)) {
		if (yVel > 0) {  // Chạm đất
			y = LevelPartList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 64 * 2;
			if (falling) {
				grounded = true;
				Mix_PlayChannel(-1, p_sfx[landSFX], 0);  // Âm thanh tiếp đất
			}
		}
		else if (yVel < 0) {  // Chạm trần
			y -= yVel;
			yVel = 0;
		}
		collision.y = getY() + PLAYER_HEIGHT;
	}
}

// Xử lý nhảy
void Player::jump() {
	if (grounded) {
		yVel -= 10;  // Lực nhảy
		grounded = false;
	}
}

// Áp dụng trọng lực
void Player::gravity() {
	if (!grounded) {
		yVel += GRAVITY;  // Tăng tốc độ rơi
		if (yVel > MAX_GRAVITY) yVel = MAX_GRAVITY;  // Giới hạn tốc độ rơi
	}
	else yVel = GRAVITY;  // Lực nhỏ giữ player trên mặt đất
}

// Xử lý khi bị đánh
void Player::getHit(vector<Skeleton*> &skeletonList, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
	// Kiểm tra va chạm với skeleton
	for (size_t i = 0; i < skeletonList.size(); i++) {
		if(skeletonList.at(i) != NULL)
			if ((skeletonList.at(i)->getDistance() <= TILE_WIDTH * 1.5 && skeletonList.at(i)->isAttacking() && getY() >= skeletonList.at(i)->getY() - TILE_WIDTH && getY() <= skeletonList.at(i)->getY() + TILE_WIDTH * 0.5)) {
				dead = true;  // Chết nếu bị tấn công
				Mix_PlayChannel(-1, p_sfx[hitSFX], 0);  // Âm thanh bị đánh
			}
	}
	// Rơi khỏi map
	if (getY() + PLAYER_HEIGHT >= LEVEL_HEIGHT) {
		dead = true;
		Mix_PlayChannel(-1, p_sfx[hitSFX], 0);
	}
}

// Hiệu ứng bị đẩy lùi khi bị đánh
void Player::knockBack() {
	if (beingHit) {
		yVel += -4;  // Nhảy lên
		// Đẩy lùi theo hướng
		if (getFlipType() == SDL_FLIP_NONE) x += -100;  // Đẩy sang trái
		else if (getFlipType() == SDL_FLIP_HORIZONTAL) x += 10;  // Đẩy sang phải
	}
}

// Xử lý camera theo player
void Player::handleCamera(SDL_Rect& camera, float& camVel) {
	// Tăng tốc độ camera dần
	float acc = 0.001;
	if (camVel > 4) acc = 0.0003;
	if (camVel > 5) acc = 0.00001;
	camVel += acc;

	// Camera theo trục X
	if (getX() + PLAYER_WIDTH / 2 - camera.x >= SCREEN_WIDTH * 1 / 2) {
		camera.x = (getX() + PLAYER_WIDTH / 2) - SCREEN_WIDTH * 1 / 2;
	}
	// Camera theo trục Y
	camera.y = (getY() + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

	// Giới hạn camera không vượt biên map
	if (camera.x < 0) camera.x = 0;
	if (camera.y < 0) camera.y = 0;
	if (camera.y > LEVEL_HEIGHT - camera.h) camera.y = LEVEL_HEIGHT - camera.h;
}

// Render player theo trạng thái
void Player::render(SDL_Rect &p_camera) {
	// Animation chạy
	if (running) {
		commonFunc::renderAnimation(tex, x, y, walkingClips[walkCounter / 4], p_camera, 0, NULL, getFlipType());
		walkCounter++;
		if (walkCounter/4 >= WALKING_ANIMATION_FRAMES) walkCounter = 0;
	}

	// Animation đứng yên
	if (idling) {
		commonFunc::renderAnimation(tex, x, y, idlingClips[idleCounter/12], p_camera, 0, NULL, getFlipType());
		idleCounter++;
		if (idleCounter / 12 >= IDLING_ANIMATION_FRAMES) idleCounter = 0;
	}
	else idleCounter = 0;

	// Animation nhảy
	if (jumping) {
		commonFunc::renderAnimation(tex, x, y, jumpingClips[jumpCounter / 10], p_camera, 0, NULL, getFlipType());
		jumpCounter++;
		if (jumpCounter / 10 >= JUMPING_ANIMATION_FRAMES) jumpCounter = 0;
	}
	else jumpCounter = 0;

	// Animation rơi
	if (falling) {
		commonFunc::renderAnimation(tex, x, y, fallingClips[fallingCounter / 8], p_camera, 0, NULL, getFlipType());
		fallingCounter++;
		if (fallingCounter / 8 >= FALLING_ANIMATION_FRAMES) fallingCounter = 0;
	}
	else fallingCounter = 0;

	// Animation chết
	if (dead) {
		commonFunc::renderAnimation(tex, x, y, deathClips[deathCounter / 10], p_camera, 0, NULL, getFlipType());
		if(deathCounter / 10 < DEATH_ANIMATION_FRAMES-1) deathCounter++;
	}
	else deathCounter = 0;

	// Animation tấn công
	if (attacking) {
		commonFunc::renderAnimation(tex, x, y, attackingClips[0], p_camera, 0, NULL, getFlipType());
		attackCounter++;
		if (attackCounter >= attackDelay) {
			attacking = false;
			attackCounter = 0;
		}
	}
}
