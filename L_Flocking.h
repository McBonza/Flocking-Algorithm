#pragma once
#include "BehaviorNode.h"
#include "Misc/NiceTypes.h"

class L_Flocking : public BaseNode<L_Flocking>
{
protected:
    virtual void on_enter() override;
    virtual void on_update(float dt) override;

    void FlockingAllignment();
    void FlockingCohesion();
    void FlockingSeparation();
    void FlockingWandering(float dtAccumulator);

private:
    Vec3 targetDir;

    float dtAccumulator;

    float alignmentRange = 7.5f;
    float cohesionRange = 11.25f;
    float separationRange = 7.5f;
};