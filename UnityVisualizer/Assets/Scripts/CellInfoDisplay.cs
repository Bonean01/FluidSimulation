using UnityEngine;
using TMPro;

public class CellInfoDisplay : MonoBehaviour {
    [SerializeField] private FluidSimulationAdapter m_simulationAdapter;
    [SerializeField] private float timeStep;
    private TextMeshPro m_TMPro;



    void Start() {
        m_TMPro = GetComponent<TextMeshPro>();
    }


    void Update() {
        if (Input.GetKey(KeyCode.LeftControl) && Input.GetMouseButtonDown(0)) {
            Vector2 mousePosWorld = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            Vector2 mousePosGrid = m_simulationAdapter.WorldToGridPoint(mousePosWorld);
            Vector2Int cellPos = new((int)mousePosGrid.x, (int)mousePosGrid.y);

            if (cellPos.x < 0  || m_simulationAdapter.Width <= cellPos.x || cellPos.y < 0  || m_simulationAdapter.Height <= cellPos.y) return;

            CellInfo info = m_simulationAdapter.GetCellInfo(cellPos.x, cellPos.y, timeStep);
            m_TMPro.text = $"CELL ({cellPos.x}, {cellPos.y})\n" +
                "-------------------\n" +
                $"VelRight: {info.velRight}\n" +
                $"VelLeft: {info.velLeft}\n" +
                $"VelTop: {info.velTop}\n" +
                $"VelBottom: {info.velBottom}\n" +
                $"Normal: ({info.normal.x}, {info.normal.y})\n" +
                $"Intercept: {info.intercept}\n" +
                $"Volume Fraction: {info.volumeFraction}\n" +
                $"Flux Right: {info.fluxRight}\n" +
                $"Flux Left: {info.fluxLeft}\n" +
                $"Flux Top: {info.fluxTop}\n" +
                $"Flux Bottom: {info.fluxBottom}\n";
        }
    }


    void OnDrawGizmos() {
        // Maybe later add a selection box around the selected cell
    }
}
