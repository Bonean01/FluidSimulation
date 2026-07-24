using UnityEngine;

enum FluidProperty {
    Velocity,
    Speed,
    Pressure,
    SolidCells,
    Divergence,
    Smoke
}


[RequireComponent(typeof(SpriteRenderer))]
public class FluidVisualizer : MonoBehaviour {
    [SerializeField] FluidSimulationAdapter simulationAdapter;
    [SerializeField] private FluidProperty displayedProperty;
    [SerializeField] private Gradient speedGradient;
    [SerializeField] private float maxSpeed;
    private FluidSimulationAdapter m_simulationAdapter;
    private Texture2D m_velocityTexture, m_pressureTexture, m_divergenceTexture, m_smokeTexture, m_solidCellMapTexture;
    private Texture2D m_speedGradientTexture;
    private SpriteRenderer m_spriteRenderer;
    

    private void Awake() {
        if (TryGetComponent(out FluidSimulationAdapter adapter)) m_simulationAdapter = adapter;
        m_simulationAdapter = m_simulationAdapter != null ? m_simulationAdapter : simulationAdapter;

        if (m_simulationAdapter == null) { 
            Debug.LogWarning("The simulation adapter of the fluid visualizer has not been assigned, disabling component");
            enabled = false;
        }

        m_spriteRenderer = GetComponent<SpriteRenderer>();

        m_simulationAdapter.OnStateUpdated += OnStateUpdated;
    }


    private void Start() {
        m_velocityTexture = m_simulationAdapter.CreateTexture();
        m_pressureTexture = m_simulationAdapter.CreateTexture();
        m_divergenceTexture = m_simulationAdapter.CreateTexture();
        m_smokeTexture = m_simulationAdapter.CreateTexture();
        m_solidCellMapTexture = m_simulationAdapter.CreateTexture();

        m_simulationAdapter.UpdateSolidMapCellTexture(ref m_solidCellMapTexture);
        m_spriteRenderer.material.SetTexture("_SolidCellMapTexture", m_solidCellMapTexture);

        float aspectRatio = (float)m_simulationAdapter.Height / m_simulationAdapter.Width;
        transform.localScale.Set(transform.localScale.x, transform.localScale.x * aspectRatio, transform.localScale.z);

        m_speedGradientTexture = new(256, 1) {
            filterMode = FilterMode.Point,
            wrapMode = TextureWrapMode.Clamp
        };
        UpdateMaterialProperties();
    }


    private void OnValidate() => UpdateMaterialProperties();


    private void UpdateMaterialProperties() {
        if (m_spriteRenderer == null || m_speedGradientTexture == null) return;

        m_spriteRenderer.material.SetInt("_DisplayedField", (int)displayedProperty);

        UpdateSpeedGradientTexture(ref m_speedGradientTexture);
        m_spriteRenderer.material.SetTexture("_SpeedGradientTexture", m_speedGradientTexture);
        m_spriteRenderer.material.SetFloat("_MaxSpeedSqrd", maxSpeed * maxSpeed);
        
        OnStateUpdated();
    }


    private void UpdateSpeedGradientTexture(ref Texture2D speedGradientTexture) {
        for (int i = 0; i < 256; i++) {
            Color color = speedGradient.Evaluate(i / 255.0f);
            speedGradientTexture.SetPixel(i, 0, color);
        }
        speedGradientTexture.Apply();
    }


    private void OnStateUpdated() {
        switch (displayedProperty) {
            case FluidProperty.Velocity:
            case FluidProperty.Speed:
                m_simulationAdapter.UpdateVelocityTexture(ref m_velocityTexture);
                m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
                break;
            
            case FluidProperty.Pressure:
                (float minPressure, float maxPressure) = m_simulationAdapter.UpdatePressureTexture(ref m_pressureTexture);
                m_spriteRenderer.material.SetFloat("_MinPressure", minPressure);
                m_spriteRenderer.material.SetFloat("_MaxPressure", maxPressure);
                m_spriteRenderer.material.SetTexture("_PressureTexture", m_pressureTexture);
                print($"min: {minPressure} \t max: {maxPressure}");
                break;

            case FluidProperty.Divergence:
                (float minDivergence, float maxDivergence) = m_simulationAdapter.UpdateDivergenceTexture(ref m_divergenceTexture);
                m_spriteRenderer.material.SetFloat("_MinDivergence", minDivergence);
                m_spriteRenderer.material.SetFloat("_MaxDivergence", maxDivergence);
                m_spriteRenderer.material.SetTexture("_DivergenceTexture", m_divergenceTexture);
                print($"min: {minDivergence} \t max: {maxDivergence}");
                break;

            case FluidProperty.Smoke:
                float total = m_simulationAdapter.UpdateSmokeTexture(ref m_smokeTexture);
                m_spriteRenderer.material.SetTexture("_SmokeTexture", m_smokeTexture);
                print($"total smoke: {total}");  
                break;
        }
    }
}
