using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class triggerBox : MonoBehaviour
{
    private void OnTriggerStay(Collider other)
    {
        if (other.GetComponent<CountDownOpen>())
        {
            CountDownOpen button = other.GetComponent<CountDownOpen>();
            if (Input.GetKeyDown(KeyCode.Space) || Input.GetKeyDown(KeyCode.RightShift) || Input.GetButtonDown("PS4_SQUARE") || Input.GetButtonDown("PS4_SQUARE_2"))
            {
                if (!button.Activated)
                {
                    foreach (Door door in button.DoorsToOpen)
                    {
                        door.open = true;
                    }
                    foreach (Door door in button.DoorsToClose)
                    {
                        door.open = false;
                    }
                    button.Activated = true;
                }
                else
                {
                    foreach (Door door in button.DoorsToOpen)
                    {
                        door.open = false;
                    }
                    foreach (Door door in button.DoorsToClose)
                    {
                        door.open = true;
                    }
                    button.Activated = false;
                } 
            }
        }
    }
}
