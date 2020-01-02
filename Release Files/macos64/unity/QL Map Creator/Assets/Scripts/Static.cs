using System;
using System.IO;
using UnityEngine;
using UnityEditor;
using System.Collections.Generic;

public class Static : MonoBehaviour
{
    [SerializeField] private GameObject space;
    private Transform gridTransform;

    int step;
    private string path;
    private Vector3Int mapSize;
    private bool isPlaying, goalThere;

    List<GameObject> grid = new List<GameObject>();

    private void Start()
    {
        gridTransform = GetComponent<Transform>();
          
        step = 0;
        isPlaying = true;
        goalThere = false;
        mapSize.x = Convert.ToInt32(gridTransform.localScale.x);
        mapSize.y = Convert.ToInt32(gridTransform.localScale.y);
        mapSize.z = Convert.ToInt32(gridTransform.localScale.z);

        path = EditorUtility.SaveFilePanel("Save new map", "", "Map_" + mapSize.x + 'x' + mapSize.y + 'x' + mapSize.z, "csv");

        for (int z = 0; z < mapSize.z; z++)
            for (int y = 0; y < mapSize.y; y++)
                for (int x = 0; x < mapSize.x; x++)
                {
                    space.transform.position = new Vector3(x, y, z) - gridTransform.localScale / 2 + new Vector3(0.5f, 0.5f, 0.5f) + gridTransform.transform.position;
                    grid.Add(Instantiate(space));
                }

        Destroy(space);
    }

    private void Update()
    {
        if (isPlaying)
        {
            if (step == 1)
            {
                FileStream fout = new FileStream(path, FileMode.Create);
                using (StreamWriter writer = new StreamWriter(fout))
                {
                    writer.Write("map-size:;");
                    writer.Write(mapSize.x);
                    writer.Write(';');
                    writer.Write(mapSize.y);
                    writer.Write(';');
                    writer.WriteLine(mapSize.z);
                    writer.WriteLine();

                    for (int z = 0; z < mapSize.z; z++)
                    {
                        for (int y = 0; y < mapSize.y; y++)
                        {
                            for (int x = 0; x < mapSize.x; x++)
                            {
                                if (grid[mapSize.y * mapSize.x * z + mapSize.y * y + x].GetComponent<OnCollision>().goal && !goalThere)
                                {
                                    writer.Write('G');
                                    goalThere = true;
                                }
                                else if (grid[mapSize.y * mapSize.x * z + mapSize.y * y + x].GetComponent<OnCollision>().wall)
                                    writer.Write('W');
                                else
                                    writer.Write('S');

                                writer.Write(';');

                                Destroy(grid[mapSize.y * mapSize.x * z + mapSize.y * y + x]);
                            }
                            writer.WriteLine();
                        }
                        writer.WriteLine();
                    }
                }
                fout.Close();

                isPlaying = false;
                print("Done");
                Application.Quit();
            }
            step++;
        }
    }
}
