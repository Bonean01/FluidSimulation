#include "math/dataStructures/Vector.h"
#include "math/dataStructures/Grid.h"


struct InterfaceData {
    Vec2f normal;
    float intercept;
};


class FluidInterface2D {
public:
    const InterfaceData& getInterfaceData(int i, int j) { return m_interfaceData.getValue(i, j); }
    void setInterfaceData(int i, int j, InterfaceData value) { m_interfaceData.setValue(i, j, value); }

    float computeFluxAt(int i, int j);

private:
    Grid2D<InterfaceData> m_interfaceData;
};
