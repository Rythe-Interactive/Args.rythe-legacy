using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EndPoint : MonoBehaviour
{
    public bool triggered;

    private void OnTriggerStay(Collider other)
    {
        Player player = other.GetComponent<Player>();
        if(player != null)
        {
            triggered = true;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        Player player = other.GetComponent<Player>();
        if (player != null)
        {
            triggered = false;
        }
    }
}
