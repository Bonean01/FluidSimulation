using UnityEngine;

enum BrushMode {
    Velocity,
    SolidCell,
    Color
}


[RequireComponent(typeof(FluidSimulationAdapter))]
public class FluidPainter : MonoBehaviour {

    [SerializeField] private BrushMode brushMode;
    [SerializeField] [Range(.5f, 2)] private float brushSize;
    [SerializeField] [Range(.5f, 2)] private float brushStrength;
    private FluidSimulationAdapter m_simulationAdapter;
    private float m_brushSizeGrid;


    private void Awake() {
        m_simulationAdapter = GetComponent<FluidSimulationAdapter>();
    }


    private Vector2 m_prevMousePosGrid, m_mouseGridDelta;
    private void Update() {
        if (!IsWithinBounds(Input.mousePosition)) return;

        Vector2 mousePosGrid = WorldToSimulationGrid(Camera.main.ScreenToWorldPoint(Input.mousePosition));
        m_mouseGridDelta = mousePosGrid - m_prevMousePosGrid;
        if (Input.GetMouseButton(0)) Paint(mousePosGrid);
        m_prevMousePosGrid = mousePosGrid;
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


    private void Paint(Vector2 mousePosGrid) {
        Vector2 mouseVel = m_mouseGridDelta / Time.deltaTime; // we also need to take the cellsize into consideration
        mouseVel *= brushStrength;
        Vec2f velocity = new(mouseVel.x, mouseVel.y);

        float widthWorld = transform.localScale.x;
        int widthGrid = m_simulationAdapter.Width;
        m_brushSizeGrid =  brushSize * widthGrid / widthWorld;

        m_simulationAdapter.ApplyVelocityImpulse(mousePosGrid, velocity, m_brushSizeGrid);
    }
}
