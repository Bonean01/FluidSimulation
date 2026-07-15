using System.Runtime.CompilerServices;
using UnityEngine;

enum FieldType {
    Velocity,
    Speed,
    Pressure,
    SolidCells
}


[RequireComponent(typeof(SpriteRenderer))]
[RequireComponent(typeof(FluidSimulationAdapter))]
public class FluidVisualizer : MonoBehaviour {

    [SerializeField] private FieldType displayedField;
    [SerializeField] private Gradient speedGradient;
    [SerializeField] private float maxSpeed;
    private FluidSimulationAdapter m_simulationAdapter;
    private Texture2D m_velocityTexture, m_pressureTexture, m_solidCellMapTexture;
    private Texture2D m_speedGradientTexture;
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


        float aspectRatio = (float)m_simulationAdapter.Height / m_simulationAdapter.Width;
        transform.localScale.Set(transform.localScale.x, transform.localScale.x * aspectRatio, transform.localScale.z);

        m_speedGradientTexture = new(256, 1) {
            filterMode = FilterMode.Point,
            wrapMode = TextureWrapMode.Clamp
        };
        for (int i = 0; i < 256; i++) {
            Color color = speedGradient.Evaluate(i / 255.0f);
            m_speedGradientTexture.SetPixel(i, 0, color);
        }
        m_speedGradientTexture.Apply();
        //temp
        m_spriteRenderer.material.SetTexture("_SpeedGradientTexture", m_speedGradientTexture);
        m_spriteRenderer.material.SetFloat("_MaxSpeedSqrd", maxSpeed * maxSpeed);
    }


    private void Update() {
        m_spriteRenderer.material.SetInt("_DisplayedField", (int)displayedField);
    }


    private void OnSimulationStepped() {
        m_simulationAdapter.UpdateVelocityTexture(ref m_velocityTexture);
        m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);

        m_spriteRenderer.material.SetTexture("_SpeedGradientTexture", m_speedGradientTexture);
        m_spriteRenderer.material.SetFloat("_MaxSpeedSqrd", maxSpeed * maxSpeed);
    }
}
