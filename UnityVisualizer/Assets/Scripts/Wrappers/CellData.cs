using System.Runtime.InteropServices;

public enum CellType : byte {
    Fluid,
    Solid
}

public enum BoundaryType : byte {
    None,
    NoSlip,
    FreeSlip,
    Inlet,
    Outlet
}


[StructLayout(LayoutKind.Sequential)]
public struct CellData {
    public CellType cellType;
    public BoundaryType boundaryType;
    public Vec2f prescribedVelocity;
    public CellData(CellType cellType = CellType.Fluid, BoundaryType boundaryType = BoundaryType.None, Vec2f prescribedVelocity = new()) {
        this.cellType = cellType;
        this.boundaryType = boundaryType;
        this.prescribedVelocity = prescribedVelocity;
    }
}
