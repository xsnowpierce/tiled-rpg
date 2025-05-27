#pragma once
#include "pch.h"
#include "AABB.h"

enum class PlayerAnimationState {
	IDLE_UP, IDLE_DOWN, IDLE_LEFT, IDLE_RIGHT,
	WALK_UP, WALK_DOWN, WALK_LEFT, WALK_RIGHT
};

enum class PlayerAnimationDirection {
	UP, DOWN, LEFT, RIGHT
};

class SpriteAnimation {
public:
	SpriteAnimation(std::vector<sf::IntRect> frames, float speed) : spriteFrames(std::move(frames)), animationSpeed(speed) {}
	std::vector<sf::IntRect> spriteFrames;
	float animationSpeed;
};

class Scene;

class Player
{
public:
	Player(sf::Texture &texture, Scene* scene);
	~Player();

	void update(float deltaTime);
	void render(sf::RenderTarget &target);

	AABB &getAABB() { return collisionBox; }

	void checkCollisions(std::vector<AABB> tiles);
	bool checkCollision(AABB tile);

	sf::Vector2i getPlayerChunk();
	sf::Vector2f getPlayerPosition() { return playerSprite.getPosition(); };

private:
	sf::Sprite playerSprite;
	AABB collisionBox;
	Scene* sceneRef;

	sf::Vector2f playerMoveDelta;
	float playerMoveSpeed = 70.f;

	void query_inputs(float deltaTime);
	


	// ANIMATION
	void animateSprite(float deltaTime);
	void updateAnimationState();
	void addAnimation(PlayerAnimationState state, std::vector<sf::IntRect> frames, float speed);

	float currentAnimationTimer = 0.f;
	int currentAnimationFrame = 0;

	PlayerAnimationDirection lastDirection;
	PlayerAnimationState currentState = PlayerAnimationState::IDLE_DOWN;
	PlayerAnimationState stateLastFrame = PlayerAnimationState::IDLE_DOWN;

	std::map<PlayerAnimationState, SpriteAnimation> animations;
};

