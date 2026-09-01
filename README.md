# Boids Algorithm Simulation

This is a C++ implementation of the Boids algorithm to simulate flocking behavior.

![Boids Simulation](BoidSimulation.gif)

## Details

Each boid adheres to three core rules:

1. **Cohesion**: The tendency to gravitate toward the center of mass of nearby boids.
2. **Separation**: Boids steer away from close neighbors to avoid collisions.
3. **Alignment**: Boids attempt to fly in the same direction as their neighbors.

The detection range and force strength of each rule can be adjusted. 

Lastly, this is a 3D simulation, so boids move along the Z-axis, adjusting their size based on their depth.

## References

This implementation was based on the pseudocode found [here](http://www.kfish.org/boids/pseudocode.html).
