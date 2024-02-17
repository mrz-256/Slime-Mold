
#include "AgentProcessor.h"


//*****************************************
//* Generic public methods                *
//*****************************************
AgentProcessor::AgentProcessor(int width, int height) {
    this->WIDTH = width;
    this->HEIGHT = height;
    this->image.create(WIDTH, HEIGHT);

    // TODO: This is not cross-platform ?(works only on linux)
    this->_seed = std::chrono::duration_cast<std::chrono::milliseconds>
            (std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    std::cout << "SEED: " << _seed << "\n";

    // commented part makes agents spawn at random places
    for (int i = 0; i < AGENT_COUNT; ++i) {
        int x = WIDTH  / 2; //rnd(this->_seed++) % WIDTH;
        int y = HEIGHT / 2; //rnd(this->_seed++) % HEIGHT;
        double angle = rnd(_seed++) % 10;
        agents.push_back(new Agent(sf::Vector2i(x, y), angle));
    }
}

AgentProcessor::~AgentProcessor() {
    for (Agent *agent: agents) {
        delete agent;
    }
}

const sf::Image &AgentProcessor::getImage() const {
    return image;
}


//*****************************************
//* Graphics                              *
//*****************************************
void AgentProcessor::blurImage() {
    sf::Image blurred_image = image;

    for (unsigned int x = 0; x < WIDTH; ++x) {
        for (unsigned int y = 0; y < HEIGHT; ++y) {

            int color = 0;
            unsigned int total_pixels = 0;

            // Compute the average color within the blur radius
            for (unsigned int i = (x > BLUR_RADIUS ? x - BLUR_RADIUS : 0); (i < WIDTH && i <= x + BLUR_RADIUS); ++i) {
                for (unsigned int j = (y > BLUR_RADIUS ? y - BLUR_RADIUS : 0); (j < HEIGHT &&
                                                                                j <= y + BLUR_RADIUS); ++j) {
                    color += image.getPixel(i, j).r;
                    total_pixels++;
                }
            }

            color /= total_pixels;
            blurred_image.setPixel(x, y, sf::Color(color, color, color));
        }
    }

    // Update the original image with the blurred result
    image = blurred_image;
}

void AgentProcessor::lowerHue() {
    for (int x = 0; x < WIDTH; ++x) {
        for (int y = 0; y < HEIGHT; ++y) {
            sf::Uint8 color = image.getPixel(x, y).r;

            if (color < DECR_VALUE) {
                color = 0;
            } else {
                color -= DECR_VALUE;
            }

            color *= MULTIPLY_VALUE;

            image.setPixel(x, y, sf::Color(color, color, color));
        }
    }
}


//*****************************************
//* Random numbers                        *
//*****************************************
uint32_t AgentProcessor::rnd(int seed) {
    seed += 0xe120fc15;
    uint64_t tmp;
    tmp = (uint64_t) seed * 0x4a39b70d;
    uint32_t m1 = (tmp >> 32) ^ tmp;
    tmp = (uint64_t) m1 * 0x12fad5c9;
    uint32_t m2 = (tmp >> 32) ^ tmp;
    return m2;
}


//*****************************************
//* Processing Agents                     *
//*****************************************
void AgentProcessor::processAllAgents() {
    //printf("AGENT_0: x: %d   y: %d   angle: %f\n", agents[0]->pos.x, agents[0]->pos.y, agents[0]->angle);

    for (int i = 0; i < AGENT_COUNT; ++i) {
        //updateAgentWrapping(agents[i]);
        processAgent(agents[i]);
    }
    if (BLURRY) {
        blurImage();
    } else {
        lowerHue();
    }
}

void AgentProcessor::processAgent(Agent *agent) {
    updateAgentWrapping(agent);

    int color_value = image.getPixel(agent->pos.x, agent->pos.y).r + ADD_ON_AGENT;
    if (color_value > 255) {
        color_value = 255;
    }
    image.setPixel(agent->pos.x, agent->pos.y, sf::Color(color_value, color_value, color_value));
}

void AgentProcessor::updateAgentWrapping(Agent *agent) {
    std::vector<double> directions(3); // [0]=left [1]=middle [2]=right
    std::vector<int> values(3);

    // region data
    // get the three directions agent can look at and the values of the cells at these 3 positions
    for (int i = -1; i <= 1; ++i) {
        directions[i + 1] = agent->angle + i * ANGLE_OF_VIEW;

        // get x position and if dangerous, wrap around screen
        double new_x = agent->pos.x + DISTANCE_FROM_AGENT * std::cos(directions[i + 1]);
        if (new_x < 0) {
            new_x = WIDTH + new_x;
        } else if (new_x >= WIDTH) {
            new_x -= WIDTH;
        }

        // get y position and if dangerous, wrap around screen
        double new_y = agent->pos.y + DISTANCE_FROM_AGENT * std::sin(directions[i + 1]);
        if (new_y < 0) {
            new_y = HEIGHT + new_y;
        } else if (new_y >= HEIGHT) {
            new_y -= HEIGHT;
        }

        values[i + 1] = image.getPixel(new_x, new_y).r;
    }
    //endregion

    //region Update agent angle
    if (values[0] > values[2]) {
        agent->angle = directions[0];
    } else if (values[2] > values[0]) {
        agent->angle = directions[2];
    } else if (values[1] > values[0] and values[1] > values[2]) {
        agent->angle = directions[1];
    } else {
        agent->angle = directions[rnd(_seed++) % 3];
    }
    //endregion

    //region Update position
    float x_move = std::cos(agent->angle);
    float y_move = std::sin(agent->angle);
    agent->pos.x += (x_move < 0) ? -1 : (x_move == 0) ? 0 : 1;
    agent->pos.y += (y_move < 0) ? -1 : (y_move == 0) ? 0 : 1;

    // Safety (Wrapping around screen)
    if (agent->pos.x < 0) {
        agent->pos.x += WIDTH;
    } else if (agent->pos.x >= WIDTH) {
        agent->pos.x -= WIDTH;
    }

    if (agent->pos.y < 0) {
        agent->pos.y += HEIGHT;
    } else if (agent->pos.y >= HEIGHT) {
        agent->pos.y -= HEIGHT;
    }
    //endregion
}
