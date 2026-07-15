using UnityEngine;

enum FieldType {
    Velocity,
    Pressure,
    SolidCells
}


[RequireComponent(typeof(SpriteRenderer))]
[RequireComponent(typeof(FluidSimulationAdapter))]
public class FluidVisualizer : MonoBehaviour {

    [SerializeField] private FieldType displayedField;
    private FluidSimulationAdapter m_simulationAdapter;
    private Texture2D m_velocityTexture, m_pressureTexture, m_solidCellMapTexture;
    private SpriteRenderer m_spriteRenderer;
    

    private void Awake() {
        m_simulationAdapter = GetComponent<FluidSimulationAdapter>();
        m_spriteRenderer = GetComponent<SpriteRenderer>();

        m_simulationAdapter.SimulationStepped += OnSimulationStepped;
    }


    private void Start() {
        m_velocityTexture = m_simulationAdapter.CreateTexture();
        m_solidCellMapTexture = m_simulationAdapter.CreateTexture();
        
        m_simulationAdapter.UpdateVelocityTexture(ref m_velocityTexture);
        m_simulationAdapter.UpdateSolidMapCellTexture(ref m_solidCellMapTexture);

        m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
        m_spriteRenderer.material.SetTexture("_SolidCellMapTexture", m_solidCellMapTexture);
    }


    private void Update() {
        m_spriteRenderer.material.SetInt("_DisplayedField", (int)displayedField);
    }


    private void OnSimulationStepped() {
        m_simulationAdapter.UpdateVelocityTexture(ref m_velocityTexture);
        m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
    }
}
