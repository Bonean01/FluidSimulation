using System.Runtime.InteropServices;


public enum BoundaryCondition : byte {
    None,
    Dirichlet,
    HomogeneousNeumann
}



[StructLayout(LayoutKind.Sequential)]
public readonly struct BoundaryData {
    public readonly BoundaryCondition velocityBoundaryCondition;
    public readonly Vec2f prescribedVelocity;

    public BoundaryData(BoundaryCondition velocityBoundaryCondition = BoundaryCondition.None, Vec2f prescribedVelocity = new Vec2f()) {
        this.velocityBoundaryCondition = velocityBoundaryCondition;
        this.prescribedVelocity = prescribedVelocity;
    }
}
