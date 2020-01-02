using UnityEngine;

public class Agent
{
    private Color color;
    private float maxSpeed;
    private Vector3 pos, vel, acc;

    public bool Dead { get; set; }
    public bool IsBest { get; set; }
    public GameObject Sphere { get; set; }
    public Brain Brain { get; private set; }
    public float Fitness { get; private set; }
    public bool ReachedGoal { get; private set; }
    public Color Color { get => color; set => color = value; }

    public Agent(int directionArraySize, float mutationRate, float maxSpeed)
    {
        Dead = false;
        IsBest = false;
        ReachedGoal = false;

        Fitness = 0;
        this.maxSpeed = maxSpeed;

        Brain = new Brain(directionArraySize, mutationRate);
        Sphere = Object.Instantiate(GameObject.FindWithTag("Start"));

        vel = Vector3.zero;
        acc = Vector3.zero;
        pos = Sphere.transform.position;
    }

    private void Move()
    {
        if (Brain.Directions.Length > Brain.Step)
        {
            acc = Brain.Directions[Brain.Step];
            Brain.IncStep();
        }
        else Dead = true;

        if (Mathf.Abs(vel.x + acc.x) < maxSpeed) vel.x += acc.x;
        if (Mathf.Abs(vel.y + acc.y) < maxSpeed) vel.y += acc.y;
        if (Mathf.Abs(vel.z + acc.z) < maxSpeed) vel.z += acc.z;
        pos += vel;

        Sphere.transform.position = pos;
    }

    public void Update()
    {
        if (!Dead && !ReachedGoal)
        {
            Move();
            if (Sphere.GetComponent<OnCollision>().touchedGoal)
                ReachedGoal = true;
            else Dead |= Sphere.GetComponent<OnCollision>().touchedWall;
        }

        Sphere.GetComponent<MeshRenderer>().material.color = IsBest ? Color.green : color;
    }

    public void CalculateFitness()
    {
        if (ReachedGoal) Fitness = Brain.Directions.Length / (Brain.Step * Brain.Step);
        else Fitness = 1.0f / Mathf.Pow(GameObject.FindWithTag("Goal").GetComponent<Information>().GetDistance(Sphere), 2);
    }

    public Agent GetCopy() { return new Agent(Brain.Directions.Length, Brain.MutationRate, maxSpeed) { Brain = Brain.Clone() }; }
};