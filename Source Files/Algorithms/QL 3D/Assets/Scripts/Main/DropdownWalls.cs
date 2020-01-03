using UnityEngine;
using UnityEngine.UI;

public class DropdownWalls : MonoBehaviour
{
    public void OnValueChanged()
    {
        if (GameObject.FindWithTag("Walls").GetComponent<Dropdown>().options[GameObject.FindWithTag("Walls").GetComponent<Dropdown>().value].text != "Hide walls...")
        {
            GameObject.Find(GameObject.FindWithTag("Walls").GetComponent<Dropdown>().options[GameObject.FindWithTag("Walls").GetComponent<Dropdown>().value].text).GetComponent<MeshRenderer>().enabled = !GameObject.Find(GameObject.FindWithTag("Walls").GetComponent<Dropdown>().options[GameObject.FindWithTag("Walls").GetComponent<Dropdown>().value].text).GetComponent<MeshRenderer>().enabled;
            GameObject.FindWithTag("Walls").GetComponent<Dropdown>().SetValueWithoutNotify(0);
        }
    }
}
