using UnityEngine;

public class OnCollision : MonoBehaviour
{
    public bool touchedWall, touchedGoal;

    public void Awake()
    {
        touchedWall = false;
        touchedGoal = false;
    }

    public void OnCollisionEnter(Collision collisionInfo)
    {
        switch (collisionInfo.collider.tag)
        {
            case "Goal":
                touchedGoal = true;
                break;
            default:
                touchedWall = true;
                break;
        }
    }
}
