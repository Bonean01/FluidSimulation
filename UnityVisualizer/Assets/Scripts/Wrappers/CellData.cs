using System.Runtime.InteropServices;
using Unity.VisualScripting;


public enum CellType : byte {
    Fluid,
    Solid,
    Empty
}


[StructLayout(LayoutKind.Sequential)]
public readonly struct CellData {
    public readonly CellType cellType;
    public readonly byte isInlet;
    
    public CellData(CellType cellType = CellType.Fluid, bool isInlet = false) {
        this.cellType = cellType;
        this.isInlet = (byte)(isInlet ? 1 : 0);
    }
}
