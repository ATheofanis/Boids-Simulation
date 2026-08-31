
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"

#include "Parameters.h"

class Boid
{
private:
    // Position
    Vector2 position;

    // Velocity
    Vector2 velocity;

    float radius = BoidRadius;
    float maxSpeed = MaxBoidSpeed;

    Color color = BoidColor;


    bool canDetectBoid(Vector2 otherBoidPosition)
    {
        Vector2 vectorToBoid = Vector2Subtract(otherBoidPosition, position);

        float diffAngle = Vector2Angle(vectorToBoid, velocity);

        diffAngle = fabsf(RAD2DEG * diffAngle);

        if (diffAngle > MaxDetectionAngle) return false;


        return true;
    }

public:
    // Constructors
    Boid()
    {
        position = Vector2{ WIDTH/2.0f, HEIGHT/2.0f };
        velocity = Vector2{0, 0};
    }

    Boid(Vector2 pos, Vector2 vel, float r, float maxSp, Color c) : position(pos), velocity(vel), radius(r), maxSpeed(maxSp), color(c) {};

    // Function to draw a boid
    void drawBoid() const
    {
        DrawCircle(position.x, position.y, radius, color);
    }


    void applyBoidAlgorithmRules(const std::vector<Boid>& boids)
    {
        Vector2 centerOfMass = Vector2 {0.0f, 0.0f};
        Vector2 separationForce = Vector2 {0.0f, 0.0f};
        Vector2 averageVelocity = Vector2 {0.0f, 0.0f};

        int cohesionRangeCount = 0;
        int separationRangeCount = 0;
        int alignmentRangeCount = 0;

        for (const auto& otherBoid : boids)
        {

            float distance = Vector2Distance(otherBoid.position, position);

            if (distance == 0) continue;

            // Rule 2: Separation
            if (distance <= BoidSeperationRadius)
            {
                separationRangeCount++;

                Vector2 oppositeForceExerted = Vector2Subtract(position, otherBoid.position);

                oppositeForceExerted = Vector2Scale(oppositeForceExerted, 1.0f / distance);

                separationForce = Vector2Add(separationForce, oppositeForceExerted);
            }

            // The detection angle of a boid can be limited to only detect boids in the direction it is heading (for more realistic simulations)
            if (canDetectBoid(otherBoid.position) == false) continue;

            // Rule 1: Coherence
            if (distance <= BoidCoherenceRadius)
            {
                centerOfMass = Vector2Add(centerOfMass, otherBoid.position);
                cohesionRangeCount++;
            }

            // Rule 3: Alignment
            if (distance <= BoidAlignmentRadius)
            {
                alignmentRangeCount++;

                averageVelocity = Vector2Add(averageVelocity, otherBoid.velocity);
            }
        }

        // Apply cohesion force if atleast one other boid is in range. Same for separation and alignment forces
        if (cohesionRangeCount > 0)
        {
            centerOfMass = centerOfMass / cohesionRangeCount;

            Vector2 cohesionOffset = (Vector2Subtract(centerOfMass, position)/ CoherenceStrength);

            velocity = Vector2Add(velocity, cohesionOffset);
        }

        if (separationRangeCount > 0)
        {
            Vector2 separationOffset = Vector2Scale(separationForce, SeparationStrength);

            velocity = Vector2Add(velocity, separationOffset);
        }

        if (alignmentRangeCount > 0)
        {
            averageVelocity = averageVelocity / alignmentRangeCount;

            Vector2 alignmentOffset = Vector2Subtract(averageVelocity, velocity);

            velocity = Vector2Add(velocity, Vector2Scale(alignmentOffset, AlignmentStrength));
        }
    }



    void updateBoidPosition(float dt, const std::vector<Boid>& boids)
    {
        applyBoidAlgorithmRules(boids);

        float currentSpeed = Vector2Length(velocity);

        if (currentSpeed > maxSpeed)
        {
            velocity = Vector2Scale(Vector2Normalize(velocity), maxSpeed);
        }
        else if (currentSpeed < MinBoidSpeed)
        {
            velocity = Vector2Scale(Vector2Normalize(velocity), MinBoidSpeed);
        }


        // BOUNDS CHECK, velocity change happens off screen to make the simulation more natural, no sharp changes of direction in sight
        if (position.x < BoidBoundLimits)
        {
            velocity.x += BoidTurnRate;
        }
        else if (position.x > WIDTH - BoidBoundLimits)
        {
            velocity.x += -BoidTurnRate;
        }

        if (position.y < BoidBoundLimits)
        {
            velocity.y += BoidTurnRate;
        }
        else if (position.y > HEIGHT - BoidBoundLimits)
        {
            velocity.y += -BoidTurnRate;
        }
        // End of bounds check

        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
    }

};


void drawBoids(const std::vector<Boid>& boids)
{
    for (const auto& boid : boids)
    {
        boid.drawBoid();
    }
}



int main()
{
    srand(time(nullptr));

    InitWindow(WIDTH, HEIGHT, "Boids");

    SetTargetFPS(60);

    std::vector<Boid> boids;

    for (int i = 0; i < BoidCount; i++)
    {
        float boidMaxSp = MaxBoidSpeed + (float)(rand() % (int)(MaxBoidSpeedRange));

        std::cout << boidMaxSp << std::endl;
        boids.emplace_back(Boid(Vector2{(float)(rand() % (WIDTH - (int)BoidRadius)), (float)(rand() % HEIGHT)},              // Random position
                                Vector2{(float)(((rand() % 2 == 0 ? 1 : -1) * (rand() + (int)MinBoidSpeed)) % (int)boidMaxSp),         (float)(((rand() % 2 == 0 ? 1 : -1) * (rand() + (int)MinBoidSpeed)) % (int)boidMaxSp)},   // Random velocity
                                BoidRadius,
                                boidMaxSp,
                                BoidColor)
                          );
    }

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();


        BeginDrawing();
        ClearBackground(sky);

        for (auto& boid : boids)
        {
            boid.updateBoidPosition(dt, boids);
        }

        drawBoids(boids);

        EndDrawing();
    }

}