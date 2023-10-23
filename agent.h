//
// Created by loicr on 10/03/2023.
//

#ifndef SFML_SCREAMING_INSECTS_H
#define SFML_SCREAMING_INSECTS_H
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <math.h>
#include <list>
#include "brain.h"
#include "obstacle.h"

extern const int WIDTH;
extern const int HEIGHT;


class Agent {
public:

    struct Genes {
        int id;
        Brain* brain;
        sf::Color color;  // Only if you're using color as a gene
        float fitness;
    };

private:
    int id;
    sf::Vector2f* position;
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    const int radius = 4;

    sf::CircleShape shape;
    Brain* brain;
    bool dead = false;
    float fitness = 0;
    bool won = false;
    bool isBest = false;
public:
    bool isBest1() const;

public:

    Genes getGenes() const;
    bool isDead() const;
    Agent(int id, sf::Vector2f position);
    Agent(sf::Vector2f position, Agent::Genes genes);
    void display(sf::RenderWindow* window);
    void move();
    //void border_constraint();
    void border_constraint();

    void update(sf::Vector2f goal, std::list<Obstacle*> obstacles);
    bool reachedGoal(sf::Vector2f goal);
    void calculateFitness(sf::Vector2f goal);

    bool isWon() const;
    ~Agent();
    float getFitness() const;

    void setIsBest(bool isBest);

    int getId() const;

    void mutate();
};


Agent::Agent(int id, sf::Vector2f position) {
    Agent::id = id;
    Agent::position = new sf::Vector2f(position.x, position.y);
    Agent::velocity = sf::Vector2f(0, 0);
    Agent::acceleration = sf::Vector2f(0, 0);

    Agent::brain = new Brain();

    Agent::shape = sf::CircleShape(radius);
    sf::Color randomColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
    Agent::shape.setFillColor(randomColor);
    Agent::shape.setOutlineColor(sf::Color::Black);
    Agent::shape.setOutlineThickness(1);
    Agent::shape.setPosition(position);
}

Agent::Agent(sf::Vector2f position, Agent::Genes genes) {
    Agent::id = genes.id;
    Agent::position = new sf::Vector2f(position.x, position.y);
    Agent::velocity = sf::Vector2f(0, 0);
    Agent::acceleration = sf::Vector2f(0, 0);

    Agent::brain = genes.brain;

    Agent::shape = sf::CircleShape(radius);
    Agent::shape.setFillColor(genes.color);
    Agent::shape.setPosition(position);
}

int Agent::getId() const {
    return id;
}


bool Agent::isWon() const {
    return won;
}

void Agent::move() {
    if (Agent::brain->getStep() < AGENT_BRAIN_SIZE) {
        Agent::acceleration = Agent::brain->getDirection();
    } else {
        Agent::dead = true;
    }

    Agent::velocity += Agent::acceleration;
    float magnitude = sqrt(pow(Agent::velocity.x, 2) + pow(Agent::velocity.y, 2));

    if (magnitude > 5 && magnitude != 0) {
        Agent::velocity /= magnitude;
        Agent::velocity = Agent::velocity * 5.0f;
    }

    Agent::position->x += Agent::velocity.x;
    Agent::position->y += Agent::velocity.y;
    Agent::shape.setPosition(*Agent::position);
}

void Agent::display(sf::RenderWindow *window) {
    window->draw(Agent::shape);
}

void Agent::border_constraint() {
    if (Agent::position->x > WIDTH - 20 || Agent::position->x < 20
    || Agent::position->y > HEIGHT - 20 || Agent::position->y < 20) {
        Agent::dead = true;
    }
}

bool Agent::reachedGoal(sf::Vector2f goal) {
    float distanceToGoal = sqrt(pow(Agent::position->x - goal.x, 2) + pow(Agent::position->y - goal.y, 2));
    return distanceToGoal < 10-radius/2.0;
}

void Agent::update(sf::Vector2f goal, std::list<Obstacle*> obstacles) {
    if (!Agent::dead && !Agent::won) {
        Agent::move();
        Agent::border_constraint();
        for (Obstacle* obstacle : obstacles) {
            if (obstacle->collide(*Agent::position)) {
                Agent::dead = true;
            }
        }
        if (Agent::reachedGoal(goal)) {
            Agent::won = true;
        }
    }
}

float Agent::getFitness() const {
    return fitness;
}

void Agent::calculateFitness(sf::Vector2f goal) {
    if (Agent::won){
        Agent::fitness = 1.0/16.0 + 10000.0/(pow(Agent::brain->getStep(), 2));
    } else {
        float distanceToGoal = sqrt(pow(Agent::position->x - goal.x, 2) + pow(Agent::position->y - goal.y, 2));
        Agent::fitness = 1.0 / pow(distanceToGoal, 2);
        return;
    }

}

bool Agent::isDead() const {
    return dead;
}

Agent::~Agent() {
    delete Agent::position;
    delete Agent::brain;
}

Agent::Genes Agent::getGenes() const {
    Genes genes;
    genes.id = Agent::id;
    genes.brain = Agent::brain;
    genes.color = Agent::shape.getFillColor();
    genes.fitness = Agent::fitness;
    return genes;
}

void Agent::setIsBest(bool isBest) {
    Agent::isBest = isBest;
    Agent::shape.setFillColor(sf::Color::Green);
    Agent::shape.setOutlineColor(sf::Color::Red);
    Agent::shape.setOutlineThickness(3);
}

bool Agent::isBest1() const {
    return isBest;
}

void Agent::mutate() {
    if (Agent::isBest) {
        return;
    }
    int rate = Agent::brain->mutate();
    sf::Color color = Agent::shape.getFillColor();
    color.r = color.r + rate > 255 ? color.r - rate : color.r + rate;
    color.g = color.g + rate > 255 ? color.g - rate : color.g + rate;
    color.b = color.b + rate > 255 ? color.b - rate : color.b + rate;
    Agent::shape.setFillColor(color);

    printf("Rate: %d\n", rate);


}


#endif //SFML_SCREAMING_INSECTS_H