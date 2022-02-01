#include <pch.h>
#include "L_Flocking.h"
#include "Agent/BehaviorAgent.h"

void L_Flocking::on_enter()
{
    BehaviorNode::on_leaf_enter(); 
    dtAccumulator = 0;

    agent->set_position(Vec3(RNG::range(0.0f, 100.0f), RNG::range(0.0f, 60.0f), RNG::range(0.0f, 100.0f)));

    agent->move_toward_point(Vec3(RNG::range(-1000.0f, 1000.0f), RNG::range(-1000.0f, 1000.0f), RNG::range(-1000.0f, 1000.0f)), 0.02f);
}

void L_Flocking::on_update(float dt)
{
    dtAccumulator += dt;

    targetDir = Vec3(0, 0, 0);

    FlockingAllignment();
    FlockingCohesion();
    FlockingSeparation();
    FlockingWandering(dtAccumulator);

    //targetDir.Normalize();

    agent->move_toward_point(agent->get_position() + targetDir, dt);

   // on_success();

    display_leaf_text();
}


void L_Flocking::FlockingAllignment() {
    // get a list of all current agents
    const auto& allAgents = agents->get_all_agents();

    // and our agent's position
    const auto& currPos = agent->get_position();

    Vec3 targetDirs = Vec3(0,0,0);
    int agentCounter = 0;

    for (const auto& a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent && a->get_type() == "Boid")
        {
            const auto& agentDir = a->get_direction();
            const float distance = Vec3::Distance(currPos, a->get_position());

            if (distance < alignmentRange)
            {
                targetDirs += agentDir;
                agentCounter++;
            }
        }
    }
    if (agentCounter > 0)
        targetDir = (targetDirs / (float)agentCounter + agent->get_direction()) / 2.0f;
    else
        targetDir = agent->get_direction();
}


void L_Flocking::FlockingCohesion() {
    // get a list of all current agents
    const auto& allAgents = agents->get_all_agents();

    // and our agent's position
    const auto& currPos = agent->get_position();

    Vec3 targetPositions = Vec3(0, 0, 0);
    int agentCounter = 0;

    for (const auto& a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent && a->get_type() == "Boid")
        {
            const auto& agentPos = a->get_position();
            const float distance = Vec3::Distance(currPos, a->get_position());

            if (distance < cohesionRange)
            {
                targetPositions += agentPos;
                agentCounter++;
            }
        }
    }
    if (agentCounter > 0)
        targetDir += (targetPositions / (float)agentCounter) - agent->get_position();
}


void L_Flocking::FlockingSeparation() {

    // get a list of all current agents
    const auto& allAgents = agents->get_all_agents();

    // and our agent's position
    const auto& currPos = agent->get_position();

    Vec3 targetDirections = Vec3(0, 0, 0);
    int agentCounter = 0;

    for (const auto& a : allAgents)
    {
        // make sure it's not our agent
        if (a != agent && a->get_type() == "Boid")
        {
            const auto& agentPos = a->get_position();
            const float distance = Vec3::Distance(currPos, a->get_position());

            if (distance < separationRange)
            {
                targetDirections += (agent->get_position() - agentPos) / (distance * distance / separationRange);
                agentCounter++;
            }
        }
    }
    if (agentCounter > 0)
        targetDir += (targetDirections);
}

void L_Flocking::FlockingWandering(float dtAccumulator) {

    Vec3 targetDirections = Vec3(cos(dtAccumulator + agent->get_movement_speed()*10.0f),
                                 sin(dtAccumulator + agent->get_movement_speed()*10.0f),
                                 cos(dtAccumulator + agent->get_movement_speed()*10.5f));
    targetDirections.Normalize();
    targetDir += (targetDirections)/4.0f;
}