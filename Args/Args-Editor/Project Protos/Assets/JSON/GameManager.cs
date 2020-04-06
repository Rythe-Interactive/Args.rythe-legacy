using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using Newtonsoft.Json;
using System;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    GameObject[] toSerialize;
    List<Serializable> sObjects;
    string jsonString;
    // Start is called before the first frame update

    public void Serialize()
    {
        toSerialize = FindGameObjectsWithLayer(8);
        sObjects = new List<Serializable>();
        SerializeStuff();
        string finalOut = JsonConvert.SerializeObject(sObjects);
        Debug.Log(finalOut);
        File.WriteAllText(@".\Assets\JSON\JSONScenes\" + SceneManager.GetActiveScene().name + ".JSON", "{\"Scene\":" + finalOut + "}");
    }

    void SerializeStuff()
    {
        for (int i = 0; i < toSerialize.Length; i++)
        {
            Debug.Log("Found Serializable");
            if (!toSerialize[i].GetComponent<CustomScript>())
            {
                toSerialize[i].AddComponent<CustomScript>();
            }
            toSerialize[i].GetComponent<CustomScript>().GetComponents();
            jsonString = "";
            CustomScript cs = toSerialize[i].GetComponent<CustomScript>();
            Serializable so = Serialize(cs.gameObject);
            so.name = cs.gameObject.name;
            for (int j = 0; j < toSerialize[i].transform.childCount; j++)
            {
                GameObject child = toSerialize[i].transform.GetChild(j).gameObject;
                if (!child.GetComponent<CustomScript>())
                {
                    child.AddComponent<CustomScript>();
                }
                child.GetComponent<CustomScript>().GetComponents();
                Serializable sObject = Serialize(toSerialize[i].transform.GetChild(j).gameObject);
                sObject.name = child.name;
                SerializableTransform st = (SerializableTransform)so.components[0];
                st.children.Add(sObject);

            }
            Debug.Log("Added Object");
            sObjects.Add(so);
        }

    }
    Serializable Serialize(GameObject sObject)
    {
        Serializable output = new Serializable();
        CustomScript cs = sObject.GetComponent<CustomScript>();

        for (int j = 0; j < cs.componentList.Length; j++)
        {
            /*Adds the Transform object for serialization*/
            if (cs.componentList[j].GetType().Name.Contains("Transform"))
            {
                Debug.Log("Added Transform");
                SerializableTransform sTransform = new SerializableTransform();
                sTransform.name = "Transform";
                sTransform.position = cs.gameObject.transform.position;
                sTransform.rotation = cs.gameObject.transform.eulerAngles;
                sTransform.scale = cs.gameObject.transform.localScale;
                output.components.Add(sTransform);
            }
            else if(cs.componentList[j].GetType().Name.Contains("Renderable"))
            {
                SerializableMesh sRenderable = new SerializableMesh();
                sRenderable.name = "Renderable";
                Renderable r = cs.gameObject.GetComponent<Renderable>();
                if (sRenderable!= null)
                {
                    sRenderable.mesh = r.data.mesh;
                    sRenderable.material = r.data.material;
                    sRenderable.albedo = r.data.albedo;
                    sRenderable.metal = r.data.metal;
                    sRenderable.roughness = r.data.roughness;
                    sRenderable.normal = r.data.normal;
                    sRenderable.height = r.data.height;
                    sRenderable.ao = r.data.ao;
                    sRenderable.emissive = r.data.emissive;
                }
                output.components.Add(sRenderable);
            }
            else if (cs.componentList[j].GetType().Name.Contains("MeshFilter"))                /*Adds the MeshFilter object for serialization*/
            {
                Debug.Log("MeshRenderer Serialization is deprecated. It will probably be removed in a future update");
                //Debug.Log("Added MeshFilter");
                //SerializableMesh sMesh = new SerializableMesh();
                //sMesh.name = "MeshFilter";
                //MeshFilter mf = cs.gameObject.GetComponent<MeshFilter>();
                //MeshRenderer mr = cs.gameObject.GetComponent < MeshRenderer>();
                //sMesh.mesh = mf.sharedMesh.name;
                //sMesh.material = mr.sharedMaterial.name;
                //sMesh.castShadows = mr.shadowCastingMode == UnityEngine.Rendering.ShadowCastingMode.On;
                //output.components.Add(sMesh);
            }
            else if (cs.componentList[j].GetType().Name.Contains("MeshRenderer"))                /*Adds the MeshRenderer object for serialization*/
            {
                Debug.Log("MeshRenderer Serialization is deprecated. It will probably be removed in a future update");
            //    Debug.Log("Added MeshRenderer");
            //    SerializableMeshRenderer sMeshRenderer = new SerializableMeshRenderer();
            //    sMeshRenderer.name = "MeshRenderer";
            //    MeshRenderer mr = cs.gameObject.GetComponent<MeshRenderer>();
            //    sMeshRenderer.material = mr.sharedMaterial.name;
            //    sMeshRenderer.castShadows = mr.shadowCastingMode == UnityEngine.Rendering.ShadowCastingMode.On;

            //    output.components.Add(sMeshRenderer);
            }
            else if (cs.componentList[j].GetType().Name.Contains("Rigidbody"))                /*Adds the Rigidbody object for serialization */
            {
                Debug.Log("Added Rigidbody");
                SerializableRigidbody sRigidbody = new SerializableRigidbody();
                sRigidbody.name = "Rigidbody";
                Rigidbody rb = cs.gameObject.GetComponent<Rigidbody>();
                sRigidbody.mass = rb.mass;
                sRigidbody.drag = rb.drag;
                sRigidbody.angularDrag = rb.angularDrag;
                sRigidbody.useGravity = rb.useGravity;
                sRigidbody.isKinematic = rb.isKinematic;
                sRigidbody.freezeXPos = rb.constraints == RigidbodyConstraints.FreezePositionX;
                sRigidbody.freezeYPos = rb.constraints == RigidbodyConstraints.FreezePositionY;
                sRigidbody.freezeZPos = rb.constraints == RigidbodyConstraints.FreezePositionZ;
                sRigidbody.freezeXRot = rb.constraints == RigidbodyConstraints.FreezeRotationX;
                sRigidbody.freezeYRot = rb.constraints == RigidbodyConstraints.FreezeRotationY;
                sRigidbody.freezeZRot = rb.constraints == RigidbodyConstraints.FreezeRotationZ;

                output.components.Add(sRigidbody);
            }
            else if (cs.componentList[j].GetType().Name.Contains("Collider"))                /*Adds the Collider object for serialization*/
            {
                Debug.Log("Added Collider");

                if (cs.gameObject.GetComponent<BoxCollider>())
                {
                    SerializableBoxCollider sBCollider = new SerializableBoxCollider();
                    sBCollider.name = "BoxCollider";
                    BoxCollider bc = cs.gameObject.GetComponent<BoxCollider>();
                    sBCollider.isTrigger = bc.isTrigger;
                    sBCollider.center = bc.center;
                    sBCollider.size = bc.size;
                    output.components.Add(sBCollider);
                }
                else if (cs.gameObject.GetComponent<SphereCollider>())
                {
                    SerializableSphereCollider sSCollider = new SerializableSphereCollider();
                    sSCollider.name = "SphereCollider";
                    SphereCollider sc = cs.gameObject.GetComponent<SphereCollider>();
                    sSCollider.isTrigger = sc.isTrigger;
                    sSCollider.center = sc.center;
                    sSCollider.radius = sc.radius;
                    output.components.Add(sSCollider);
                }
            }
            else
            {
                SerializableComponent Script = new SerializableComponent();
                string fName = cs.componentList[j].GetType().ToString();
                if (cs.componentList[j].GetType().ToString().Contains("UnityEngine."))
                {
                    Debug.Log(fName.Length);
                    Script.name = fName.Substring(12);
                }
                else
                {
                    Script.name = fName;
                }
                output.components.Add(Script);
            }
        }
        return output;
    }

    GameObject[] FindGameObjectsWithLayer(int layer)
    {
        GameObject[] goArray = GameObject.FindObjectsOfType<GameObject>();
        Debug.Log(goArray.Length);
        List<GameObject> goList = new List<GameObject>();
        for (var i = 0; i < goArray.Length; i++)
        {
            if (goArray[i].layer == layer)
            {
                goList.Add(goArray[i]);
            }
        }
        if (goList.Count == 0)
        {
            return null;
        }
        Debug.Log(goList.Count);
        return goList.ToArray();
    }
}

[Serializable]
public class SerializableComponent
{
    public string name;
}

[Serializable]
public class Serializable
{
    public string name;
    public List<SerializableComponent> components = new List<SerializableComponent>();
}
[Serializable]
public class SerializableMesh : SerializableComponent
{
    public string name;
    public string mesh;
    public string material;
    public string albedo;
    public string metal;
    public string roughness;
    public string normal;
    public string height;
    public string ao;
    public string emissive;
}
[Serializable]
public class SerializableTransform : SerializableComponent
{
    public string name;
    public Vector3 position = Vector3.zero;
    public Vector4 rotation = new Vector4(0, 0, 0, 0);
    public Vector3 scale = Vector3.one;
    public List<Serializable> children = new List<Serializable>();
}
[Serializable]
public class SerializableRigidbody : SerializableComponent
{
    public string name;
    public float mass = 1;
    public float drag = 0;
    public float angularDrag = 0.05f;
    public bool useGravity = true;
    public bool isKinematic = false;
    public bool freezeXPos = false;
    public bool freezeYPos = false;
    public bool freezeZPos = false;
    public bool freezeXRot = false;
    public bool freezeYRot = false;
    public bool freezeZRot = false;
}
[Serializable]
public class SerializableBoxCollider : SerializableComponent
{
    public string name;
    public string type;
    public bool isTrigger;
    public Vector3 center = Vector3.zero;
    public Vector3 size = Vector3.one;
}
public class SerializableSphereCollider : SerializableComponent
{
    public string name;
    public string type;
    public bool isTrigger;
    public Vector3 center = Vector3.zero;
    public float radius = 0;
}



