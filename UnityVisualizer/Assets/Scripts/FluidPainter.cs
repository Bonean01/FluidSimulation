using UnityEngine;

enum BrushMode {
    Velocity,
    SolidCell,
    Color
}


[RequireComponent(typeof(FluidSimulationAdapter))]
public class FluidPainter : MonoBehaviour {

    [SerializeField] private BrushMode brushMode;
    [SerializeField] private float brushSize;
    [SerializeField] [Range(0, 1)] private float brushStrength;
    private FluidSimulationAdapter m_simulationAdapter;


    private void Awake() {
        m_simulationAdapter = GetComponent<FluidSimulationAdapter>();
    }


    private void Update() {
        if (!IsWithinBounds(Input.mousePosition)) return;
        if (Input.GetMouseButton(0)) Paint();
    }


    private bool IsWithinBounds(Vector2 screenPos) {
        Vector2 worldPos = Camera.main.ScreenToWorldPoint(screenPos);
        return worldPos.x < transform.position.x + transform.localScale.x / 2
            && worldPos.x > transform.position.x - transform.localScale.x / 2
            && worldPos.y < transform.position.y + transform.localScale.y / 2
            && worldPos.y > transform.position.y - transform.localScale.y / 2;
    }


    private Vector2Int WorldToSimulationGrid(Vector2 worldPos) {
        float widthWorld = transform.localScale.x;
        int widthGrid = m_simulationAdapter.Width;
        int heightGrid = m_simulationAdapter.Height;
        float cellWidthWorld = widthWorld / widthGrid;
        return new() {
            x = (int)(worldPos.x / cellWidthWorld + widthGrid / 2),
            y = (int)(worldPos.y / cellWidthWorld + heightGrid / 2)
        };
    }


    private void Paint() {
        Vector2Int mousePosGrid = WorldToSimulationGrid(Camera.main.ScreenToWorldPoint(Input.mousePosition));

        Vector2 mouseVel = Input.mousePositionDelta / Time.deltaTime; // this should probably use the fixed delta
        mouseVel *= brushStrength;
        Vec2f velocity = new(mouseVel.x, mouseVel.y);

        m_simulationAdapter.ApplyVelocityImpulse(mousePosGrid, velocity, brushSize);
    }
}
