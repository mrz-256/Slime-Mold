
#ifndef SLIME_MOLD_AGENTPROCESSOR_H
#define SLIME_MOLD_AGENTPROCESSOR_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <chrono>
#include "Agent.h"

class AgentProcessor {
public:
    //*****************************************
    //* Constants                             *
    //*****************************************
    const bool WRAP_AROUND_SCREEN = true;// Other version (borders) is unimplemented

    /// The amount of agents(particles)
    const int AGENT_COUNT = 50000;
    /// This value gets added to the pixel agent was in
    const int ADD_ON_AGENT = 12;

    /// The distance of the sensory area from the agent
    const int DISTANCE_FROM_AGENT = 4;
    /// The angle from the sensor to the left and right from the agents angle(direction) (in radians)
    const float ANGLE_OF_VIEW = 0.4;

    /// screen blur
    const bool BLURRY = true;
    /// The blurring radius
    const int BLUR_RADIUS = 1;

    /// If blurring is false, it instead decrements each cell by this value
    const int DECR_VALUE = 0;
    /// If blurring is false, it also multiplies the color by this value ( probably should be in (0..1) )
    const float MULTIPLY_VALUE = 0.9f;

private:
    //*****************************************
    //* Basic variable fields                 *
    //*****************************************
    sf::Image image;
    std::vector<Agent *> agents;

    int WIDTH;
    int HEIGHT;

    int _seed;

public:
    //*****************************************
    //* Generic public methods                *
    //*****************************************
    AgentProcessor(int width, int height);

    const sf::Image &getImage() const;

    void processAllAgents();

    virtual ~AgentProcessor();

private:

    //*****************************************
    //* Graphics                              *
    //*****************************************
    void blurImage();

    void lowerHue();

    //*****************************************
    //* Random numbers                        *
    //*****************************************
    uint32_t rnd(int seed);

    //*****************************************
    //* Processing agents helper methods      *
    //*****************************************
    void processAgent(Agent *agent);

    void updateAgentWrapping(Agent *agent);


};


#endif //SLIME_MOLD_AGENTPROCESSOR_H
