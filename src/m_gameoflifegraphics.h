#ifndef M_GAMEOFLIFEGRAPHICS_H
#define M_GAMEOFLIFEGRAPHICS_H

#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>
// #include <SFML/Keyboard.hpp>

namespace GameOfLife {

namespace {
    const std::string TITLE = "Game Of Life";
    const int WIDTH = 1600, HEIGHT = 1000;
}

template <typename World>
class Graphics {
  public:
    using WorldAccessor = std::function<void(World*, bool)>;
    using Initialisation = std::function<void(void)>;
    using Stoping = std::function<void(void)>;

  private:
    WorldAccessor       d_getWorld;
    Initialisation      d_initWorld;
    Stoping             d_stopWorld;
    World               d_worldView;
    sf::ContextSettings d_settings;
    sf::RenderWindow    d_window;
    sf::Font            d_font;
    int                 d_tick;
    int                 d_speed;
    bool                d_showStates;

    void updateDisplay();

    void handleWindowClosing();

  public:
    Graphics(WorldAccessor& getWorld,
            Initialisation& initWorld,
            Stoping& stopWorld,
            const World& world);

    void run();
};

template<typename World>
Graphics<World>::Graphics(WorldAccessor& getWorld,
        Initialisation& initWorld,
        Stoping& stopWorld,
        const World& world)
: d_getWorld(getWorld)
, d_initWorld(initWorld)
, d_stopWorld(stopWorld)
, d_worldView(world)
, d_settings()
, d_window(
        sf::VideoMode(WIDTH, HEIGHT),
        TITLE,
        sf::Style::Resize | sf::Style::Close,
        d_settings)
, d_tick(0)
, d_speed(0)
, d_showStates(true)
{
    d_settings.antialiasingLevel = 8;
    d_window.setVerticalSyncEnabled(true);
    if (!d_font.loadFromFile("font/arial.ttf")) {
        std::cout << "Error loading font\n";
    }
    updateDisplay();
}

template<typename World>
void Graphics<World>::run()
{
	while (d_window.isOpen()) {
        d_tick++;
		sf::Event event;
		while (d_window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
                handleWindowClosing();
            }
			if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space) {
                    std::cout << "the space key was released\n";
                    d_speed = d_speed == 0 ? 4 : d_speed * 4;
                    d_speed = d_speed >= 256 ? 256 : d_speed;
                    std::cout << "speed=" << d_speed << std::endl;
                }
                if (event.key.code == sf::Keyboard::RControl
                    || event.key.code == sf::Keyboard::LControl) {
                    std::cout << "the control key was released\n";
                    d_speed = d_speed == 4 ? 0 : d_speed / 4;
                    std::cout << "speed=" << d_speed << std::endl;
                }
                if (event.key.code == sf::Keyboard::Key::Z
                    || event.key.code == sf::Keyboard::Key::P) {
                    std::cout << "the Z/P key was released\n";
                    d_speed = 0;
                    std::cout << "speed=" << d_speed << std::endl;
                }
                if (event.key.code == sf::Keyboard::Key::I) {
                    std::cout << "the I key was released\n";
                    d_initWorld();
                    d_tick = 60;
                    d_speed = 1;
                    updateDisplay();
                    d_speed = 0;
                    std::cout << "speed=" << d_speed << std::endl;
                }
                if (event.key.code == sf::Keyboard::Key::S) {
                    std::cout << "the S key was released\n";
                    d_showStates = !d_showStates;
                    std::cout << "showStates=" << d_showStates << std::endl;
                }
            }
        }
        updateDisplay();
    }
}

template<typename World>
void Graphics<World>::updateDisplay()
{
    int height = HEIGHT / d_worldView.size();
    const sf::Color reddish(sf::Color(255, 32, 79));
    sf::CircleShape alive(height);
    alive.setFillColor(reddish);
    sf::CircleShape alive2(height * 2 / 3);
    alive2.setFillColor(reddish);
    sf::CircleShape alive3(height / 2);
    alive3.setFillColor(reddish);
    sf::CircleShape alive4(height / 3);
    alive4.setFillColor(reddish);

    auto place = [&] (sf::CircleShape& cell, int x, int y, int width)
    {
        cell.setPosition(x * width, y * height);
        float correction = height - cell.getRadius();
        cell.move(correction, correction);
        d_window.draw(cell);
    };

    bool updateWorld = false;
    if (d_speed > 0 && d_tick >= 60 / d_speed) {
        updateWorld = true;
        d_tick = 0;
        d_getWorld(&d_worldView, updateWorld);
    }

    d_window.clear(sf::Color::Black);
    for (int y = 0; y < d_worldView.size(); ++y) {
        const auto& line = d_worldView[y];
        int width = WIDTH / line.size();
        for (int x = 0; x < line.size(); ++x) {
            if (line[x] == 0) continue;
            else if (!d_showStates) {
                if (line[x] > 0) place(alive, x, y, width);
            }
            else if (line[x] == 1) place(alive, x, y, width);
            else if (line[x] == 2) place(alive2, x, y, width);
            else if (line[x] == 3) place(alive3, x, y, width);
            else if (line[x] == 4) place(alive4, x, y, width);
            else if (line[x] == -1) place(alive4, x, y, width);
            else if (line[x] == -2) place(alive3, x, y, width);
            else if (line[x] == -3) place(alive2, x, y, width);
        }
    }

    sf::Text speedTxt;
    speedTxt.setFont(d_font);
    std::string text( 
            "Show states (s) = " + std::string(d_showStates ? "true" : "false")
            + "\nSpeed (space/ctrl/z) = " + std::to_string(d_speed)
            + "\nInitialise (i)");
    speedTxt.setString(text);

    speedTxt.setCharacterSize(24); // in pixels, not points!
    speedTxt.setFillColor(sf::Color(128, 249, 165));
    d_window.draw(speedTxt);

    d_window.display();
}

template<typename World>
void Graphics<World>::handleWindowClosing()
{
    d_stopWorld();
    d_window.close();
}

} // closing project namespace

#endif

