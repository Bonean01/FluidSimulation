using System.Runtime.InteropServices;


[StructLayout(LayoutKind.Sequential)]
public readonly struct CellConfig {
    public readonly CellData cellData;
    public readonly BoundaryData boundaryData;

    public CellConfig(CellData cellData = new(), BoundaryData boundaryData = new()) {
        this.cellData = cellData;
        this.boundaryData = boundaryData;
    }
}
