#include "swarm.h"
#include <stdlib.h>
#include <math.h>

// Initialize the swarm
void initialize_swarm(Agent *swarm, int numAgents, int cols, int rows)
{
  for (int i = 0; i < numAgents; i++)
  {
    // Spread out agents even more by covering the entire image space
    point_set3D(&(swarm[i].position),
                ((rand() % cols) - cols / 2) / 25 - 5,   // X range from -640 to 640
                ((rand() % rows) - rows / 2) / 25 + 3,   // Y range from -360 to 360
                ((rand() % rows) - rows / 2) / 10 - 11); // Z range from -40 to 40

    // Increase the initial velocity range to allow for more dynamic movement
    vector_set(&(swarm[i].velocity),
               (rand() % 200) / 100.0 - 1.0,
               (rand() % 200) / 100.0 - 1.0,
               (rand() % 200) / 100.0 - 1.0);

    vector_set(&(swarm[i].acceleration), 0.0, 0.0, 0.0);
    swarm[i].maxSpeed = 0.5;  // Increase max speed slightly
    swarm[i].maxForce = 0.04; // Increase max force slightly
  }
}

// Update the swarm
void update_swarm(Agent *swarm, int numAgents, float maxSpeed)
{
  for (int i = 0; i < numAgents; i++)
  {
    Agent *agent = &swarm[i];

    // Increase the speed of movement by scaling the velocity
    Vector scaledVelocity;
    vector_scale(&(agent->velocity), 4.0, &scaledVelocity); // Increase the velocity magnitude

    // Update position based on velocity
    point_add_vector(&(agent->position), &(agent->velocity), &(agent->position));

    // Apply the acceleration to velocity
    vector_add(&(agent->velocity), &(agent->acceleration), &(agent->velocity));

    // Limit the speed to maxSpeed
    if (vector_length(&(agent->velocity)) > maxSpeed)
    {
      vector_normalize(&(agent->velocity));
      vector_scale(&(agent->velocity), maxSpeed, &(agent->velocity));
    }

    // Reset acceleration for the next frame
    vector_set(&(agent->acceleration), 0.0, 0.0, 0.0);
  }
}

// Render the swarm
void render_swarm(Agent *swarm, int numAgents, Module *m, Module *figure)
{
  for (int i = 0; i < numAgents; i++)
  {
    Point center;
    point_copy(&center, &(swarm[i].position));

    // Insert the figure as an agent
    module_identity(m);
    module_scale(m, 0.4, 0.4, 0.4);
    module_translate(m, center.val[0], center.val[1], center.val[2]);
    module_module(m, figure); // insert the figure into the module
  }
}
