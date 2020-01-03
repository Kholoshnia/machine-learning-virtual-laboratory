using Unity.Jobs;
using UnityEngine;

public class Layers
{
    private static int BestPopulation;
    private readonly UpdateThreads updateThreads;
    public static Population[] Populations { get; private set; }

    private struct UpdateThreads : IJobParallelFor
    {
        public void Execute(int i)
        {
            if (Populations[i].AllAgentsDead())
            {
                Populations[i].CalculateFitness();
                Populations[i].SetBestAgent();
                Populations[i].CalculateFitnessSum();
                SetBestPopulation();
                for (int j = 0; j < Populations[i].Agents.Length; j++)
                    Object.Destroy(Populations[i].Agents[j].Sphere);
                Populations[i].NaturalSelection();
                Populations[i].Mutate();
                if (Populations[i].ReachedTheGoal) Populations[i].AfterReach++;
            }
            else
                Populations[i].Update();
        }
    }

    public Layers(int directionArraySize, int populationQuantity, int layersQuantity, float mutationRate, float maxSpeed)
    {
        BestPopulation = 0;
        updateThreads = new UpdateThreads();
        Populations = new Population[layersQuantity];
        for (int i = 0; i < Populations.Length; i++)
            Populations[i] = new Population(directionArraySize, populationQuantity, layersQuantity, mutationRate, maxSpeed);
    }

    public void Update()
    {
        for (int i = 0; i < Populations.Length; i++)
            updateThreads.Execute(i);
    }

    public bool AllPopulationsDead()
    {
        for (int i = 0; i < Populations.Length; i++)
            if (!Populations[i].AllAgentsDead())
                return false;
        return true;
    }

    public static void SetBestPopulation()
    {
        float max = 0;
        int maxIndex = 0;
        for (int i = 0; i < Populations.Length; i++)
            if (Populations[i].FitnessSum > max)
            {
                max = Populations[i].FitnessSum;
                maxIndex = i;
            }
        BestPopulation = maxIndex;
    }

    public Population GetBestPopulation() { return Populations[BestPopulation]; }
};