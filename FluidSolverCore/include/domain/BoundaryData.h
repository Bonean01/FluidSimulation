#pragma once

#include "BoundaryType.h"
#include "math/dataStructures/Vector.h"

struct BoundaryData {
    BoundaryType boundaryType = BoundaryType::None;
    bool hasPrescribedVelocity = false;
    Vec2f prescribedVelocity = 0.0f;

    BoundaryData() {}

    BoundaryData(BoundaryType boundaryType) :
        boundaryType(boundaryType) { hasPrescribedVelocity = false; }

    BoundaryData(BoundaryType boundaryType, float prescribedVelocity) :
        boundaryType(boundaryType),
        prescribedVelocity(prescribedVelocity) { hasPrescribedVelocity = true; }
};


/*
Maybe rather than storing all of this implicitly on m_cellData we could:

Option 1:
    - Abstract out the MACGrid2D's "Grid-related" operations into a parent class StaggeredGrid2D
    - Have a StaggeredGrid2D<BoundaryData> m_boundaryData field in the simulation

Option 2: <==== potentially better
    - Be a bit more accurate on the usage of the MACGrid and hold both a vector field and a
    scalar field, or rather a Grid2D and a StaggeredGrid2D, so that we could have:
    MACGrid2D<float, float> for the pressure and velocity fields, and MACGrid2D<CellData, BoundaryData>
    for all the domain data

We are just missing a better way to iterate through the two components of the staggered part.
    
Potential solution maybe?
    - Expose both component arrays through getter methods, prepare generic methods for each component and
    call them separately and make them independent from their location, ie rather than X: i + 1, j
    and Y: i, j + 1, have smth like X, Y: getAdyacent(VectorComponent) or getOposite(VectorComponent), or
    even better design to not need those methods. Then have Advection::execute(VectorComponent::X)
    and Advection::execute(VectorComponent::Y), same for diffusion, same for projection. Also have
    methods for getting the location of the edges such as getEdgePosition(i, j, component)
*/
