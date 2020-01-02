using UnityEngine;
using UnityEngine.SceneManagement;

public class SetObjects : MonoBehaviour
{
    void Awake()
    {
        for (int i = 0; i < SceneManager.GetActiveScene().GetRootGameObjects().Length; i++)
        {
            if (SceneManager.GetActiveScene().GetRootGameObjects()[i].tag != "grid" &&
                SceneManager.GetActiveScene().GetRootGameObjects()[i].tag != "space" &&
                SceneManager.GetActiveScene().GetRootGameObjects()[i].tag != "goal" &&
                SceneManager.GetActiveScene().GetRootGameObjects()[i].tag != "wall")
            {
                SceneManager.GetActiveScene().GetRootGameObjects()[i].tag = "wall";
                SceneManager.GetActiveScene().GetRootGameObjects()[i].AddComponent<Rigidbody>();
                SceneManager.GetActiveScene().GetRootGameObjects()[i].AddComponent<MeshCollider>();
                SceneManager.GetActiveScene().GetRootGameObjects()[i].GetComponent<MeshCollider>().convex = true;
                SceneManager.GetActiveScene().GetRootGameObjects()[i].GetComponent<Rigidbody>().useGravity = false;
                SceneManager.GetActiveScene().GetRootGameObjects()[i].GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeAll;
                SceneManager.GetActiveScene().GetRootGameObjects()[i].GetComponent<Rigidbody>().collisionDetectionMode = CollisionDetectionMode.Continuous;
            }
        }
    }
}
