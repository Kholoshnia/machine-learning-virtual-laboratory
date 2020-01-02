using System;
using System.IO;
using UnityEngine;
using UnityEngine.UI;

public class Main : MonoBehaviour
{
    private Modes mode;

    private string pathIn, pathOut, pathInfo;

    private Layers layers;

    private Vector3 vel, pos;
    private Vector3[] acc;
    private GameObject[] way;
    private GameObject[] walls;

    private int k;
    private bool showTextControls, pause;
    private bool visualization;
    private float mutationRate, maxSpeed;
    private int directionArraySize, populationQuantity, layersQuantity, autoCompletion;

    public void Awake()
    {

        switch (Application.platform)
        {
            /*case RuntimePlatform.OSXPlayer:
                pathInfo = "/Users/vadim/Documents/GitHub/NEAT-QL/Release Files/data/info.csv";
                break;
            case RuntimePlatform.WindowsPlayer:
                pathInfo = Application.dataPath.Remove(Application.dataPath.Length) + "/data/info.csv";
                break;*/
            case RuntimePlatform.OSXEditor:
                pathInfo = "/Users/vadim/Documents/GitHub/mlvl/Source Files/Algorithms/NEAT 3D/Data/info.csv";
                break;
            /*case RuntimePlatform.WindowsEditor:
                pathInfo = Application.dataPath.Remove(Application.dataPath.Length) + "/data/info.csv";
                break;*/
        }

        FileStream fin = new FileStream(pathInfo, FileMode.Open);

        using (StreamReader reader = new StreamReader(fin))
        {
            switch (reader.ReadLine().Split(';')[1])
            {
                case "LEARN":
                    mode = Modes.LEARN;
                    break;
                case "CHECK":
                    mode = Modes.CHECK;
                    break;
                default:
                    Debug.Log("Incorrect mode");
                    break;
            }

            switch (mode)
            {
                case Modes.LEARN:
                    pathIn = reader.ReadLine();
                    pathOut = reader.ReadLine();
                    switch (reader.ReadLine().Split(';')[1])
                    {
                        case "WITH":
                            visualization = true;
                            break;
                        case "WITHOUT":
                            visualization = false;
                            break;
                    }
                    directionArraySize = Convert.ToInt32(reader.ReadLine().Split(';')[1]);
                    populationQuantity = Convert.ToInt32(reader.ReadLine().Split(';')[1]);
                    layersQuantity = Convert.ToInt32(reader.ReadLine().Split(';')[1]);
                    autoCompletion = Convert.ToInt32(reader.ReadLine().Split(';')[1]);
                    maxSpeed = Convert.ToSingle(reader.ReadLine().Split(';')[1]);
                    mutationRate = Convert.ToSingle(reader.ReadLine().Split(';')[1]);
                    break;
                case Modes.CHECK:
                    pathIn = reader.ReadLine();
                    pathOut = reader.ReadLine();
                    break;
            }            
        }
        fin.Close();

        pause = false;
        showTextControls = false;

        fin = new FileStream(pathIn, FileMode.Open);

        using (StreamReader reader = new StreamReader(fin))
        {
            var values = reader.ReadLine().Split(';');
            int numberOfObjects = Convert.ToInt32(values[1]);
            walls = new GameObject[numberOfObjects - 2];

            reader.ReadLine();
            reader.ReadLine();
            values = reader.ReadLine().Replace('.', ',').Split(';');
            GameObject.FindWithTag("Goal").transform.position = new Vector3(Convert.ToSingle(values[1]), Convert.ToSingle(values[2]), Convert.ToSingle(values[3]));
            values = reader.ReadLine().Replace('.', ',').Split(';');
            GameObject.FindWithTag("Goal").transform.rotation = new Quaternion(Convert.ToSingle(values[1]), Convert.ToSingle(values[2]), Convert.ToSingle(values[3]), Convert.ToSingle(values[4]));
            values = reader.ReadLine().Replace('.', ',').Split(';');
            GameObject.FindWithTag("Goal").transform.localScale = new Vector3(Convert.ToSingle(values[1]), Convert.ToSingle(values[2]), Convert.ToSingle(values[3]));
            reader.ReadLine();
            reader.ReadLine();
            values = reader.ReadLine().Replace('.', ',').Split(';');
            GameObject.FindWithTag("Start").transform.position = new Vector3(Convert.ToSingle(values[1]), Convert.ToSingle(values[2]), Convert.ToSingle(values[3]));
            values = reader.ReadLine().Replace('.', ',').Split(';');
            GameObject.FindWithTag("Start").transform.rotation = new Quaternion(Convert.ToSingle(values[1]), Convert.ToSingle(values[2]), Convert.ToSingle(values[3]), Convert.ToSingle(values[4]));
            values = reader.ReadLine().Replace('.', ',').Split(';');
            GameObject.FindWithTag("Start").transform.localScale = new Vector3(Convert.ToSingle(values[1]), Convert.ToSingle(values[2]), Convert.ToSingle(values[3]));

            for (int i = 0; i < numberOfObjects - 2; i++)
            {
                reader.ReadLine();
                reader.ReadLine();
                walls[i] = Instantiate(GameObject.FindWithTag("Wall"));
                walls[i].name = "Wall " + i;
                values = reader.ReadLine().Replace('.', ',').Split(';');
                walls[i].transform.position = new Vector3(Convert.ToSingle(values[1]), Convert.ToSingle(values[2]), Convert.ToSingle(values[3]));
                values = reader.ReadLine().Replace('.', ',').Split(';');
                walls[i].transform.rotation = new Quaternion(Convert.ToSingle(values[1]), Convert.ToSingle(values[2]), Convert.ToSingle(values[3]), Convert.ToSingle(values[4]));
                values = reader.ReadLine().Replace('.', ',').Split(';');
                walls[i].transform.localScale = new Vector3(Convert.ToSingle(values[1]), Convert.ToSingle(values[2]), Convert.ToSingle(values[3]));
            }
        }
        fin.Close();

        Destroy(GameObject.FindWithTag("Wall"));

        if (mode == Modes.LEARN)
        {
            layers = new Layers(directionArraySize, populationQuantity, layersQuantity, mutationRate, maxSpeed);

            GameObject.FindWithTag("TextControls").GetComponent<Text>().text = "[H] - Show controls";

            if (!visualization)
            {
                Destroy(GameObject.FindWithTag("Walls"));
                GameObject.FindWithTag("MainCamera").GetComponent<CameraMovement>().enabled = false;

                for (int i = 0; i < walls.Length; i++)
                    walls[i].GetComponent<MeshRenderer>().enabled = false;

                for (int i = 0; i < Layers.Populations.Length; i++)
                    for (int j = 0; j < Layers.Populations[i].Agents.Length; j++)
                        Layers.Populations[i].Agents[j].Sphere.GetComponent<MeshRenderer>().enabled = false;

                GameObject.FindWithTag("Goal").GetComponent<MeshRenderer>().enabled = false;
                GameObject.FindWithTag("Start").GetComponent<MeshRenderer>().enabled = false;
                Destroy(GameObject.FindWithTag("TextControls"));
            }
        }
        else if (mode == Modes.CHECK)
        {
            pos = GameObject.FindWithTag("Start").transform.position;
            vel = Vector3.zero;

            FileStream fin2 = new FileStream(pathOut, FileMode.Open);

            using (StreamReader reader = new StreamReader(fin2))
            {
                var values = reader.ReadLine().Split(';');
                maxSpeed = Convert.ToInt32(values[1]);
                values = reader.ReadLine().Split(';');
                way = new GameObject[Convert.ToInt32(values[1])];
                acc = new Vector3[way.Length];
                for (int i = 0; i < way.Length; i++)
                {
                    way[i] = Instantiate(GameObject.FindWithTag("Start"));

                    values = reader.ReadLine().Replace('.', ',').Split(';');
                    acc[i] = new Vector3(Convert.ToSingle(values[0]), Convert.ToSingle(values[1]), Convert.ToSingle(values[2]));
                }
            }

            populationQuantity = 1;
            layers = new Layers(directionArraySize, populationQuantity, layersQuantity, mutationRate, maxSpeed);
            Destroy(GameObject.FindWithTag("TextReachedTheGoal"));
            Destroy(GameObject.FindWithTag("ImageReachedTheGoal"));
            Destroy(GameObject.FindWithTag("TextGen"));
            Destroy(GameObject.FindWithTag("ImageGen"));
            GameObject.FindWithTag("TextControls").GetComponent<RectTransform>().transform.localPosition = new Vector3(-205.35f, 230.3f, 0.0f);
        }

        for (int i = 0; i < walls.Length; i++)
            GameObject.FindWithTag("Walls").GetComponent<Dropdown>().options.Add(new Dropdown.OptionData { text = walls[i].name });
    }

    public void Learn()
    {
        if (Input.GetKeyUp(KeyCode.Space))
            pause = !pause;

        if (Input.GetKeyUp(KeyCode.H))
        {
            showTextControls = !showTextControls;

            if (showTextControls) GameObject.FindWithTag("TextControls").GetComponent<Text>().text = "[WASDEQ] - Move camere\n[Mouse] - Rotate camera\n[P] - Change perspective\n[Space] - Pause\n[Enter] - save&exit\n[Esc] - exit\n[H] - Hide controls";
            else GameObject.FindWithTag("TextControls").GetComponent<Text>().text = "[H] - Show controls";
        }

        if (Input.GetKeyUp(KeyCode.Escape))
            Application.Quit();

        if (Input.GetKeyUp(KeyCode.Return) || (layers.GetBestPopulation().AfterReach >= autoCompletion))
        {
            FileStream fout = new FileStream(pathOut, FileMode.Create);
            using (StreamWriter writer = new StreamWriter(fout))
            {
                writer.Write("max-speed:;");
                writer.WriteLine(maxSpeed);
                writer.Write("directions-array-size:;");
                writer.WriteLine(layers.GetBestPopulation().MinStep);
                for (int i = 0; i < layers.GetBestPopulation().MinStep; i++)
                {
                    writer.Write(layers.GetBestPopulation().Agents[layers.GetBestPopulation().BestAgent].Brain.Directions[i].x.ToString().Replace(',', '.'));
                    writer.Write(';');
                    writer.Write(layers.GetBestPopulation().Agents[layers.GetBestPopulation().BestAgent].Brain.Directions[i].y.ToString().Replace(',', '.'));
                    writer.Write(';');
                    writer.Write(layers.GetBestPopulation().Agents[layers.GetBestPopulation().BestAgent].Brain.Directions[i].z.ToString().Replace(',', '.'));
                    writer.WriteLine();
                }
            }
            fout.Close();

            Application.Quit();
            pause = true;
        }

        if (!pause)
        {
            layers.Update();

            if (layers.GetBestPopulation().ReachedTheGoal) GameObject.FindWithTag("TextReachedTheGoal").GetComponent<Text>().text = "Reached the goal: Yes";
            else GameObject.FindWithTag("TextReachedTheGoal").GetComponent<Text>().text = "Reached the goal: No";
            GameObject.FindWithTag("TextGen").GetComponent<Text>().text = "Gen: " + layers.GetBestPopulation().Gen;
        }
    }

    public void Check()
    {
        if (Input.GetKeyUp(KeyCode.H))
        {
            showTextControls = !showTextControls;

            if (showTextControls) GameObject.FindWithTag("TextControls").GetComponent<Text>().text = "[WASDEQ] - Move camere\n[Mouse] - Rotate camera\n[P] - Change perspective\n[Esc] - exit\n[H] - Hide controls";
            else GameObject.FindWithTag("TextControls").GetComponent<Text>().text = "[H] - Show controls";
        }

        if (Input.GetKeyUp(KeyCode.Escape))
            Application.Quit();

        if (k < acc.Length)
        {
            if (Mathf.Abs(vel.x + acc[k].x) < maxSpeed) vel.x += acc[k].x;
            if (Mathf.Abs(vel.y + acc[k].y) < maxSpeed) vel.y += acc[k].y;
            if (Mathf.Abs(vel.z + acc[k].z) < maxSpeed) vel.z += acc[k].z;
            pos += vel;

            way[k].transform.position = pos;

            k++;
        }
    }

    public void Update()
    {
        switch (mode)
        {
            case Modes.LEARN:
                Learn();
                break;
            case Modes.CHECK:
                Check();
                break;
        }
    }
}
