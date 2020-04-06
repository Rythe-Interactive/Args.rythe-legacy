using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Pressure : MonoBehaviour
{
    [SerializeField] private List<Door> DoorsToOpen;
    public bool Activated;

    void OnTriggerStay(Collider other)
    {
        if (other.GetComponent<Push>())

        {
            Activated = true;
            foreach (Door door in DoorsToOpen)
            {
                door.open = true;
            }
        }
        else if (other.CompareTag("PickUp"))
        {
            if (other.GetComponent<PickUp>())
            {
                if (other.GetComponent<PickUp>().pickup)
                {
                    foreach (Door door in DoorsToOpen)
                    {
                        door.open = true;
                    }
                    Activated = true;
                }
            }
        }
        else
        {
            foreach (Door door in DoorsToOpen)
            {
                door.open = false;
            }
            Activated = false;
        }
    }

    void OnTriggerExit(Collider other)
    {
        foreach (Door door in DoorsToOpen)
        {
            door.open = false;
        }
        Activated = false;
    }
}
