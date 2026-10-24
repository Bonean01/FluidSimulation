using System.Runtime.InteropServices;


[StructLayout(LayoutKind.Sequential)]
public readonly struct InterfaceData {
    public readonly Vec2f normal;
    public readonly float intercept;
}
