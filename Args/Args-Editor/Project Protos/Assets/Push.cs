using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Push : MonoBehaviour
{
    public bool Pushing;
    void OnTriggerStay(Collider other)
    {
        if (other.transform.CompareTag("PickUp"))
        {
            //other.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotationY;
            Pushing = true;
        }
        else
        {
            //other.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotationX;
            //other.GetComponent<Rigidbody>().constraints = RigidbodyConstraints.FreezeRotationZ;
            Pushing = false;
        }
    }

    void OnTriggerExit(Collider other)
    {
        Pushing = false;
    }

   
}
