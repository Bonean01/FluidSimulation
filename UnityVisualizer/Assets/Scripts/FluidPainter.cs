using UnityEngine;

enum BrushMode {
    Velocity,
    SolidCell,
    Smoke,
    Color
}


[RequireComponent(typeof(FluidSimulationAdapter))]
public class FluidPainter : MonoBehaviour {
    [SerializeField] [Range(.5f, 2)] private float brushSize;
    [SerializeField] [Range(.5f, 2)] private float brushStrength;
    private FluidSimulationAdapter m_simulationAdapter;
    private float m_brushSizeGrid;


    private void Awake() {
        m_simulationAdapter = GetComponent<FluidSimulationAdapter>();
    }


    private Vector2 m_prevMousePosWorld;
    private void Update() {
        if (!IsWithinBounds(Input.mousePosition)) return;

        Vector2 mousePosWorld = Camera.main.ScreenToWorldPoint(Input.mousePosition);

        if (Input.GetMouseButton(0) || Input.GetMouseButton(1))
            Paint(mousePosWorld);
        
        m_prevMousePosWorld = mousePosWorld;
    }


    private bool IsWithinBounds(Vector2 screenPos) {
        Vector2 worldPos = Camera.main.ScreenToWorldPoint(screenPos);
        return worldPos.x < transform.position.x + transform.localScale.x / 2
            && worldPos.x > transform.position.x - transform.localScale.x / 2
            && worldPos.y < transform.position.y + transform.localScale.y / 2
            && worldPos.y > transform.position.y - transform.localScale.y / 2;
    }


    private void Paint(Vector2 mousePosWorld) {
        // Calculate the world-space velocity
        Vector2 mouseWorldDelta = mousePosWorld - m_prevMousePosWorld;
        Vector2 mouseVel = mouseWorldDelta / Time.deltaTime; // we also need to take the cellsize into consideration

        // Calculate the brush size in grid-space
        float widthWorld = transform.localScale.x;
        int widthGrid = m_simulationAdapter.Width;
        m_brushSizeGrid = brushSize * widthGrid / widthWorld;

        Vector2 mousePosGrid = m_simulationAdapter.WorldToGridPoint(mousePosWorld);
        if (Input.GetMouseButton(0)) {
            Vec2f effectiveVel = new(mouseVel.x * brushStrength, mouseVel.y * brushStrength);
            m_simulationAdapter.ApplyVelocityImpulse(mousePosGrid, effectiveVel, m_brushSizeGrid);
        }
        if (Input.GetMouseButton(1)) {
            m_simulationAdapter.ApplySmokeCircle(mousePosGrid, brushStrength, m_brushSizeGrid);
        }
    }
}
