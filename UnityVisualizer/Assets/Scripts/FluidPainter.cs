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


    private Vector2 m_prevMousePosWorld, m_mouseWorldDelta;
    private void Update() {
        if (!IsWithinBounds(Input.mousePosition)) return;

        Vector2 mousePosWorld = Camera.main.ScreenToWorldPoint(Input.mousePosition);
        Vector2 mousePosGrid = m_simulationAdapter.WorldToGridPoint(mousePosWorld);
        m_mouseWorldDelta = mousePosWorld - m_prevMousePosWorld;
        if (Input.GetMouseButton(0)) Paint(mousePosGrid);
        m_prevMousePosWorld = mousePosWorld;
    }


    private bool IsWithinBounds(Vector2 screenPos) {
        Vector2 worldPos = Camera.main.ScreenToWorldPoint(screenPos);
        return worldPos.x < transform.position.x + transform.localScale.x / 2
            && worldPos.x > transform.position.x - transform.localScale.x / 2
            && worldPos.y < transform.position.y + transform.localScale.y / 2
            && worldPos.y > transform.position.y - transform.localScale.y / 2;
    }


    private void Paint(Vector2 mousePosGrid) {
        Vector2 mouseVel = m_mouseWorldDelta / Time.deltaTime; // we also need to take the cellsize into consideration
        mouseVel *= brushStrength;
        Vec2f velocity = new(mouseVel.x, mouseVel.y);

        float widthWorld = transform.localScale.x;
        int widthGrid = m_simulationAdapter.Width;
        m_brushSizeGrid =  brushSize * widthGrid / widthWorld;

        m_simulationAdapter.ApplyVelocityImpulse(mousePosGrid, velocity, m_brushSizeGrid);
    }
}
