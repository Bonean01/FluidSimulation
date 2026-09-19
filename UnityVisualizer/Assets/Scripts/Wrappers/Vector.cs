using System.Runtime.InteropServices;

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


[StructLayout(LayoutKind.Sequential)]
public struct Vec2i {
    public float x, y;
    public Vec2i(float x, float y) {
        this.x = x;
        this.y = y;
    }

    public static Vec2i operator *(Vec2i vec, float a) => new(vec.x * a, vec.y * a);
    public static Vec2i operator *(float a, Vec2i vec) => new(a * vec.x, a * vec.y);
}
