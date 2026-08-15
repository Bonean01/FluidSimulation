using System.Runtime.InteropServices;

public enum BoundaryType : byte {
    None,
    Dirichlet,
    HomogeneousNeumann
}


[StructLayout(LayoutKind.Sequential)]
public readonly struct BoundaryData {
    public readonly BoundaryType boundaryType;
    public readonly Vec2f prescribedVelocity;

    public BoundaryData(BoundaryType boundaryType = BoundaryType.None, Vec2f prescribedVelocity = new Vec2f()) {
        this.boundaryType = boundaryType;
        this.prescribedVelocity = prescribedVelocity;
    }
}
