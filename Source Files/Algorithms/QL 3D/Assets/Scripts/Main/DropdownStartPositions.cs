using UnityEngine;
using UnityEngine.UI;

public class DropdownStartPositions : MonoBehaviour
{
	public int value;

    public void Awake() { value = 0; }

    public void OnValueChanged()
	{
		if (GameObject.FindWithTag("StartPositions").GetComponent<Dropdown>().options[GameObject.FindWithTag("StartPositions").GetComponent<Dropdown>().value].text != "Choose start...")
		{
            value = GameObject.FindWithTag("StartPositions").GetComponent<Dropdown>().value;
		}
	}
}
