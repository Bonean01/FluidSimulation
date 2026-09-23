using System.Runtime.InteropServices;


[StructLayout(LayoutKind.Sequential)]
public readonly struct SurfaceData { 
    public readonly Vec2i position;
    public readonly Vec2f closestSurfacePointPos;
    public readonly float estimatedSD;
    public readonly bool known;  // bools can potentially be problematic
    public readonly bool inQueue;
    public readonly uint depth;
}
