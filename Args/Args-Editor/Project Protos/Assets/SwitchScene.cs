using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SwitchScene : MonoBehaviour
{
    public EndPoint[] endpoints;
    void Start()
    {
        endpoints = FindObjectsOfType<EndPoint>();
    }

    // Update is called once per frame
    void Update()
    {
        if(endpoints[0].triggered && endpoints[1].triggered)
        {
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
        }
    }
}
