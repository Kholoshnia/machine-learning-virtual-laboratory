using System;
using System.IO;
using UnityEngine;
using UnityEditor;

public class Dynamic : MonoBehaviour
{
    [SerializeField] private GameObject space;
    private Transform gridTransform;

    private string path;
    private FileStream fout;
    private int step;
    private Vector3Int mapSize, it;
    private StreamWriter writer;
    private bool isPlaying, goalThere, foutPause;

    private void Start()
    {
        gridTransform = GetComponent<Transform>();

        step = 1;
        isPlaying = true;
        goalThere = false;
        foutPause = false;
        it = Vector3Int.zero;
        mapSize.x = Convert.ToInt32(gridTransform.localScale.x);
        mapSize.y = Convert.ToInt32(gridTransform.localScale.y);
        mapSize.z = Convert.ToInt32(gridTransform.localScale.z);

        path = EditorUtility.SaveFilePanel("Save new map", "", "Map_" + mapSize.x + 'x' + mapSize.y + 'x' + mapSize.z, "csv");
        fout = new FileStream(path, FileMode.Create);
        writer = new StreamWriter(fout);

        writer.Write("map-size:;");
        writer.Write(mapSize.x);
        writer.Write(';');
        writer.Write(mapSize.y);
        writer.Write(';');
        writer.WriteLine(mapSize.z);
        writer.WriteLine();

        space.transform.position = new Vector3(it.x, it.y, it.z) - gridTransform.localScale / 2 + new Vector3(0.5f, 0.5f, 0.5f) + gridTransform.transform.position;
    }

    private void Update()
    {
        if (isPlaying)
        {
            if (step % 3 == 0)
            {
                if (space.GetComponent<OnCollision>().goal && !goalThere)
                {
                    writer.Write('G');
                    goalThere = true;
                }
                else if (space.GetComponent<OnCollision>().wall)
                    writer.Write('W');
                else
                    writer.Write('S');

                writer.Write(';');

                it.x++;
                if (it.x >= mapSize.x)
                {
                    it.y++;
                    it.x = 0;
                    writer.WriteLine();
                }
                if (it.y >= mapSize.y)
                {
                    it.z++;
                    it.y = 0;
                    writer.WriteLine();
                }
                if (it.z >= mapSize.z)
                {
                    isPlaying = false;
                    foutPause = true;
                }

                space.GetComponent<OnCollision>().goal = false;
                space.GetComponent<OnCollision>().wall = false;

                space.transform.position = new Vector3(it.x, it.y, it.z) - gridTransform.localScale / 2 + new Vector3(0.5f, 0.5f, 0.5f) + gridTransform.transform.position;
            }
            step++;
        }
        if (foutPause)
        {
            writer.Close();
            fout.Close();
            foutPause = false;
            print("Done");
        }
    }
}