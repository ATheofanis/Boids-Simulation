
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
    Vector3 position;

    // Velocity
    Vector3 velocity;

    float radius = BoidRadius;
    float maxSpeed = MaxBoidSpeed;

    float wingsLen = WingsLen;

    int flapTimer = 5 + rand() % 10;
    int flapDelay = 10 + rand() % 20;

    bool flap = false;

    Color color = BoidColor;


    bool canDetectBoid(Vector3 otherBoidPosition)
    {
        Vector3 vectorToBoid = Vector3Subtract(otherBoidPosition, position);

        float diffAngle = Vector3Angle(vectorToBoid, velocity);

        diffAngle = fabsf(RAD2DEG * diffAngle);

        return (diffAngle <= MaxDetectionAngle);
    }

public:
    // Constructors
    Boid()
    {
        position = Vector3{ WIDTH/2.0f, HEIGHT/2.0f, 0.0f };
        velocity = Vector3{0, 0, 0};
    }

    Boid(Vector3 pos, Vector3 vel, float r, float maxSp, Color c) : position(pos), velocity(vel), radius(r), maxSpeed(maxSp), color(c) {};

    // Function to draw a boid
    void drawBoid() const
    {
        float wingsLength = wingsLen;
        float frontLength = FrontLen;

        float ZFactor = -position.z / 140 + 1;

        wingsLength += ZFactor;
        frontLength += ZFactor;


        Vector2 normalizedVelocity = Vector2Normalize((Vector2){velocity.x, velocity.y});

        Vector2 perpendicularVector = Vector2{-normalizedVelocity.y, normalizedVelocity.x};

        Vector2 scaledPerpVector = Vector2Scale(perpendicularVector, wingsLength);

        Vector2 leftSide = Vector2Subtract((Vector2){position.x, position.y}, scaledPerpVector);
        Vector2 rightSide = Vector2Add((Vector2){position.x, position.y}, scaledPerpVector);


        Vector2 frontSide = (Vector2Add((Vector2){position.x, position.y}, Vector2Scale(normalizedVelocity, frontLength)));


        DrawTriangle(rightSide, frontSide, leftSide, color);
        DrawCircle(position.x, position.y, TailLen, TailColor);
    }


    void applyBoidAlgorithmRules(const std::vector<Boid>& boids)
    {
        Vector3 centerOfMass = Vector3 {0.0f, 0.0f, 0.0f};
        Vector3 separationForce = Vector3 {0.0f, 0.0f, 0.0f};
        Vector3 averageVelocity = Vector3 {0.0f, 0.0f, 0.0f};

        int cohesionRangeCount = 0;
        int separationRangeCount = 0;
        int alignmentRangeCount = 0;

        for (const auto& otherBoid : boids)
        {

            float distance = Vector3Distance(otherBoid.position, position);

            if (distance == 0) continue;

            // Rule 2: Separation
            if (distance <= BoidSeperationRadius)
            {
                separationRangeCount++;

                Vector3 oppositeForceExerted = Vector3Subtract(position, otherBoid.position);

                oppositeForceExerted = Vector3Scale(oppositeForceExerted, 1.0f / distance);

                separationForce = Vector3Add(separationForce, oppositeForceExerted);
            }

            // The detection angle of a boid can be limited to only detect boids in the direction it is heading (for more realistic simulations)
            if (canDetectBoid(otherBoid.position) == false) continue;

            // Rule 1: Coherence
            if (distance <= BoidCoherenceRadius)
            {
                centerOfMass = Vector3Add(centerOfMass, otherBoid.position);
                cohesionRangeCount++;
            }

            // Rule 3: Alignment
            if (distance <= BoidAlignmentRadius)
            {
                alignmentRangeCount++;

                averageVelocity = Vector3Add(averageVelocity, otherBoid.velocity);
            }
        }

        // Apply cohesion force if atleast one other boid is in range. Same for separation and alignment forces
        if (cohesionRangeCount > 0)
        {
            centerOfMass = centerOfMass / cohesionRangeCount;

            Vector3 cohesionOffset = (Vector3Subtract(centerOfMass, position)/ CoherenceStrength);

            velocity = Vector3Add(velocity, cohesionOffset);
        }

        if (separationRangeCount > 0)
        {
            Vector3 separationOffset = Vector3Scale(separationForce, SeparationStrength);

            velocity = Vector3Add(velocity, separationOffset);
        }

        if (alignmentRangeCount > 0)
        {
            averageVelocity = averageVelocity / alignmentRangeCount;

            Vector3 alignmentOffset = Vector3Subtract(averageVelocity, velocity);

            velocity = Vector3Add(velocity, Vector3Scale(alignmentOffset, AlignmentStrength));
        }
    }


    void flapWings()
    {
        if (flapTimer == flapDelay)
        {
            // If the wings havent flapped then flap them
            if (flap == false)
            {
                wingsLen = wingsLen * 0.7;
            }
            else
            {
                wingsLen = WingsLen;
            }
            flap = !flap;
            flapTimer = 0;
        }
    }


    void updateBoidPosition(float dt, const std::vector<Boid>& boids)
    {
        flapTimer++;

        flapWings();



        applyBoidAlgorithmRules(boids);

        float currentSpeed = Vector3Length(velocity);

        if (currentSpeed > maxSpeed)
        {
            velocity = Vector3Scale(Vector3Normalize(velocity), maxSpeed);
        }
        else if (currentSpeed < MinBoidSpeed)
        {
            velocity = Vector3Scale(Vector3Normalize(velocity), MinBoidSpeed);
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

        if (position.z < 0)
        {
            velocity.z += BoidTurnRate;
        }
        else if (position.z > DEPTH)
        {
            velocity.z -= BoidTurnRate;
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
    int wingFlapTimer = 0;

    srand(time(nullptr));

    InitWindow(WIDTH, HEIGHT, "Boids");

    Texture2D skyImage = LoadTexture("..\\Images\\BoidSky.png");

    // Info to draw sky texture
    Rectangle sourceRect = { 0.0f, 0.0f, (float)skyImage.width, (float)skyImage.height };
    Rectangle destRect = { 0.0f, 0.0f, (float)WIDTH, (float)HEIGHT };
    Vector2 origin = { 0.0f, 0.0f };

    SetTargetFPS(60);

    std::vector<Boid> boids;

    for (int i = 0; i < BoidCount; i++)
    {
        float boidMaxSp = MaxBoidSpeed + (float)(rand() % (int)(MaxBoidSpeedRange));

        //std::cout << boidMaxSp << std::endl;
        boids.emplace_back(Boid(Vector3{(float)(rand() % (WIDTH - (int)BoidRadius)), (float)(rand() % HEIGHT), (float(rand() % DEPTH))},              // Random position
                                Vector3{(float)(((rand() % 2 == 0 ? 1 : -1) * (rand() + (int)MinBoidSpeed)) % (int)boidMaxSp),
                                        (float)(((rand() % 2 == 0 ? 1 : -1) * (rand() + (int)MinBoidSpeed)) % (int)boidMaxSp),
                                        (float)(((rand() % 2 == 0 ? 1 : -1) * (rand() + (int)MinBoidSpeed)) % (int)boidMaxSp)},   // Random velocity
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

        DrawTexturePro(skyImage, sourceRect, destRect, origin, 0.0f, WHITE);

        for (auto& boid : boids)
        {
            boid.updateBoidPosition(dt, boids);
        }

        drawBoids(boids);

        EndDrawing();
    }

    UnloadTexture(skyImage);
    CloseWindow();

    return 0;

}