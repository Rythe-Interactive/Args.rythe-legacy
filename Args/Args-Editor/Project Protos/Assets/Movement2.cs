using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Movement2 : MonoBehaviour
{
    [SerializeField] float speed;

    [SerializeField] private float rotateSpeed;
    private Push pushScript;
    [SerializeField] private float JumpSpeed;
    private float DistToGround;

    public bool grounded;
    private float angle;
    private float translation;

    // Start is called before the first frame update
    void Start()
    {


        pushScript = GetComponent<Push>();
        DistToGround = GetComponent<BoxCollider>().size.y + 0.1f;
    }

    // Update is called once per frame
    void FixedUpdate()
    {

        //CheckGrounded();
        translation = 0;

        if (!pushScript.Pushing)
        {
            //PS4 Rotation
            angle = Input.GetAxis("PS4_RIGHTHOR_2") * rotateSpeed * Time.deltaTime;
            transform.Rotate(0, angle, 0);
            

            ////keyboard
            //if (Input.GetKeyDown(KeyCode.RightShift) && CheckGrounded())
            //{
            //    transform.GetComponent<Rigidbody>().AddForce(Vector3.up * JumpSpeed);
            //}
            //if (Input.GetKey(KeyCode.LeftArrow))
            //{
            //    transform.Rotate(Vector3.up, -rotateSpeed);
            //}
            //else if (Input.GetKey(KeyCode.RightArrow))
            //{
            //    transform.Rotate(Vector3.up, rotateSpeed);
            //}
        }

        
        ////keyboard movement
        //if (Input.GetKey(KeyCode.UpArrow))
        //{
        //    transform.position += transform.forward * Time.deltaTime * speed;
        //}
        //else if (Input.GetKey(KeyCode.DownArrow))
        //{
        //    transform.position -= transform.forward * Time.deltaTime * speed;
        //}


        //PS4 Movement
        
        translation = -Input.GetAxis("PS4_LEFTVERT_2") * speed * Time.deltaTime;

        if (Input.GetButtonDown("PS4_X_2") && grounded)
        {
            transform.GetComponent<Rigidbody>().AddForce(Vector3.up * JumpSpeed);
        }
        transform.position += transform.forward * translation;

    }

    //bool CheckGrounded()
    //{
    //    Debug.DrawRay(transform.position, Vector3.down * (collider.bounds.size.y*0.5f + 0.1f), Color.magenta);
    //    //RaycastHit hit;
    //    if (Physics.BoxCast(transform.position, collider.bounds.size, Vector3.down, transform.rotation, collider.bounds.size.y * 0.5f + 0.1f))
    //    {
    //        grounded = true;
    //        return true;
    //    }
    //    grounded = false;
    //    return false;
    //}
}