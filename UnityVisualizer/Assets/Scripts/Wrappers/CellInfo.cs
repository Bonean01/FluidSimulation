using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public readonly struct CellInfo {
    public readonly float velRight;
    public readonly float velLeft;
    public readonly float velTop;
    public readonly float velBottom;
    public readonly Vec2f normal;
    public readonly float intercept;
    public readonly float volumeFraction;
    public readonly float fluxRight;
    public readonly float fluxLeft;
    public readonly float fluxTop;
    public readonly float fluxBottom;
}
