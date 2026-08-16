using System.Runtime.InteropServices;
using Unity.VisualScripting;

[StructLayout(LayoutKind.Sequential)]
public struct Vec2f {
    public float x, y;
    public Vec2f(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public static Vec2f operator *(Vec2f vec, float a) => new(vec.x * a, vec.y * a);
    public static Vec2f operator *(float a, Vec2f vec) => new(a * vec.x, a * vec.y);
}
