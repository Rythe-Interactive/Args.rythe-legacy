using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class groundCheck : MonoBehaviour
{
    public Movement2 movement2;
    public Movement movement;
    public bool grounded;

    void Start()
    {
        movement2 = GetComponentInParent<Movement2>();
        if(movement2 == null)
        {
            movement = GetComponentInParent<Movement>();
        }
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerStay(Collider other)
    {
        if(!other.GetComponentInParent<Player>())
        {
            grounded = true;
            if (movement != null)
                movement.grounded = true;
            if (movement2 != null)
                movement2.grounded = true;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (!other.GetComponentInParent<Player>())
        {
            grounded = false;
            if (movement != null)
                movement.grounded = false;
            if (movement2 != null)
                movement2.grounded = false;
        }
    }
}
