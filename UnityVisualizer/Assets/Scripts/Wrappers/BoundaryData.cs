using System.Runtime.InteropServices;

public enum BoundaryType : byte {
    None,
    NoSlip,
    FreeSlip,
    VelocityInlet,
    VelocityOutlet
}


[StructLayout(LayoutKind.Sequential)]
public readonly struct BoundaryData {
    public readonly BoundaryType boundaryType;
    [MarshalAs(UnmanagedType.I1)] public readonly bool hasPrescribedVelocity;
    public readonly Vec2f prescribedVelocity;


    public BoundaryData(BoundaryType boundaryType = BoundaryType.None) {
        this.boundaryType = boundaryType;
        prescribedVelocity = new();
        hasPrescribedVelocity = false;
    }

    public BoundaryData(BoundaryType boundaryType, Vec2f prescribedVelocity) {
        this.boundaryType = boundaryType;
        this.prescribedVelocity = prescribedVelocity;
        hasPrescribedVelocity = true;
    }
}
