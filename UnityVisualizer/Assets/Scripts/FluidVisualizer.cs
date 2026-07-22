using System.Runtime.CompilerServices;
using UnityEngine;

enum FluidProperty {
    Velocity,
    Speed,
    Pressure,
    SolidCells,
    Divergence
}


[RequireComponent(typeof(SpriteRenderer))]
[RequireComponent(typeof(FluidSimulationAdapter))]
public class FluidVisualizer : MonoBehaviour {

    [SerializeField] private FluidProperty displayedProperty;
    [SerializeField] private Gradient speedGradient;
    [SerializeField] private float maxSpeed;
    private FluidSimulationAdapter m_simulationAdapter;
    private Texture2D m_velocityTexture, m_pressureTexture, m_divergenceTexture, m_solidCellMapTexture;
    private Texture2D m_speedGradientTexture;
    private SpriteRenderer m_spriteRenderer;
    private float m_minPressure, m_maxPressure, m_minDivergence, m_maxDivergence;
    

    private void Awake() {
        m_simulationAdapter = GetComponent<FluidSimulationAdapter>();
        m_spriteRenderer = GetComponent<SpriteRenderer>();

        m_simulationAdapter.OnStateUpdated += OnStateUpdated;
    }


    private void Start() {
        m_velocityTexture = m_simulationAdapter.CreateTexture();
        m_pressureTexture = m_simulationAdapter.CreateTexture();
        m_divergenceTexture = m_simulationAdapter.CreateTexture();
        m_solidCellMapTexture = m_simulationAdapter.CreateTexture();
        
        m_simulationAdapter.UpdateVelocityTexture(ref m_velocityTexture);
        (m_minPressure, m_maxPressure) = m_simulationAdapter.UpdatePressureTexture(ref m_pressureTexture);
        (m_minDivergence, m_maxDivergence) = m_simulationAdapter.UpdateDivergenceTexture(ref m_divergenceTexture);
        m_simulationAdapter.UpdateSolidMapCellTexture(ref m_solidCellMapTexture);

        m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
        m_spriteRenderer.material.SetTexture("_PressureTexture", m_pressureTexture);
        m_spriteRenderer.material.SetTexture("_DivergenceTexture", m_divergenceTexture);
        m_spriteRenderer.material.SetTexture("_SolidCellMapTexture", m_solidCellMapTexture);
        
        m_spriteRenderer.material.SetFloat("_MinPressure", m_minPressure);
        m_spriteRenderer.material.SetFloat("_MaxPressure", m_maxPressure);
        m_spriteRenderer.material.SetFloat("_MinDivergence", m_minDivergence);
        m_spriteRenderer.material.SetFloat("_MaxDivergence", m_maxDivergence);


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
        m_spriteRenderer.material.SetInt("_DisplayedField", (int)displayedProperty);
    }


    private void OnStateUpdated() {
        if (displayedProperty == FluidProperty.Velocity || displayedProperty == FluidProperty.Speed) {
            m_simulationAdapter.UpdateVelocityTexture(ref m_velocityTexture);
            m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
        }

        if (displayedProperty == FluidProperty.Speed) {
            m_spriteRenderer.material.SetTexture("_SpeedGradientTexture", m_speedGradientTexture);
            m_spriteRenderer.material.SetFloat("_MaxSpeedSqrd", maxSpeed * maxSpeed);
        }

        if (displayedProperty == FluidProperty.Pressure) {
            m_simulationAdapter.UpdatePressureTexture(ref m_pressureTexture);
            m_spriteRenderer.material.SetTexture("_PressureTexture", m_pressureTexture);
        }

        if (displayedProperty == FluidProperty.Divergence) {
            m_simulationAdapter.UpdateDivergenceTexture(ref m_divergenceTexture);
            m_spriteRenderer.material.SetTexture("_DivergenceTexture", m_divergenceTexture);
        }
    }
}
