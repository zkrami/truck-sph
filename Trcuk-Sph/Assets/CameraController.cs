using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour {

	// Use this for initialization
	void Start () {
        for (int i = 0; i < cameras.Count; i++) cameras[i].enabled = false;
        cameras[0].enabled = true; 
	}


    public List<Camera> cameras = new List<Camera>(); 
     
     int camIndex;
	
	// Update is called once per frame
     
	void Update () {
		
        if (Input.GetKeyDown(KeyCode.V))
         {
             camIndex++;
             camIndex %= cameras.Count;
             for (int i = 0; i < cameras.Count; i++)
             {
                 if (i != camIndex)
                 {
                     cameras[i].enabled = false;
                 }
                 else
                 {
                     cameras[i].enabled = true; 
                 }
             }

         }
      
     }
	}

