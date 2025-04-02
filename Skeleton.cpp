#include "Skeleton.h"

// Constructor: Khởi tạo skeleton với vị trí và texture
Skeleton::Skeleton(float p_x, float p_y, SDL_Texture* p_tex) : Entity(p_x, p_y, p_tex) {
	collision.x = getX() + SKELETON_WIDTH;  // Thiết lập vùng va chạm
	collision.y = getY() + SKELETON_HEIGHT;
	collision.w = SKELETON_WIDTH-12;
	collision.h = SKELETON_HEIGHT-2;

	// Thiết lập các khung hình animation đi bộ
	for (int i = 0; i < WALKING_ANIMATION_FRAMES; i++) {
		walkingClips[i].x = i * (getCurrentFrame().w / 4);
		walkingClips[i].y = getCurrentFrame().h / 5;
		walkingClips[i].w = getCurrentFrame().w / 4;
		walkingClips[i].h = getCurrentFrame().h / 5;
	}
	// Thiết lập các khung hình animation đứng yên
	for (int i = 0; i < IDLING_ANIMATION_FRAMES; i++) {
		idlingClips[i].x = i * (getCurrentFrame().w / 4);
		idlingClips[i].y = 0;
		idlingClips[i].w = getCurrentFrame().w / 4;
		idlingClips[i].h = getCurrentFrame().h / 5;
	}
	// Thiết lập các khung hình animation rơi
	for (int i = 0; i < FALLING_ANIMATION_FRAMES; i++) {
		fallingClips[i].x = i * (getCurrentFrame().w / 4);
		fallingClips[i].y = (getCurrentFrame().h / 5) * 4;
		fallingClips[i].w = getCurrentFrame().w / 4;
		fallingClips[i].h = getCurrentFrame().h / 5;
	}
	// Thiết lập các khung hình animation tấn công
	for (int i = 0; i < ATTACKING_ANIMATION_FRAMES; i++) {
		attackingClips[i].x = i * (getCurrentFrame().w / 4);
		attackingClips[i].y = (getCurrentFrame().h / 5) * 3;
		attackingClips[i].w = getCurrentFrame().w / 4;
		attackingClips[i].h = getCurrentFrame().h / 5;
	}
	// Thiết lập các khung hình animation bị đánh
	for (int i = 0; i < BEINGHIT_ANIMATION_FRAMES; i++) {
		beingHitClips[i].x = i * (getCurrentFrame().w / 4);
		beingHitClips[i].y = (getCurrentFrame().h / 5) * 2;
		beingHitClips[i].w = getCurrentFrame().w / 4;
		beingHitClips[i].h = getCurrentFrame().h / 5;
	}
}

// Cập nhật trạng thái skeleton mỗi frame
void Skeleton::update(Player& p_player, vector<LevelPart>& LevelPartList, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
	if (!beingHit) {
		if (xVel < 0) flipType = SDL_FLIP_HORIZONTAL;  // Lật hình khi di chuyển trái
		if (xVel > 0) flipType = SDL_FLIP_NONE;       // Không lật khi di chuyển phải
	}
	gravity();  // Áp dụng trọng lực
	getHit(p_player, p_sfx, camera);  // Xử lý khi bị đánh
	autoMovement(LevelPartList);  // Di chuyển tự động
	moveToPlayer(p_player, LevelPartList);  // Đuổi theo player
	knockBack();  // Hiệu ứng bị đẩy lùi

	// Cập nhật trạng thái skeleton
	if (xVel == 0 && grounded && !attacking && !dead && !beingHit) idling = true;
	else idling = false;

	if (xVel != 0 && grounded && !attacking && !dead && !beingHit) walking = true;
	else walking = false;

	if (yVel > 0 && !grounded && !dead && !beingHit) falling = true;
	else falling = false;

	// Di chuyển theo trục X
	if (!attacking) {
		x += xVel;
		collision.x = getX() + SKELETON_WIDTH;

		if (getX() + SKELETON_WIDTH < 0) {
			x = -SKELETON_WIDTH;
			collision.x = getX() + SKELETON_WIDTH;
			xVel *= -1;  // Đổi hướng khi chạm biên trái
		}
		if (commonFunc::touchesWall(collision, LevelPartList)) {
			x -= xVel;
			collision.x = getX() + SKELETON_WIDTH;
			xVel *= -1;  // Đổi hướng khi chạm tường
		}
	}

	// Di chuyển theo trục Y
	y += yVel;
	collision.y = getY() + SKELETON_HEIGHT;
	if (getY() + SKELETON_HEIGHT < 0) {
		y = -SKELETON_HEIGHT;
		collision.y = getY() + SKELETON_HEIGHT;
	}
	if (commonFunc::touchesWall(collision, LevelPartList, grounded, groundSTT, levelSTT)) {
		if (yVel > 0) {
			y = LevelPartList.at(levelSTT).getTilesList().at(groundSTT)->getY() - 64 * 2;
			if (falling) {
				grounded = true;  // Tiếp đất
			}
		}
		else if (yVel < 0) {
			y -= yVel;
			yVel = 0;  // Dừng lại khi chạm trần
		}
		collision.y = getY() + SKELETON_HEIGHT;
	}
}

// Áp dụng trọng lực cho skeleton
void Skeleton::gravity() {
	if (!grounded) {
		yVel += GRAVITY;
		if (yVel > MAX_GRAVITY) yVel = MAX_GRAVITY;  // Giới hạn tốc độ rơi
	}
	else yVel = GRAVITY;  // Duy trì trọng lực nhỏ khi đang đứng
}

// Di chuyển tự động theo AI
void Skeleton::autoMovement(vector<LevelPart>& LevelPartList) {
	if (grounded && !beingHit) {
		if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 84 && LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 2)->getType() > 84) xVel = 0;
		else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 84 && xVel > 0) xVel = -SKELETON_VEL * 0.5;  // Đổi hướng khi gặp chướng ngại vật bên phải
		else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 1)->getType() > 84 && xVel < 0) xVel = SKELETON_VEL * 0.5;   // Đổi hướng khi gặp chướng ngại vật bên trái
		else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 2)->getType() > 84 && LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 2)->getType() > 84) xVel = 0;
		else if(getFlipType() == SDL_FLIP_NONE) xVel = SKELETON_VEL * 0.5;  // Di chuyển phải
		else if(getFlipType() == SDL_FLIP_HORIZONTAL) xVel = -SKELETON_VEL * 0.5;  // Di chuyển trái
	}
}

// Di chuyển về phía player khi trong tầm nhìn
void Skeleton::moveToPlayer(Player& p_player, vector<LevelPart>& LevelPartList) {
	distanceToPlayer = sqrt(pow(p_player.getX() - getX(), 2) + pow(p_player.getY() - getY(), 2));
	if (!beingHit) {
		// Kiểm tra player trong tầm nhìn
		if ((p_player.getY() >= getY() - TILE_WIDTH && p_player.getY() <= getY() + TILE_WIDTH * 0.5) && distanceToPlayer <= TILE_WIDTH * 7) {
			if (p_player.getX() - getX() < 0) {
				if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT - 1)->getType() > 84) xVel = 0;
				else xVel = -SKELETON_VEL;  // Di chuyển sang trái
			}
			else if (LevelPartList.at(levelSTT).getTilesList().at(groundSTT + 1)->getType() > 84) xVel = 0;
			else xVel = SKELETON_VEL;  // Di chuyển sang phải
		}
	}
	// Kích hoạt tấn công khi đủ gần
	if ( (distanceToPlayer <= TILE_WIDTH * 1.5 || (p_player.getY() >= getY() - TILE_WIDTH * 2.5 && p_player.getY() <= getY() - 64 && distanceToPlayer <= TILE_WIDTH * 2.5)) && !dead && !beingHit && grounded) attacking = true;
	else attacking = false;
}

// Kiểm tra skeleton đang tấn công
bool Skeleton::isAttacking() {
	if (attackingCounter / 21 >= 2) return true;  // Chỉ trả về true khi đạt frame tấn công hiệu quả
	return false;
}

// Xử lý khi skeleton bị đánh
void Skeleton::getHit(Player& p_player, Mix_Chunk* p_sfx[], SDL_Rect& camera) {
	for (int i = 0; i < p_player.getBulletList().size(); i++) {
		if (p_player.getBulletList().at(i) != NULL) {
			if (commonFunc::checkCollision(p_player.getBulletList().at(i)->getCollision(), getCollision())) {
				if (p_player.getBulletList().at(i)->getX() >= getX() + SKELETON_WIDTH && p_player.getBulletList().at(i)->getX() <= getX()+SKELETON_WIDTH*1.5) {
					beingHit = true;  // Kích hoạt trạng thái bị đánh
					maxHealth--;     // Giảm máu
					p_player.getBulletList().at(i)->setMove(false);  // Dừng đạn
				}
			}
		}
	}
	if (beingHit && beingHitCounter == 0) Mix_PlayChannel(-1, p_sfx[1], 0);  // Phát âm thanh bị đánh

	if (beingHitCounter / 7 >= BEINGHIT_ANIMATION_FRAMES) {
		beingHit = false;  // Kết thúc trạng thái bị đánh
		beingHitCounter = 0;
	}

	if (maxHealth <= 0 || getY() + SKELETON_HEIGHT/2 > LEVEL_HEIGHT) {
		dead = true;  // Đánh dấu đã chết
		beingHit = false;
		Mix_PlayChannel(-1, p_sfx[0], 0);  // Phát âm thanh chết
	}
}

// Hiệu ứng bị đẩy lùi khi bị đánh
void Skeleton::knockBack() {
	if (beingHit && beingHitCounter==0) {
		yVel = -3;  // Nhảy lên
		if (getFlipType() == SDL_FLIP_NONE) xVel = -4;  // Đẩy lùi sang trái
		else if(getFlipType() == SDL_FLIP_HORIZONTAL ) xVel = 4;  // Đẩy lùi sang phải
	}
}

// Render skeleton theo trạng thái hiện tại
void Skeleton::render(SDL_Rect& p_camera) {
	// Render animation đi bộ
	if (walking) {
		commonFunc::renderAnimation(tex, x, y, walkingClips[walkCounter / 12], p_camera, 0, NULL, getFlipType());
		walkCounter++;
		if (walkCounter / 12 >= WALKING_ANIMATION_FRAMES) walkCounter = 0;
	}

	// Render animation đứng yên
	if (idling) {
		commonFunc::renderAnimation(tex, x, y, idlingClips[idleCounter / 18], p_camera, 0, NULL, getFlipType());
		idleCounter++;
		if (idleCounter / 18 >= IDLING_ANIMATION_FRAMES) idleCounter = 0;
	}
	else idleCounter = 0;

	// Render animation rơi
	if (falling) {
		commonFunc::renderAnimation(tex, x, y, fallingClips[fallingCounter / 12], p_camera, 0, NULL, getFlipType());
		fallingCounter++;
		if (fallingCounter / 12 >= FALLING_ANIMATION_FRAMES) fallingCounter = 0;
	}
	else fallingCounter = 0;

	// Render animation tấn công
	if (attacking) {
		commonFunc::renderAnimation(tex, x, y, attackingClips[attackingCounter / 21], p_camera, 0, NULL, getFlipType());
		attackingCounter++;
		if (attackingCounter / 21 >= ATTACKING_ANIMATION_FRAMES) attackingCounter = 0;
	}
	else attackingCounter = 0;

	// Render animation bị đánh
	if (beingHit) {
		commonFunc::renderAnimation(tex, x, y, beingHitClips[beingHitCounter / 7], p_camera, 0, NULL, getFlipType());
		beingHitCounter++;
	}
	else beingHitCounter = 0;
}
