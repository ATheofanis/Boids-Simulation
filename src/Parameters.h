//
// Created by theoa on 31/08/2026.
//

#pragma once


// =============== BORDERS ====================
constexpr int WIDTH = 1200;
constexpr int HEIGHT = 900;

constexpr Vector2 CenterOfSimulation = Vector2{(float)(WIDTH/2), (float)(HEIGHT/2)};
// =============== BORDERS ====================




inline int BoidCount = 500;  // BOID POPULATION

inline float BoidRadius = 2.0f;  // BOID SIZE


inline float BoidBoundLimits = BoidRadius * 4;


inline float BoidTurnRate = 5.0f;

inline Color BoidColor = Fade(BLACK, 0.6f);


inline float MaxDetectionAngle = 120;  // DETECTION ANGLE



// ========== DETECTION RADIUS ==========
inline float BoidCoherenceRadius = 100.0f; // How far away neighboring boids can be detected to calculate coherence force
inline float BoidSeperationRadius = 35.0f;
inline float BoidAlignmentRadius = 150.0f;
// ========== DETECTION RADIUS ==========



// ========= STRENGTH FOR EACH RULE =========
inline float CoherenceStrength = 30.0f; // How strongly boids are attracted to the center of mass of their neighbors
inline float SeparationStrength = 2.5f; // 2.5f
inline float AlignmentStrength = 0.09f;
// ========= STRENGTH FOR EACH RULE =========




// ========= BOID SPEED PARAMETERS ===========
inline float MaxBoidSpeed = 180.0f;
inline float MaxBoidSpeedRange = MaxBoidSpeed / 2.5f;
inline float MinBoidSpeed = MaxBoidSpeed * 0.8f;
// ========= BOID SPEED PARAMETERS ===========



inline Color sky{102, 191, 255, 255};