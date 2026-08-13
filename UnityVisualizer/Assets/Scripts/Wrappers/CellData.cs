using System.Runtime.InteropServices;

public enum CellType : byte {
    Fluid,
    Solid
}


[StructLayout(LayoutKind.Sequential)]
public readonly struct CellData {
    public readonly CellType cellType;
    
    public CellData(CellType cellType = CellType.Fluid) {
        this.cellType = cellType;
    }
}
