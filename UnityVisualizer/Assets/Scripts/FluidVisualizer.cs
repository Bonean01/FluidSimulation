using UnityEngine;
using UnityEngine.UIElements;

enum FieldType {
    Velocity,
    Pressure,
    SolidCells
}

[RequireComponent(typeof(FluidSimulationAdapter))]
[RequireComponent(typeof(SpriteRenderer))]
public class FluidVisualizer : MonoBehaviour {

    [SerializeField] private FieldType displayedField;
    private FluidSimulationAdapter m_simulationAdapter;
    private Texture2D m_velocityTexture, m_pressureTexture, m_solidCellMapTexture;
    private SpriteRenderer m_spriteRenderer;
    

    private void Awake() {
        m_simulationAdapter = GetComponent<FluidSimulationAdapter>();
        m_spriteRenderer = GetComponent<SpriteRenderer>();
        

        m_velocityTexture = m_simulationAdapter.CreateTexture();
        m_solidCellMapTexture = m_simulationAdapter.CreateTexture();

        //float aspectRatio = (float)height / width;
        //transform.localScale = new Vector3(transform.localScale.x, transform.localScale.x * aspectRatio, transform.localScale.z);
    }


    private void Start() {
        // Create the textures from the fields
        m_simulationAdapter.UpdateVelocityTexture(ref m_velocityTexture);
        m_simulationAdapter.UpdateSolidMapCellTexture(ref m_solidCellMapTexture);

        m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
        m_spriteRenderer.material.SetTexture("_SolidCellMapTexture", m_solidCellMapTexture);
    }

    private void Update() {
        m_spriteRenderer.material.SetInt("_DisplayedField", (int)displayedField);

        //if (Input.GetKeyDown(KeyCode.RightArrow)) {
        //    simulation.Step(1.0f / 60.0f);
        //    simulation.UpdateVelocityTexture(ref m_velocityTexture);
        //    m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
        //}
    }
}
