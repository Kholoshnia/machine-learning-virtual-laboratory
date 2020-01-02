using UnityEngine;

public class Brain
{
    public float MutationRate { get; }
    public int Step { get; private set; }
    public Vector3[] Directions { get; private set; }

    public Brain(int directionArraySize, float mutationRate)
    {
        Step = 0;
        MutationRate = mutationRate;
        Directions = new Vector3[directionArraySize];

        Randomize();
    }

    public void Mutate()
    {
        for (int i = 0; i < Directions.Length; i++)
        {
            float rand = Random.Range(0.0f, 1.0f);
            if (rand < MutationRate)
                SetDirection(i);
        }
    }

    public void Randomize()
    {
        for (int i = 0; i < Directions.Length; i++)
            SetDirection(i);
    }

    public Brain Clone()
    {
        Brain clone = new Brain(Directions.Length, MutationRate);
        for (int i = 0; i < Directions.Length; i++)
            clone.Directions[i] = Directions[i];
        return clone;
    }

    public void IncStep() { Step++; }

    private void SetDirection(int i) { Directions[i] = new Vector3(Random.Range(-0.1f, 0.1f), Random.Range(-0.1f, 0.1f), Random.Range(-0.1f, 0.1f)); }
};