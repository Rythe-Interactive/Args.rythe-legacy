using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;

[CreateAssetMenu(fileName = "new renderdat", menuName = "Create new RenderData")]
public class RenderData : ScriptableObject
{
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
