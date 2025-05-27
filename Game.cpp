#include "pch.h"
#include "Game.h"
#include "GameSettings.h"

sf::View Game::getLetterboxView(sf::View view, sf::Vector2f windowSize)
{
    // Compares the aspect ratio of the window to the aspect ratio of the view,
    // and sets the view's viewport accordingly in order to achieve a letterbox effect.
    // A new view (with a new viewport set) is returned.

    float windowRatio = (float)windowSize.x / (float)windowSize.y;
    float viewRatio = view.getSize().x / (float)view.getSize().y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;

    bool horizontalSpacing = true;
    if (windowRatio < viewRatio)
        horizontalSpacing = false;

    // If horizontalSpacing is true, the black bars will appear on the left and right side.
    // Otherwise, the black bars will appear on the top and bottom.

    if (horizontalSpacing) {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    }

    else {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }

    view.setViewport(sf::FloatRect({ posX, posY }, { sizeX, sizeY }));

    return view;
}

void Game::initWindow()
{
    this->window.create(sf::VideoMode(sf::Vector2u(GameSettings::screenWidth, GameSettings::screenHeight)), GameSettings::screenName, sf::Style::Resize + sf::Style::Close);
    this->view = sf::View();
    view.setSize(sf::Vector2((float)GameSettings::screenWidth, (float)GameSettings::screenHeight));
    view.setCenter(sf::Vector2(view.getSize().x / 2, view.getSize().y / 2));
    window.setVerticalSyncEnabled(GameSettings::ENABLE_VSYNC);
    if(GameSettings::MAX_FPS != -1)
        this->window.setFramerateLimit(GameSettings::MAX_FPS);
}

void Game::initScene()
{
    this->currentScene = new Scene(&view);
}

Game::Game()
{
    this->initWindow();
    this->initScene();
}

Game::~Game()
{
    delete this->currentScene;
}

void Game::updateScene(float deltaTime)
{
    this->currentScene->update(deltaTime);
}

void Game::update(float deltaTime)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();

        // catch the resize events
        if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            view = this->getLetterboxView(view, { (float) resized->size.x, (float) resized->size.y });
        }
    }
    
    this->updateScene(deltaTime);
}

void Game::renderScene()
{
    this->currentScene->render(this->window);
}

void Game::render()
{
    this->window.clear();
    this->window.setView(view);
    // render game
    this->renderScene();

    this->window.display();
}

const sf::RenderWindow& Game::getWindow() const
{
    return this->window;
}