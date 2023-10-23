//
// Created by loicr on 23/10/2023.
//

#ifndef SFML_ALGOGEN_TEST_OBSTACLE_H
#define SFML_ALGOGEN_TEST_OBSTACLE_H


#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include "math.h"
class Obstacle {

private:
    sf::Vector2f position;

public:
    Obstacle(sf::Vector2f position);
    const sf::Vector2f &getPosition() const;
    void setPosition(const sf::Vector2f &position);

    virtual void show(sf::RenderWindow* window) = 0;
    virtual bool collide(sf::Vector2f& position) = 0;
};

Obstacle::Obstacle(sf::Vector2f position) {
    Obstacle::position = position;
}

const sf::Vector2f &Obstacle::getPosition() const {
    return position;
}

void Obstacle::setPosition(const sf::Vector2f &position) {
    Obstacle::position = position;
}

class CircleObstacle : public Obstacle {
private:
    float radius;
public:
    CircleObstacle(sf::Vector2f position, float radius) : Obstacle(position), radius(radius) {}
    void show(sf::RenderWindow* window) override;

    bool collide(sf::Vector2f& position);
};

void CircleObstacle::show(sf::RenderWindow *window) {
    sf::CircleShape circle(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(getPosition());
    circle.setFillColor(sf::Color::Red);
    window->draw(circle);
}

bool CircleObstacle::collide(sf::Vector2f& position) {
    return sqrt(pow(position.x - getPosition().x, 2) + pow(position.y - getPosition().y, 2)) <= radius;
}

class RectangleObstacle : public Obstacle {
private:
    sf::Vector2f size;
public:
    RectangleObstacle(sf::Vector2f position, sf::Vector2f size) : Obstacle(position), size(size) {}
    void show(sf::RenderWindow* window) override;

    bool collide(sf::Vector2f& position);
};

void RectangleObstacle::show(sf::RenderWindow *window) {
    sf::RectangleShape rectangle(size);
    rectangle.setPosition(getPosition());
    rectangle.setFillColor(sf::Color::Red);
    window->draw(rectangle);
}

bool RectangleObstacle::collide(sf::Vector2f& position) {
    return position.x >= getPosition().x && position.x <= getPosition().x + size.x && position.y >= getPosition().y && position.y <= getPosition().y + size.y;
}




#endif //SFML_ALGOGEN_TEST_OBSTACLE_H
