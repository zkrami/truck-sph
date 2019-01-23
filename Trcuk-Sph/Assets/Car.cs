using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Car : MonoBehaviour
{

    // Use this for initialization


    Vector3 speed = new Vector3(0, 0, 0), force = new Vector3(0, 0, 0);

    public Engine engine;
    public float Mass = 80f;
    float teta = 0;
    float R = 10000000f;
    public float rollingResistence = 0.009f * 8000f;   // Cr * Mass
    float h = 0.059f;    // height of center
    float g = 9.8f;
    float width;   // dist between front wheels


    float I = 80 * 3f;

    Vector3 pivot, pivot2;
    public GameObject c0, c1, c3, c4;
    public Vector3 centerOfGravity = new Vector3(0, 0, 0);

    public GameObject world;


    float kotta(float t, float y)
    {

        float H = engine.paramas.time_step / 10f;

        for (int i = 0; i < 10; i++, t += H)
        {
            float k1 = ff(t, y);
            float k2 = ff(t + H / 2, y + (1f / 2) * k1);
            float k3 = ff(t + H / 2, y + (1f / 2) * k2);
            float k4 = ff(t + H, y + H * k3);

            y += H * (k1 + 2 * k2 + 2 * k3 + k4);

        }
        return y;
    }
    float d; 
    float ff(float t, float y)
    {
        float W = Mass * g;
        float Fc = Mass * speed.magnitude * speed.magnitude / R;

        float ret = Fc * Mathf.Sin(y * Mathf.Deg2Rad) * d;
        ret += Fc * Mathf.Cos(y * Mathf.Deg2Rad) * h;

        ret -= W * Mathf.Cos(y * Mathf.Deg2Rad) * d;

        ret += W * Mathf.Sin(y * Mathf.Deg2Rad) * h;

        ret *= t;

        return ret / I;
    }


    void Start()
    {


        width = (c0.transform.position - c3.transform.position).magnitude;
        




    }

    // Update is called once per frame

    
    bool myTeta = false;
    float angle = 0;


    void DrawLine(Vector3 start, Vector3 end, Color color, float duration = 0.2f)
    {
        GameObject myLine = new GameObject();
        myLine.transform.position = start;
        myLine.AddComponent<LineRenderer>();

        LineRenderer lr = myLine.GetComponent<LineRenderer>();
        lr.material = new Material(Shader.Find("Particles/Alpha Blended Premultiply"));
        lr.startColor = color;
        lr.endColor = color;
        lr.startWidth = 0.1f;
        lr.endWidth = 0.1f;
        lr.SetPosition(0, start);
        lr.SetPosition(1, end);
        GameObject.Destroy(myLine, duration);

    }

    public static float DistanceToLine(Ray ray, Vector3 point)
    {
        return Vector3.Cross(ray.direction, point - ray.origin).magnitude;
    }
    Vector3 RotateVector3Around(Vector3 target, Vector3 axis, float angle, Vector3 point)
    {
        GameObject t = new GameObject();

        t.transform.position = point;


        t.transform.RotateAround(target, axis, angle);
        return t.transform.position;

    }

    float myKotta = 0;
    bool ok = false;
    float all = 0;
    public Vector3 acc = new Vector3(0, 0, 0);
    public GameObject sphere;
    public float breakingFactor;
    public float engine_force; 
    float prev_w;

    public float scaller_h;
    public float scaller_d;
    void Update()
    {
        sphere.transform.position = centerOfGravity;
        h = new Plane(c0.transform.position, c3.transform.position, c1.transform.position).GetDistanceToPoint(centerOfGravity) * scaller_h; 
        
        if (Mathf.Abs(angle) >= 90f)
        {
            Engine.truck_acc(0, -engine.paramas.gravity.y, engine.paramas.gravity.y * Mathf.Sign(teta) * -10); 
            return;
        }
        pivot = c1.transform.position - c0.transform.position;
        pivot2 = c3.transform.position - c4.transform.position;

        d = width/2; 
       
        if (ok == true)
        {

            Plane plane2 = new Plane(c4.transform.position - c1.transform.position, c3.transform.position); // right
            Plane plane1 = new Plane(c4.transform.position - c1.transform.position, c0.transform.position); //left 


            float prev = myKotta;
            myKotta = kotta(all, myKotta);
            angle += myKotta - prev;
            all += 0.02f;

            if (myTeta == true)
            {

                d = plane1.GetDistanceToPoint(centerOfGravity);
                world.transform.RotateAround(c0.transform.position, pivot, -myKotta + prev);
            }
            else
            {
                d = plane2.GetDistanceToPoint(centerOfGravity); 
                world.transform.RotateAround(c3.transform.position, pivot2, -myKotta + prev);

            }
                





        }


        d *= scaller_d; 

        float comp = Mathf.Sqrt((d * g * R) / ( h));


        if (speed.magnitude >= comp && ok == false)
        {
            ok = true;

            all += 0.02f;

            myTeta = teta > 0;

        }

        force -= speed.normalized * rollingResistence;    // RollingResistence force


        if (Input.GetKey(KeyCode.Space))
        {
       
            force -= speed.normalized * breakingFactor; 
        }


        Vector3 unit = (c0.transform.position - c1.transform.position).normalized;

        
        if (Input.GetKey(KeyCode.W))
        {
            force += unit * engine_force;
        }
        else if (Input.GetKey(KeyCode.S))
        {
            force -= unit * engine_force;
        }

        if (Input.GetKey(KeyCode.D))
        {
            teta += 1f;
            teta = Mathf.Min(teta, 25f);
        }
        else if (Input.GetKey(KeyCode.A))
        {
            teta -= 1f;
            teta = Mathf.Max(teta, -25f);
        }
        else if (teta != 0)
        {
            teta += -1f * Mathf.Sign(teta);

        }

        speed += (force / Mass) * engine.paramas.time_step;

        acc = force / Mass;





        if (teta != 0)
        {

            float t = teta > 0 ? 90 - teta : -90 - teta;


            Vector3 target = new Vector3(c0.transform.position.x, c0.transform.position.y, c0.transform.position.z);


            target = RotateVector3Around(c1.transform.position, Vector3.up, t, target);


            float l = (c1.transform.position - c0.transform.position).magnitude;

            float r = l / Mathf.Abs(Mathf.Cos(t * Mathf.Deg2Rad));
            R = r;

            target = (target - c1.transform.position) * (r / l) + c1.transform.position;

            float w = speed.magnitude / r;

            float sign = Mathf.Sign(Mathf.Cos(Vector3.Angle(speed.normalized, unit.normalized) * Mathf.Deg2Rad));
            w *= sign;

            float beta = w * engine.paramas.time_step * Mathf.Sign(teta) * Mathf.Rad2Deg;

            speed = RotateVector3Around(Vector3.zero, Vector3.up, beta, speed);
            acc = RotateVector3Around(Vector3.zero, Vector3.up, beta, acc);

            world.transform.RotateAround(target, Vector3.up, beta);

            Engine.set_w(w);

          
             Vector3 rr = target - world.transform.position;
             Vector3 wr = Vector3.up * w;
             Vector3 p = Vector3.Cross(wr, Vector3.Cross(wr, rr));
             p = world.transform.TransformVector(p);
            Engine.truck_acc(p.x, p.y, p.z);

        }
        else
        {
            world.transform.position += speed * engine.paramas.time_step;
            prev_w = 0;
            R = 1000000f;

            Engine.set_w(0);
            acc = world.transform.TransformVector(acc);
            Engine.truck_acc(-acc.x, -acc.y, -acc.z);
        }

        force = new Vector3(0, 0, 0);

        
    }


    private Rect carInfo = new Rect(20, 20, 250, 100);

    void OnGUI()
    {
          carInfo = GUI.Window(2, carInfo, carWindow, "Car Info");
    }
    public void PressKey(KeyCode key)
    {


        if ( key == KeyCode.Space) 
        {

            force -= speed.normalized * breakingFactor;
        }


        Vector3 unit = (c0.transform.position - c1.transform.position).normalized;


        if (key == KeyCode.W)
        {
            force += unit * engine_force;
        }
        else if (key == KeyCode.S)
        {
            force -= unit * engine_force;
        }

        if (key ==  KeyCode.D)
        {
            teta += 1f;
            teta = Mathf.Min(teta, 25f);
        }
        else if (key ==  KeyCode.A)
        {
            teta -= 1f;
            teta = Mathf.Max(teta, -25f);
        }
    }

    private void carWindow(int id)
    {
        
        GUI.Label(new Rect(20, 20, 220, 30), "Speed: " + speed.magnitude * 3.6 + " Km/H ");
        GUI.Label(new Rect(20, 40, 220, 30), "Time: " + Time.time + " s ");
    }
}
