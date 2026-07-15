using System.Runtime.InteropServices;

[StructLayout(LayoutKind.Sequential)]
public struct Vec2f {
    public float x, y;
    public Vec2f(float x, float y) {
        this.x = x;
        this.y = y;
    }
}
