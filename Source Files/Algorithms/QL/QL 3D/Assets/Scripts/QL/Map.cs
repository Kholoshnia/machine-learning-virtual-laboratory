using UnityEngine;
using System.Collections.Generic;

class Map
{
    public char[,,] map;
    public List<int> Initials { get; }
    public List<GameObject> Walls { get; }
    public GameObject Finish { get; private set; }
    public List<GameObject> Spaces { get; private set; }

    public Map()
    {
        Initials = new List<int>();
        Walls = new List<GameObject>();
        Spaces = new List<GameObject>();
    }

    public void Initialize(Vector3Int mapSize)
    {
        GameObject space = GameObject.Find("Space"), wall = GameObject.Find("Wall"), goal = GameObject.Find("Goal");

        int k = 0;
        for (int z = mapSize.z - 1; z >= 0; z--)
            for (int y = mapSize.y - 1; y >= 0; y--)
                for (int x = 0; x < mapSize.x; x++)
                {
                    space.transform.position = new Vector3(x, mapSize.y - 1 - y, mapSize.z - 1 - z);

                    switch (map[z, y, x])
                    {
                        case 'G':
                            goal.transform.position = new Vector3(x, y, z);
                            Spaces.Add(Object.Instantiate(space));
                            Finish = Object.Instantiate(goal);
                            break;
                        case 'W':
                            wall.transform.position = new Vector3(x, y, z);
                            Spaces.Add(Object.Instantiate(space));
                            Walls.Add(Object.Instantiate(wall));
                            break;
                        case 'S':
                            Spaces.Add(Object.Instantiate(space));
                            Initials.Add(k);
                            break;
                    }

                    k++;
                }

        for (int i = 0; i < Spaces.Count; i++)
            Spaces[i].name = "Position " + i;

        for (int i = 0; i < Walls.Count; i++)
            Walls[i].name = "Walls " + i;

        Object.Destroy(space);
        Object.Destroy(wall);
        Object.Destroy(goal);
    }
};