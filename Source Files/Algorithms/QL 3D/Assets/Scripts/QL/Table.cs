using UnityEngine;

class Table
{
    private int state;
    private Vector3Int mapSize;
    private readonly float gamma;
    private readonly int finishState;

    public long[,] R { get; }
    public long[,] Q { get; }

    public Table(long finishReward, Vector3Int mapSize, Map map, float gamma)
    {
        this.gamma = gamma;
        this.mapSize = mapSize;

        R = new long[mapSize.x * mapSize.y * mapSize.z, mapSize.x * mapSize.y * mapSize.z];
        Q = new long[mapSize.x * mapSize.y * mapSize.z, mapSize.x * mapSize.y * mapSize.z];

        for (int y = 0; y < mapSize.x * mapSize.y * mapSize.z; y++)
            for (int x = 0; x < mapSize.x * mapSize.y * mapSize.z; x++)
            {
                R[y, x] = -1L;
                Q[y, x] = 0L;
            }

        for (int z = 0; z < mapSize.z; z++)
            for (int y = 0; y < mapSize.y; y++)
                for (int x = 0; x < mapSize.x; x++)
                {
                    if (x - 1 >= 0)
                        if (map.map[z, y, x - 1] == 'S')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * z + mapSize.y * y + (x - 1)] = 0;
                        else if (map.map[z, y, x - 1] == 'G')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * z + mapSize.y * y + (x - 1)] = finishReward;

                    if (x + 1 < mapSize.x)
                        if (map.map[z, y, x + 1] == 'S')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * z + mapSize.y * y + x + 1] = 0;
                        else if (map.map[z, y, x + 1] == 'G')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * z + mapSize.y * y + x + 1] = finishReward;

                    if (y - 1 >= 0)
                        if (map.map[z, y - 1, x] == 'S')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * z + mapSize.y * (y - 1) + x] = 0;
                        else if (map.map[z, y - 1, x] == 'G')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * z + mapSize.y * (y - 1) + x] = finishReward;

                    if (y + 1 < mapSize.y)
                        if (map.map[z, y + 1, x] == 'S')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * z + mapSize.y * (y + 1) + x] = 0;
                        else if (map.map[z, y + 1, x] == 'G')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * z + mapSize.y * (y + 1) + x] = finishReward;

                    if (z - 1 >= 0)
                        if (map.map[z - 1, y, x] == 'S')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * (z - 1) + mapSize.y * y + x] = 0;
                        else if (map.map[z - 1, y, x] == 'G')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * (z - 1) + mapSize.y * y + x] = finishReward;

                    if (z + 1 < mapSize.z)
                        if (map.map[z + 1, y, x] == 'S')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * (z + 1) + mapSize.y * y + x] = 0;
                        else if (map.map[z + 1, y, x] == 'G')
                            R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * (z + 1) + mapSize.y * y + x] = finishReward;

                    if (map.map[z, y, x] == 'G')
                    {
                        R[mapSize.y * mapSize.x * z + mapSize.y * y + x, mapSize.y * mapSize.x * z + mapSize.y * y + x] = finishReward;
                        for (int i = 0; i < map.Spaces.Count; i++)
                            if (map.Spaces[i].transform.position == map.Finish.transform.position)
                                finishState = i;
                    }
                }
    }

    void ChooseAnAction(VisualizationType visualization)
    {
        int possibleAction = GetRandomAction();
        if (R[state, possibleAction] >= 0)
        {
            Q[state, possibleAction] = R[state, possibleAction] + System.Convert.ToInt64(gamma * Maximum(possibleAction, false));
            state = possibleAction;
        }
        if (visualization == VisualizationType.STATES)
            GameObject.FindWithTag("Start").transform.position = GameObject.Find("Position " + state).transform.position;
    }

    int GetRandomAction()
    {
        int act;
        bool choiceIsValid = false;
        do
        {
            act = Random.Range(0, mapSize.x * mapSize.y * mapSize.z - 1);
            choiceIsValid |= R[state, act] > -1;
        } while (!choiceIsValid);
        return act;
    }

    public void Episode(int initState, int iterations, VisualizationType visualization)
    {
        state = initState;
        if (state != finishState)
            do { ChooseAnAction(visualization); } while (state == finishState);
        for (int i = 0; i < iterations; i++)
            ChooseAnAction(visualization);
        if (visualization == VisualizationType.ITERATIONS)
            GameObject.FindWithTag("Start").transform.position = GameObject.Find("Position " + initState).transform.position;
    }

    public long Maximum(int nowState, bool returnIntdex)
    {
        int bestAction = 0;
        double tempMaxQ = 0;
        for (int action = 0; action < mapSize.x * mapSize.y * mapSize.z; action++)
            if (Q[nowState, action] > tempMaxQ)
            {
                tempMaxQ = Q[nowState, action];
                bestAction = action;
            }
        if (returnIntdex)
            return bestAction;
        return Q[nowState, bestAction];
    }
};