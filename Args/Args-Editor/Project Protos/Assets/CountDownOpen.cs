    using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CountDownOpen : MonoBehaviour
{
    [Range(0.0f,180.0f)] [SerializeField] private float AmountTime;
    private float Time;
    [SerializeField] public List<Door> DoorsToOpen;
    [SerializeField] public List<Door> DoorsToClose;
    [SerializeField] private bool Timed;

    public bool Activated;
    private float TimePassed;

    void Start()
    {
        Time = AmountTime * 60;
    }

    void Update()
    {

        if (Activated)
        {
            
            if (Timed)
            {
                if (TimePassed >= Time)
                {
                    Activated = false;
                    foreach (Door door in DoorsToOpen)
                    {
                        door.open = false;
                    }
                    foreach (Door door in DoorsToClose)
                    {
                        door.open = true;
                    }
                    TimePassed = 0;
                }
                TimePassed++;
            }
        }
    }

    void OnTriggerStay(Collider other)
    {
        
    }


}
