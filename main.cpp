#include <iostream>
#include <SFML/Graphics.hpp>
#include "AgentProcessor.h"

const int WIDTH = 500;
const int HEIGHT = 500;

int main() {
    //*****************************************
    //* Initialization                        *
    //*****************************************
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Slime Mold");
    window.setFramerateLimit(60);

    sf::Texture texture;
    texture.create(WIDTH, HEIGHT);

    sf::Sprite sprite;

    AgentProcessor ap(WIDTH, HEIGHT);


    //*****************************************
    //* Loop                                  *
    //*****************************************
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // VISUAL STUFF
        ap.processAllAgents();

        texture.loadFromImage(ap.getImage());

        sprite.setTexture(texture);

        window.draw(sprite);

        window.display();
    }


    return 0;
}
