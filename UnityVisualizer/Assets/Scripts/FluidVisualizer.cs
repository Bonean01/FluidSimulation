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
    
    private void Awake() {
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
        FluidSimulation simulation = new(width, height, cellWidth);
        for (int i = 0; i < simulation.GetWidth(); i++) {
            for (int j = 0; j < simulation.GetHeight(); j++) {
                float velX = Random.Range(0.0f, 1.0f);
                float velY = Random.Range(0.0f, 1.0f);
                simulation.SetVelocity(i, j, new(velX, velY));
            }
        }

        // Create the textures from the fields
        simulation.UpdateVelocityTexture(ref m_velocityTexture);
        simulation.UpdateSolidMapCellTexture(ref m_solidCellMapTexture);

        m_spriteRenderer.material.SetTexture("_VelocityTexture", m_velocityTexture);
        m_spriteRenderer.material.SetTexture("_SolidCellMapTexture", m_solidCellMapTexture);
    }

    private void Update() {
        m_spriteRenderer.material.SetInt("_DisplayedField", (int)displayedField);
    }
}
