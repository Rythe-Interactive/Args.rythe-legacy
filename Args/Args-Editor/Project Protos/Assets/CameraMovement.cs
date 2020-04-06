using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMovement : MonoBehaviour
{
    [SerializeField] private Transform player1;
    [SerializeField] private Transform player2;
    [SerializeField] private float height;
    private Vector3 Offset;

    void Start()
    {

    }

    void Update()
    {
        Offset.x = (player1.position.x+player2.position.x)*0.5f;
        Offset.z = (player1.position.z+player2.position.z)*0.5f - 5f;
        Offset.y = height;

        transform.position = Offset;
    }
}
