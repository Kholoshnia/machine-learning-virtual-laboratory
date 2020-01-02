using UnityEngine;

public class Population
{
    private Color color;
    private readonly int layersQuantity;

    public int AfterReach { get; set; }
    public int Gen { get; private set; }
    public int MinStep { get; private set; }
    public int BestAgent { get; private set; }
    public Agent[] Agents { get; private set; }
    public float FitnessSum { get; private set; }
    public bool ReachedTheGoal { get; private set; }

    public Population(int directionArraySize, int populationQuantity, int layersQuantity, float mutationRate, float maxSpeed)
    {
        if (layersQuantity != 1) color = Random.ColorHSV();
        FitnessSum = 0;
        Agents = new Agent[populationQuantity];
        if (layersQuantity != 1)
            for (int i = 0; i < Agents.Length; i++)
                Agents[i] = new Agent(directionArraySize, mutationRate, maxSpeed) { Color = color };
        else
            for (int i = 0; i < Agents.Length; i++)
                Agents[i] = new Agent(directionArraySize, mutationRate, maxSpeed);
        Gen = 1;
        BestAgent = 0;
        MinStep = directionArraySize;
        this.layersQuantity = layersQuantity;
    }

    public void Update()
    {
        for (int i = 0; i < Agents.Length; i++)
        {
            if (Agents[i].Brain.Step > MinStep) Agents[i].Dead = true;
            else Agents[i].Update();
            if (Agents[i].ReachedGoal)
            {
                MinStep = Agents[i].Brain.Step;
                ReachedTheGoal = true;
            }
        }
    }

    public void Mutate()
    {
        for (int i = 1; i < Agents.Length; i++)
            Agents[i].Brain.Mutate();
    }

    public void SetBestAgent()
    {
        float max = 0;
        int maxIndex = 0;
        for (int i = 0; i < Agents.Length; i++)
            if (Agents[i].Fitness > max)
            {
                max = Agents[i].Fitness;
                maxIndex = i;
            }
        BestAgent = maxIndex;
        if (Agents[BestAgent].ReachedGoal)
            MinStep = Agents[BestAgent].Brain.Step;
    }

    public Agent SelectParent()
    {
        float random = Random.Range(0, FitnessSum);
        float runningSum = 0;
        for (int i = 0; i < Agents.Length; i++)
        {
            runningSum += Agents[i].Fitness;
            if (runningSum > random)
                return Agents[i];
        }
        return null;
    }

    public bool AllAgentsDead()
    {
        for (int i = 0; i < Agents.Length; i++)
            if (!Agents[i].Dead && !Agents[i].ReachedGoal)
                return false;
        return true;
    }

    public void CalculateFitness()
    {
        for (int i = 0; i < Agents.Length; i++)
            Agents[i].CalculateFitness();
    }

    public void NaturalSelection()
    {
        Agent[] newAgents = new Agent[Agents.Length];
        newAgents[0] = Agents[BestAgent].GetCopy();
        newAgents[0].IsBest = true;
        for (int i = 1; i < Agents.Length; ++i)
        {
            Agent parent = SelectParent();
            newAgents[i] = parent.GetCopy();
        }
        Agents = newAgents;
        if (layersQuantity != 1)
            for (int i = 0; i < Agents.Length; i++)
                Agents[i].Color = color;
        Gen++;
    }

    public void CalculateFitnessSum()
    {
        FitnessSum = 0;
        for (int i = 0; i < Agents.Length; i++)
            FitnessSum += Agents[i].Fitness;
    }
};