using UnityEngine;

public class OnCollision : MonoBehaviour
{
    public bool wall, goal;

    private void Awake()
    {
        wall = false;
        goal = false;
    }

    public void OnCollisionStay(Collision collision)
    {
        switch (collision.collider.tag)
        {
            case "goal":
                goal = true;
                break;
            case "wall":
                wall = true;
                break;
        }
    }

    private void OnCollisionEnter(Collision collision)
    {
        switch (collision.collider.tag)
        {
            case "goal":
                goal = true;
                break;
            case "wall":
                wall = true;
                break;
        }
    }
}
