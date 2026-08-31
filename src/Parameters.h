//
// Created by theoa on 31/08/2026.
//

#pragma once


// =============== BORDERS ====================
constexpr int WIDTH = 1500;
constexpr int HEIGHT = 1000;
constexpr int DEPTH = 360;

constexpr Vector2 CenterOfSimulation = Vector2{(float)(WIDTH/2), (float)(HEIGHT/2)};
// =============== BORDERS ====================




inline int BoidCount = 600;  // BOID POPULATION

inline float BoidRadius = 2.5f;  // BOID SIZE


inline float WingsLen = 6.0f + 2;
inline float FrontLen = 4.0f + 2;
inline float TailLen = FrontLen / 3;

inline float BoidBoundLimits = BoidRadius * 4 + 50;


inline float BoidTurnRate = 5.0f;

inline Color BoidColor = Fade(BLACK, 0.6f);
inline Color TailColor = Fade(BLACK, 0.4f);


inline float MaxDetectionAngle = 180;  // DETECTION ANGLE



// ========== DETECTION RADIUS ==========
inline float BoidCoherenceRadius = 90.0f; // How far away neighboring boids can be detected to calculate coherence force
inline float BoidSeperationRadius = 55.0f;
inline float BoidAlignmentRadius = 120.0f;
// ========== DETECTION RADIUS ==========



// ========= STRENGTH FOR EACH RULE =========
inline float CoherenceStrength = 30.0f; // How strongly boids are attracted to the center of mass of their neighbors
inline float SeparationStrength = 10.5f; // 2.5f
inline float AlignmentStrength = 0.09f;
// ========= STRENGTH FOR EACH RULE =========




// ========= BOID SPEED PARAMETERS ===========
inline float MaxBoidSpeed = 210.0f + 100;
inline float MaxBoidSpeedRange = MaxBoidSpeed / 2.5f;
inline float MinBoidSpeed = MaxBoidSpeed * 0.8f;
// ========= BOID SPEED PARAMETERS ===========



inline Color sky{102, 191, 255, 255};