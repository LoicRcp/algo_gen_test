#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <random>
#include "agent.h"
#include <ctime>
#include <SFML/System/Clock.hpp>
#include <list>
#include "population.h"
#include "obstacle.h"

extern const int WIDTH = 1000;
extern const int HEIGHT = 1000;
extern const int AGENT_BRAIN_SIZE = 1000;
#define OBJ_NUMBER 3000
#define FRAME_RATE 60

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Algo Gen Project");


    //square shape with only border
    sf::RectangleShape border(sf::Vector2f(WIDTH-34, HEIGHT-34));
    border.setFillColor(sf::Color::Transparent);
    border.setOutlineThickness(2);
    border.setOutlineColor(sf::Color::Red);
    border.setPosition(20, 20);


    sf::Vector2f goal = sf::Vector2f(WIDTH/2.0, 40);
    sf::Vector2f startingPoint = sf::Vector2f(WIDTH/2.0, HEIGHT-40);

    sf::CircleShape goalShape(10);
    goalShape.setOrigin(5, 5);
    goalShape.setFillColor(sf::Color::Green);
    goalShape.setPosition(goal);

    //list of obstacles

    std::list<Obstacle*> obstacles;
    obstacles.emplace_back(new RectangleObstacle(sf::Vector2f(20, 200), sf::Vector2f(600, 10)));
    obstacles.emplace_back(new RectangleObstacle(sf::Vector2f(200, 600), sf::Vector2f(600, 10)));


    Population population(OBJ_NUMBER, startingPoint);
    bool clearNext = false;

    sf::RectangleShape fadeRect(sf::Vector2f(window.getSize().x, window.getSize().y));
    fadeRect.setFillColor(sf::Color(255, 255, 255, 200));

    bool pause = true;

    while (window.isOpen()) {


        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)) {
                pause = !pause;
            }
        }
        if (pause) continue;
        window.draw(fadeRect);

        window.draw(goalShape);
        window.draw(border);


        if (population.allDead()) {
            population.calculateFitness(goal);
            population.naturalSelection();
            population.mutate();
        } else {
            population.update(goal, obstacles);
            population.show(&window);
        }

        for (Obstacle* obstacle : obstacles) {
            obstacle->show(&window);
        }


        window.display();
    }

    return 0;
}
