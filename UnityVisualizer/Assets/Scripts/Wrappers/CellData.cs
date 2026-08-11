using System.Runtime.InteropServices;

public enum CellType : byte {
    Fluid,
    Solid
}


[StructLayout(LayoutKind.Sequential)]
public readonly struct CellData {
    public readonly CellType cellType;
    [MarshalAs(UnmanagedType.I1)] public readonly bool hasPrescribedPressure;
    public readonly float prescribedPressure;
    
    public CellData(CellType cellType = CellType.Fluid) {
        this.cellType = cellType;
        prescribedPressure = 0.0f;
        hasPrescribedPressure = false;
    }

    public CellData(CellType cellType, float prescribedPressure) {
        this.cellType = cellType;
        this.prescribedPressure = prescribedPressure;
        hasPrescribedPressure = true;
    }
}
