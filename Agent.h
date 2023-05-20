
#ifndef SLIME_MOLD_AGENT_H
#define SLIME_MOLD_AGENT_H

#include <SFML/Graphics.hpp>

struct Agent {
    sf::Vector2i pos{0, 0};
    float angle = 0;

    Agent(const sf::Vector2i &pos, float angle) : pos(pos), angle(angle) {}
};

#endif //SLIME_MOLD_AGENT_H
