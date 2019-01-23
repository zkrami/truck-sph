using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class scinario : MonoBehaviour {

	// Use this for initialization
    public Car truck;
	void Start () {
		
	}

    static bool TimeBetween(float s, float e)
    {
        return Time.time >= s && Time.time <= e; 
    }
	
	// Update is called once per frame
	void Update () {
       
	}
}
