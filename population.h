//
// Created by loicr on 21/10/2023.
//

#ifndef SFML_ALGOGEN_TEST_POPULATION_H
#define SFML_ALGOGEN_TEST_POPULATION_H


#include <list>
#include "agent.h"
#include <random>
#include "obstacle.h"

class Population {
    std::list<Agent*> agents;
    sf::Vector2f startPosition;

public:
    Population(int size, sf::Vector2f startPosition);
    void show(sf::RenderWindow* window);
    void calculateFitness(sf::Vector2f goal);
    bool allDead();
    ~Population();

    void naturalSelection();

    Agent::Genes selectParent(std::list<Agent::Genes>& genesList);

    Agent *getBestAgent();

    void mutate();

    void update(sf::Vector2f goal, std::list<Obstacle *> obstacles);
};

Population::Population(int size, sf::Vector2f startPosition) {
    for (int i = 0; i < size; i++) {
        agents.push_back(new Agent(i, startPosition));
    }
    Population::startPosition = startPosition;
}

void Population::show(sf::RenderWindow *window) {
    for (Agent* agent : agents) {
        agent->display(window);
    }
}

void Population::update(sf::Vector2f goal, std::list<Obstacle*> obstacles) {
    for (Agent* agent : agents) {
        agent->update(goal, obstacles);
    }
}

void Population::calculateFitness(sf::Vector2f goal) {
    for (Agent* agent : agents) {
        agent->calculateFitness(goal);
    }
}

bool Population::allDead() {
    for (Agent* agent : agents) {
        if (!agent->isDead() && !agent->isWon()) {
            return false;
        }
    }
    return true;
}

Agent::Genes brainCrossover(Agent::Genes parent1, Agent::Genes parent2, int id) {
    Agent::Genes childGenes;
    childGenes.id = id;
    std::vector<sf::Vector2f> directions;

    std::vector<sf::Vector2f> parent1Directions = parent1.brain->getDirections();
    std::vector<sf::Vector2f> parent2Directions = parent2.brain->getDirections();

    for (int i = 0; i < AGENT_BRAIN_SIZE; i++) {
       directions.push_back(sf::Vector2f((parent1Directions[i].x + parent2Directions[i].x)/2,
                                         (parent1Directions[i].y + parent2Directions[i].y)/2));
    }

    childGenes.brain = new Brain(directions);

    sf::Color parent1Color = parent1.color;
    sf::Color parent2Color = parent2.color;

    childGenes.color = sf::Color((parent1Color.r + parent2Color.r)/2,
                                 (parent1Color.g + parent2Color.g)/2,
                                 (parent1Color.b + parent2Color.b)/2);

    return childGenes;
}

Agent* Population::getBestAgent() {
    Agent* bestAgent = agents.front();
    for (Agent* agent : agents) {
        if (agent->getFitness() > bestAgent->getFitness()) {
            bestAgent = agent;
        }
    }
    return bestAgent;
}

void Population::naturalSelection(){
    std::list<Agent*> newAgents;
    std::list<Agent::Genes> genesList;

    Agent::Genes bestAgentGenes = getBestAgent()->getGenes();
    Agent::Genes newBestAgentGenes = brainCrossover(bestAgentGenes, bestAgentGenes, bestAgentGenes.id);
    newAgents.push_back(
            new Agent(startPosition, newBestAgentGenes)
            );
    newAgents.back()->setIsBest(true);

    int id = 0;
    for (Agent* agent : agents) {
        if (agent->getId() == newBestAgentGenes.id){
            id++;
            continue;
        }
        genesList = {};
        for (Agent* pAgent : agents) {
            genesList.push_back(pAgent->getGenes());
        }

        Agent::Genes parent1Genes = selectParent(genesList);
        int idToRemove = parent1Genes.id;
        genesList.remove_if([idToRemove](const Agent::Genes& genes) { return genes.id == idToRemove; });

        if (genesList.empty()){
            throw std::runtime_error("Population of one: Invalid");
        }

        Agent::Genes parent2Genes = selectParent(genesList);
        //do cross-overs
        Agent::Genes childGenes = brainCrossover(parent1Genes, parent2Genes, id);

        newAgents.push_back(new Agent(startPosition, childGenes));

        id++;
    }

    for (Agent* agent : agents) {
        delete agent;
    }
    agents = newAgents;
}

void Population::mutate() {
    for (Agent* agent : agents) {
        agent->mutate();
    }
}


Agent::Genes Population::selectParent(std::list<Agent::Genes>& genesList) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    float fitnessSum = 0;
    for (Agent::Genes genes : genesList) {
        fitnessSum += genes.fitness;
    }

    std::uniform_real_distribution<float> dist(0.0, fitnessSum);
    float random = dist(gen);
    float selection = 0;

    for (Agent::Genes genes : genesList) {
        selection += genes.fitness;
        if (selection > random) {
            return genes;
        }
    }

    // raise exception if no parent found
    throw std::runtime_error("No parent found");
}

Population::~Population() {
    for (Agent* agent : agents) {
        delete agent;
    }

}


#endif //SFML_ALGOGEN_TEST_POPULATION_H
