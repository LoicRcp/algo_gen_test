//
// Created by loicr on 21/10/2023.
//

#ifndef SFML_ALGOGEN_TEST_BRAIN_H
#define SFML_ALGOGEN_TEST_BRAIN_H
#include <SFML/System/Vector2.hpp>
#include <list>
#include <random>
#include <utility>
#include "agent.h"

extern const int AGENT_BRAIN_SIZE;
class Brain {
private:
    int step = 0;
    std::vector<sf::Vector2f> directions;


public:
    Brain();
    Brain(std::vector<sf::Vector2f> directions) : directions(std::move(directions)), step(0) {}
    void randomize();
    sf::Vector2f getDirection();

    int getStep() const;

    void setStep(int step);

    const std::vector<sf::Vector2f> &getDirections() const;

    int mutate();
};

Brain::Brain() {
    randomize();
}

const std::vector<sf::Vector2f> &Brain::getDirections() const {
    return directions;
}

void Brain::randomize() {
    for (int i = 0; i < AGENT_BRAIN_SIZE; i++) {
        float randomAngle = static_cast<float>(rand() % 360);
        sf::Vector2f randomDirection = sf::Vector2f(cos(randomAngle), sin(randomAngle));
        directions.push_back(randomDirection);
    }
}

sf::Vector2f Brain::getDirection() {

    sf::Vector2f direction = Brain::directions[Brain::step];
    Brain::step = Brain::step + 1;
    return direction;
}


int Brain::getStep() const {
    return step;
}

void Brain::setStep(int step) {
    Brain::step = step;
}

int Brain::mutate() {


    float mutationRate = 0.01;
    int mutation = 0;
    for (int i = 0; i < directions.size(); i++) {
        float random = static_cast<float>(rand() % 100) / 100;
        if (random < mutationRate) {
            float randomAngle = static_cast<float>(rand() % 360);
            sf::Vector2f randomDirection = sf::Vector2f(cos(randomAngle), sin(randomAngle));
            directions[i] = randomDirection;
            mutation += 1;

        }
    }
    return mutation;
}


#endif //SFML_ALGOGEN_TEST_BRAIN_H
