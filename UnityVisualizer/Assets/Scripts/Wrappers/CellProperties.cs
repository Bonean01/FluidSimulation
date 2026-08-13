using System.Runtime.InteropServices;


[StructLayout(LayoutKind.Sequential)]
public readonly struct CellProperties {
    public readonly CellData cellData;
    public readonly BoundaryData boundaryData;

    public CellProperties(CellData cellData = new(), BoundaryData boundaryData = new()) {
        this.cellData = cellData;
        this.boundaryData = boundaryData;
    }
}
