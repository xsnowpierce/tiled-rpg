#include "pch.h"
#include "Player.h"
#include "Scene.h"

Player::Player(sf::Texture &texture, Scene* scene) :
	playerSprite(texture), collisionBox({}, {15, 8}), sceneRef(scene)
{

	playerSprite.setTextureRect(sf::IntRect({ 1, 6 }, { 15, 22 }));
	playerSprite.setOrigin({0, 11});
	playerSprite.setPosition({});

	
	std::vector<sf::IntRect> idleUpAnimationFrames;
	idleUpAnimationFrames.push_back(sf::IntRect({ 0, 69 }, { 15, 23 }));
	addAnimation(PlayerAnimationState::IDLE_UP, idleUpAnimationFrames, 200.f);

	std::vector<sf::IntRect> idleDownAnimationFrames;
	idleDownAnimationFrames.push_back(sf::IntRect({ 1,6 }, { 15, 22 }));
	addAnimation(PlayerAnimationState::IDLE_DOWN, idleDownAnimationFrames, 200.f);

	std::vector<sf::IntRect> idleLeftAnimationFrames;
	idleLeftAnimationFrames.push_back(sf::IntRect({ 0, 102 }, { 15, 22 }));
	addAnimation(PlayerAnimationState::IDLE_LEFT, idleLeftAnimationFrames, 200.f);

	std::vector<sf::IntRect> idleRightAnimationFrames;
	idleRightAnimationFrames.push_back(sf::IntRect({ 1, 38 }, { 15, 22 }));
	addAnimation(PlayerAnimationState::IDLE_RIGHT, idleRightAnimationFrames, 200.f);

	std::vector<sf::IntRect> walkDownAnimationFrames;
	walkDownAnimationFrames.push_back(sf::IntRect({1,6}, {15, 22}));
	walkDownAnimationFrames.push_back(sf::IntRect({17,6}, {15, 22}));
	walkDownAnimationFrames.push_back(sf::IntRect({33,6}, {15, 22}));
	walkDownAnimationFrames.push_back(sf::IntRect({49,6}, {15, 22}));
	addAnimation(PlayerAnimationState::WALK_DOWN, walkDownAnimationFrames, 200.f);

	std::vector<sf::IntRect> walkUpAnimationFrames;
	walkUpAnimationFrames.push_back(sf::IntRect({ 0, 69}, { 15, 23 }));
	walkUpAnimationFrames.push_back(sf::IntRect({ 16, 69}, { 15, 23 }));
	walkUpAnimationFrames.push_back(sf::IntRect({ 32, 69}, { 15, 23 }));
	walkUpAnimationFrames.push_back(sf::IntRect({ 48, 69}, { 15, 23 }));
	addAnimation(PlayerAnimationState::WALK_UP, walkUpAnimationFrames, 200.f);

	std::vector<sf::IntRect> walkLeftAnimationFrames;
	walkLeftAnimationFrames.push_back(sf::IntRect({ 0, 102 }, { 15, 22 }));
	walkLeftAnimationFrames.push_back(sf::IntRect({ 16, 102 }, { 15, 22 }));
	walkLeftAnimationFrames.push_back(sf::IntRect({ 32, 102 }, { 15, 22 }));
	walkLeftAnimationFrames.push_back(sf::IntRect({ 48, 102 }, { 15, 22 }));
	addAnimation(PlayerAnimationState::WALK_LEFT, walkLeftAnimationFrames, 200.f);

	std::vector<sf::IntRect> walkRightAnimationFrames;
	walkRightAnimationFrames.push_back(sf::IntRect({ 1, 38 }, { 15, 22 }));
	walkRightAnimationFrames.push_back(sf::IntRect({ 17, 38 }, { 15, 22 }));
	walkRightAnimationFrames.push_back(sf::IntRect({ 33, 38 }, { 15, 22 }));
	walkRightAnimationFrames.push_back(sf::IntRect({ 49, 38 }, { 15, 22 }));
	addAnimation(PlayerAnimationState::WALK_RIGHT, walkRightAnimationFrames, 200.f);
}

Player::~Player()
{
}

void Player::update(float deltaTime)
{
	if(!sceneRef->isLoadingNewScreen())
		this->query_inputs(deltaTime);
	this->animateSprite(deltaTime);
}

void Player::render(sf::RenderTarget& target)
{
	playerSprite.setPosition(collisionBox.getPosition() - collisionBox.getHalfSize());
	target.draw(playerSprite);
	//collisionBox.render(target);
}

void Player::checkCollisions(std::vector<AABB> tiles)
{
	for (AABB tile : tiles) {
		tile.checkCollision(collisionBox, 1.0f);
	}
}

bool Player::checkCollision(AABB tile)
{
	return tile.checkCollision(collisionBox, 1.0f);
}

sf::Vector2i Player::getPlayerChunk()
{
	sf::Vector2i playerChunk({ (int)std::floor(playerSprite.getPosition().x / GameSettings::screenWidth) , (int)std::floor(playerSprite.getPosition().y / GameSettings::screenHeight) });
	playerChunk.x *= 15;
	playerChunk.y *= 10;
	return playerChunk;
}


void Player::query_inputs(float deltaTime)
{
	playerMoveDelta = sf::Vector2f({});

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Left))
		playerMoveDelta.x--;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Right))
		playerMoveDelta.x++;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up))
		playerMoveDelta.y--;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down))
		playerMoveDelta.y++;

	if (playerMoveDelta.length() != 0.f) {
		playerMoveDelta = playerMoveDelta.normalized() * playerMoveSpeed;
	}
	
	this->collisionBox.moveBox(playerMoveDelta * deltaTime);
}

void Player::animateSprite(float deltaTime)
{
	updateAnimationState();

	if (currentState != stateLastFrame) {
		currentAnimationTimer = 0.0f;
	}

	stateLastFrame = currentState;

	SpriteAnimation& anim = animations.at(currentState);

	if (anim.spriteFrames.empty()) return;

	currentAnimationTimer -= deltaTime;

	if (currentAnimationTimer <= 0.f) {
		currentAnimationTimer = anim.animationSpeed / 1000.f;
		currentAnimationFrame++;

		if (currentAnimationFrame >= anim.spriteFrames.size()) {
			currentAnimationFrame = 0;
		}

		playerSprite.setTextureRect(anim.spriteFrames[currentAnimationFrame]);
	}
}


void Player::updateAnimationState() {
	if (playerMoveDelta.length() == 0.f) {
		switch (lastDirection) {
			case PlayerAnimationDirection::UP:    
				currentState = PlayerAnimationState::IDLE_UP; 
				break;
			case PlayerAnimationDirection::DOWN:
				currentState = PlayerAnimationState::IDLE_DOWN;
				break;
			case PlayerAnimationDirection::LEFT:
				currentState = PlayerAnimationState::IDLE_LEFT;
				break;
			case PlayerAnimationDirection::RIGHT:
				currentState = PlayerAnimationState::IDLE_RIGHT;
				break;
		}
	}
	else {
		if (std::abs(playerMoveDelta.x) > std::abs(playerMoveDelta.y)) {
			lastDirection = playerMoveDelta.x > 0 ? PlayerAnimationDirection::RIGHT : PlayerAnimationDirection::LEFT;
		}
		else {
			lastDirection = playerMoveDelta.y > 0 ? PlayerAnimationDirection::DOWN : PlayerAnimationDirection::UP;
		}

		switch (lastDirection) {
			case PlayerAnimationDirection::UP:
				currentState = PlayerAnimationState::WALK_UP;
				break;
			case PlayerAnimationDirection::DOWN:
				currentState = PlayerAnimationState::WALK_DOWN;
				break;
			case PlayerAnimationDirection::LEFT:
				currentState = PlayerAnimationState::WALK_LEFT;
				break;
			case PlayerAnimationDirection::RIGHT:
				currentState = PlayerAnimationState::WALK_RIGHT;
				break;
		}
	}
}

void Player::addAnimation(PlayerAnimationState state, std::vector<sf::IntRect> frames, float speed)
{
	animations.insert({ state, SpriteAnimation(frames, speed) });
}

