using UnityEngine;

enum FieldType {
    Velocity,
    Divergence,
    Pressure,
    SolidCells
}

[RequireComponent(typeof(SpriteRenderer))]
public class FluidVisualizer : MonoBehaviour {
    [SerializeField] private int width, height;
    [SerializeField] private float cellWidth;
    [SerializeField] private FieldType displayedField;
    private Texture2D m_velocityTexture, m_pressureTexture, m_solidCellMapTexture;
    private SpriteRenderer m_spriteRenderer;
    private FluidSimulation simulation;
    
    private void Awake() {
        simulation = new(width, height, cellWidth);

        m_spriteRenderer = GetComponent<SpriteRenderer>();
        
        m_velocityTexture = new(width, height, TextureFormat.RGBA32, mipChain: false, linear: true) {
            filterMode = FilterMode.Point,
            wrapMode = TextureWrapMode.Clamp
        };
        m_solidCellMapTexture = new(width, height, TextureFormat.RGBA32, mipChain: false, linear: true) {
            filterMode = FilterMode.Point,
            wrapMode = TextureWrapMode.Clamp
        };
    }


    private void Start() {
        for (int j = 10; j < simulation.GetHeight() - 10; j++) {
            simulation.SetVelocity(simulation.GetWidth() / 2, j, new(0.1f, 0.0f));
        }

        // Create the textures from the fields
        simulation.UpdateVelocityTexture(ref m_velocityTexture);
        simulation.UpdateSolidMapCellTexture(ref m_solidCellMapTexture);

        m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
        m_spriteRenderer.material.SetTexture("_SolidCellMapTexture", m_solidCellMapTexture);
    }

    private void Update() {
        m_spriteRenderer.material.SetInt("_DisplayedField", (int)displayedField);

        if (Input.GetKeyDown(KeyCode.Space)) {
            simulation.Step(1.0f / 1200.0f);
            simulation.UpdateVelocityTexture(ref m_velocityTexture);
            m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
        }
    }
}
