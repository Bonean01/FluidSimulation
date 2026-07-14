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
            simulation.SetVelocity(3, j, new(0.1f, 0.0f));
        }

        for (int j = 10; j < simulation.GetHeight() - 10; j++) {
            simulation.SetVelocity(simulation.GetWidth() - 4, j, new(0.0f, 0.1f));
        }

        for (int i = 10; i < simulation.GetWidth() - 10; i++) {
            simulation.SetVelocity(i, 3, new(0.1f, 0.0f));
        }

        for (int i = 10; i < simulation.GetWidth() - 10; i++) {
            simulation.SetVelocity(i, simulation.GetHeight() - 4, new(0.0f, 0.1f));
        }

        // Create the textures from the fields
        simulation.UpdateVelocityTexture(ref m_velocityTexture);
        simulation.UpdateSolidMapCellTexture(ref m_solidCellMapTexture);

        m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
        m_spriteRenderer.material.SetTexture("_SolidCellMapTexture", m_solidCellMapTexture);
    }

    private void Update() {
        m_spriteRenderer.material.SetInt("_DisplayedField", (int)displayedField);

        if (Input.GetKeyDown(KeyCode.RightArrow)) {
            simulation.Step(1.0f / 60.0f);
            simulation.UpdateVelocityTexture(ref m_velocityTexture);
            m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
        }
    }
}
