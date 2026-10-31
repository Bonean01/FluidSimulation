using System.Runtime.InteropServices;
using UnityEngine.UIElements;

[StructLayout(LayoutKind.Sequential)]
public readonly struct MarkerParticle {
    public readonly Vec2f position;
    public readonly Vec3f color;
}
