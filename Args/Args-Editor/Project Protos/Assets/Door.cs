using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Door : MonoBehaviour
{
    public bool open;
    public bool useButton;
    public bool usePlate;
    public List<CountDownOpen> buttons;
    public List<Pressure> plates;
    private Vector3 StartPos;
    private Vector3 OpenPos;

    private int amountOfTruesNeeded;

    void Start()
    {
        StartPos = transform.position;
        OpenPos = StartPos + new Vector3(0, 100, 0);
        open = false;
        if (useButton) amountOfTruesNeeded = buttons.Count;
        else if (usePlate) amountOfTruesNeeded = plates.Count;
    }

    void Update()
    {
        int index = 0;
        if (usePlate)
        {
            foreach (Pressure plate in plates)
            {
                if (plate.Activated) index++;
            }
        }
        else if (useButton)
        {
            foreach (CountDownOpen button in buttons)
            {
                if (button.Activated) index++;
            }
        }
        if (useButton || usePlate)
        {
            if (index >= amountOfTruesNeeded)
            {
                open = true;
            }
            else
            {
                open = false;
            }
        }

        if (open)
        {
            transform.position = OpenPos;
        }
        else
        {
            transform.position = StartPos;
        }

    }
}
