using UnityEngine;


[RequireComponent(typeof(FluidSimulationAdapter))]
public class FluidSimulationPlayer : MonoBehaviour {

    [SerializeField] private uint fixedFrameRate = 60;
    [SerializeField] private bool startPaused;
    private FluidSimulationAdapter m_simulationAdapter;
    private float m_fixedDelta;
    private bool m_paused;
    private double m_timer; 


    private void Awake() {
        m_simulationAdapter = GetComponent<FluidSimulationAdapter>();
        m_fixedDelta = 1.0f / fixedFrameRate;
        m_paused = startPaused;
    }


    private void Update() { 
        ProcessInput();
        if (!m_paused) m_timer += Time.deltaTime;
        if (m_timer >= m_fixedDelta) {
            m_simulationAdapter.StepSimulation(m_fixedDelta);
            m_timer = 0;
        }
    }


    private void ProcessInput() {
        // Pause / Resume
        if (Input.GetKeyDown(KeyCode.Space)) m_paused = !m_paused;

        // Step if paused
        if (Input.GetKeyDown(KeyCode.RightArrow) && m_paused) m_simulationAdapter.StepSimulation(m_fixedDelta);
    }
}
