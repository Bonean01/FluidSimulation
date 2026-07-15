using UnityEngine;


[RequireComponent(typeof(FluidSimulationAdapter))]
public class FluidSimulationPlayer : MonoBehaviour {

    [SerializeField] private uint fixedFrameRate = 60;
    private FluidSimulationAdapter m_simulationAdapter;
    private float m_fixedDelta;
    private int m_speedUp = 1;
    private bool m_paused;
    private double m_timer; 


    private void Awake() {
        m_simulationAdapter = GetComponent<FluidSimulationAdapter>();
        m_fixedDelta = 1.0f / fixedFrameRate;
    }


    private void Update() { 
        ProcessInput();
        if (!m_paused) m_timer += Time.deltaTime;
        if (m_timer >= m_fixedDelta) {
            m_simulationAdapter.StepSimulation(m_fixedDelta * m_speedUp);
            m_timer = 0;
        }
    }


    private void ProcessInput() {
        // Pause / Resume
        if (Input.GetKeyDown(KeyCode.Space)) m_paused = !m_paused;

        // Speed up x2
        //if (Input.GetKeyDown(KeyCode.UpArrow) && m_speedUp < 4.0f) m_speedUp *= 2;
        
        // Speed down x2
        //if (Input.GetKeyDown(KeyCode.DownArrow) && m_speedUp > 1.0f / 4.0f) m_speedUp /= 2;

        // Step if paused
        if (Input.GetKeyDown(KeyCode.RightArrow) && m_paused) m_simulationAdapter.StepSimulation(m_fixedDelta);

        // Step back?
    }
}
