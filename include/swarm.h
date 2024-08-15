#ifndef SWARM_H
#define SWARM_H

#include "module.h"

typedef struct {
    Point position;
    Vector velocity;
    Vector acceleration;
    float maxSpeed;
    float maxForce;
} Agent;

void initialize_swarm(Agent *swarm, int numAgents, int cols, int rows);
void update_swarm(Agent *swarm, int numAgents, float maxSpeed);
void render_swarm(Agent *swarm, int numAgents, Module *m, Module *figure);

#endif // SWARM_H
