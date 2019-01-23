using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Linq;



using System.Threading;

public class Engine : MonoBehaviour
{
    public GameObject particle;
    public GameObject world;
    public GameObject p0, p1, p2, p3, p4, p5, truckCenter;
    public Car truck;
    public List<GameObject> particles = new List<GameObject>();

    //static string sphDll = @"sph_e1ngine.dll";




    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void init();


    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void update();

    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void pause();

    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void play();


    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void get_particle(int i, ref float x, ref float y, ref float z);



    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void free_sph();


    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void change(float x0, float y0, float z0, float x1, float y1, float z1);


    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void set_paramas(float actual_particles_number, float capsule_radius, float gas_constant, float gravity_x, float gravity_y, float gravity_z,
                    float kernel, float mass, float rest_desnity, float surf_coe,
                    float surf_norm, float time_step, float viscosity, float cT, bool walls);

    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void re_init();
    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void set_points(float p0_x, float p0_y, float p0_z, float p1_x, float p1_y, float p1_z,
        float p2_x, float p2_y, float p2_z, float p3_x, float p3_y, float p3_z, float p4_x, float p4_y, float p4_z
        , float p5_x, float p5_y, float p5_z);


    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void truck_acc(float x, float y, float z);


    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void set_target(float x, float y, float z);


    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void set_dw(float y);


    [DllImport(@"D:\Temp\VS2012\ConsoleApplication9\x64\Release\sph_e1.dll", CallingConvention = CallingConvention.Cdecl)]
    public static extern void set_w(float y);
    public ScreenshotMovie screenShot; 

    public class param
    {

        public Vector3 gravity;

        public float rest_density;
        public float gas_constant;
        public float viscosity;
        public float time_step;
        public float surf_norm;
        public float surf_coe;
        public uint actual_particles_number;
        public float kernel;
        public float mass;
        public float capsule_radius;
        public float cT;
        public bool walls;
        public param()
        {

            capsule_radius = 0.1f;

            actual_particles_number = 1000;
            kernel = 0.028f;
            mass = 0.02f;

            gravity.x = 0.0f;
            gravity.y = -9.8f;
            gravity.z = 0.0f;
            rest_density = 998.29f;
            gas_constant = 0.4f; // gass stifness 
            viscosity = 10.5f;
            time_step = 0.003f;
            surf_norm = 7.065f; // Threshold 
            surf_coe = 0.0728f;
            cT = 0;
            walls = true;
        }

    };
    public param paramas = new param();

    public GameObject walls;

    void ReCreateParticles()
    {





        for (int i = 0; i < particles.Count; i++)
            Destroy(particles[i], 0);

        particles.Clear();

        /*Vector3 scaler = world.transform.localScale;
        world.transform.localScale = new Vector3(1, 1, 1); 
        
       */

        for (int i = 0; i < paramas.actual_particles_number; i++)
        {
            
            float x = 0, y = 0, z = 0;
            GameObject t = Instantiate(particle, world.transform);

            get_particle(i, ref x, ref y, ref z);
            t.transform.position = new Vector3(x, y, z);
            t.name = "Particle " + i;
            particles.Add(t);

        }
       /* world.transform.localScale = scaler;*/


    }
    void Start()
    {

        init();

       /* Vector3 scaler = world.transform.localScale;
        world.transform.localScale = new  Vector3(1, 1, 1); */
        for (int i = 0; i < paramas.actual_particles_number; i++)
        {

            float x = 0, y = 0, z = 0;
            GameObject t = Instantiate(particle, world.transform);
            
            get_particle(i, ref x, ref y, ref z);

            t.transform.localPosition = new Vector3(x, y, z);

            t.name = "Particle " + i;

            particles.Add(t);

        }
        /*world.transform.localScale = scaler;*/
        play();
    }


    // Update is called once per frame

    void UpdatePoints()
    {
        set_points(p0.transform.position.x, p0.transform.position.y, p0.transform.position.z,
                    p1.transform.position.x, p1.transform.position.y, p1.transform.position.z,
                    p2.transform.position.x, p2.transform.position.y, p2.transform.position.z,
                    p3.transform.position.x, p3.transform.position.y, p3.transform.position.z,
                    p4.transform.position.x, p4.transform.position.y, p4.transform.position.z,
                    p5.transform.position.x, p5.transform.position.y, p5.transform.position.z);



    }
    void Update()
    {

        if (Input.GetKeyDown(KeyCode.BackQuote))
            sphOptionWindowIsActivated = !sphOptionWindowIsActivated;

        // UpdatePoints();

        update();
        Vector3 seg = new Vector3(0, 0, 0);
        for (int i = 0; i < paramas.actual_particles_number; i++)
        {

            float x = 0, y = 0, z = 0;
            GameObject t = particles[i];

            get_particle(i, ref x, ref y, ref z);


            t.transform.localPosition = new Vector3(x, y, z);
          



            seg += t.transform.position;



        }
        truck.centerOfGravity = (seg / paramas.actual_particles_number) * paramas.mass * paramas.actual_particles_number + truck.Mass * truckCenter.transform.position;
        truck.centerOfGravity /= (truck.Mass + paramas.mass * paramas.actual_particles_number);




    }

    private Rect sphRect = new Rect(20, 20, 500, 400);
    bool sphOptionWindowIsActivated = false;
    void OnGUI()
    {
        if (sphOptionWindowIsActivated)
            sphRect = GUI.Window(0, sphRect, sphWindow, "sph Options ");
    }


    void Updatesph(bool reinitialize)
    {




        set_paramas(paramas.actual_particles_number, paramas.capsule_radius, paramas.gas_constant, paramas.gravity.x,
            paramas.gravity.y, paramas.gravity.z, paramas.kernel, paramas.mass,
           paramas.rest_density, paramas.surf_coe, paramas.surf_norm, paramas.time_step, paramas.viscosity, paramas.cT, paramas.walls);

        if (reinitialize)
        {
            re_init();
            ReCreateParticles();
        }





    }

    void sphWindow(int windowID)
    {



        bool reinitialize = false;





        // Time step 
        paramas.time_step = GUI.HorizontalScrollbar(new Rect(10, 60, 200, 20), paramas.time_step, 0.0001f, 0.0f, 0.1f);
        GUI.Label(new Rect(10, 40, 200, 20), "Time Step: " + paramas.time_step.ToString());

        //Viscosity 
        paramas.viscosity = GUI.HorizontalScrollbar(new Rect(10, 100, 200, 20), paramas.viscosity, 0.1f, 1.0f, 100.0f);
        GUI.Label(new Rect(10, 80, 200, 20), "Viscosity : " + paramas.viscosity.ToString());

        // Threshold

        paramas.surf_norm = GUI.HorizontalScrollbar(new Rect(10, 140, 200, 20), paramas.surf_norm, 0.1f, 1.0f, 100.0f);
        GUI.Label(new Rect(10, 120, 200, 20), "Threshold(Surf Norm) : " + paramas.surf_norm.ToString());

        // Surf coe 

        paramas.surf_coe = GUI.HorizontalScrollbar(new Rect(10, 180, 200, 20), paramas.surf_coe, 0.001f, 0.0001f, 20.0f);
        GUI.Label(new Rect(10, 160, 200, 20), "Surf Coe : " + paramas.surf_coe.ToString());

        // Rest Density 

        paramas.rest_density = GUI.HorizontalScrollbar(new Rect(10, 220, 200, 20), paramas.rest_density, 0.1f, 0.1f, 10000.0f);
        GUI.Label(new Rect(10, 200, 200, 20), "Rest Density: " + paramas.rest_density.ToString());

        // Mass 

        paramas.mass = GUI.HorizontalScrollbar(new Rect(10, 260, 200, 20), paramas.mass, 0.01f, 0.001f, 1.0f);
        GUI.Label(new Rect(10, 240, 200, 20), "Mass: " + paramas.mass.ToString());

        // Kernel 
        float kernel_t = paramas.kernel;
        paramas.kernel = GUI.HorizontalScrollbar(new Rect(10, 300, 200, 20), paramas.kernel, 0.01f, 0.0001f, 0.1f);
        GUI.Label(new Rect(10, 280, 200, 20), "Kernel: " + paramas.kernel.ToString());

        if (kernel_t != paramas.kernel) reinitialize = true;

        // Gas Constant 

        paramas.gas_constant = GUI.HorizontalScrollbar(new Rect(10, 340, 200, 20), paramas.gas_constant, 0.01f, 0.01f, 100.0f);
        GUI.Label(new Rect(10, 320, 200, 20), "Gas Constant : " + paramas.gas_constant.ToString());

        //Play 


        if (GUI.Button(new Rect(10, 360, 50, 20), "Play")) play();

        // Pause 
        if (GUI.Button(new Rect(70, 360, 50, 20), "Pause")) pause();


        // Default 
        if (GUI.Button(new Rect(130, 360, 50, 20), "Default"))
        {
            reinitialize = true;
            paramas = new param();

        }

        // Gravity 
        paramas.gravity.y = GUI.HorizontalScrollbar(new Rect(220, 60, 200, 20), paramas.gravity.y, 0.01f, -100.0f, 100.0f);
        GUI.Label(new Rect(220, 40, 200, 20), "Gravity : " + paramas.gravity.y.ToString());


        // Particles Number 


        uint particlesNumberPre = paramas.actual_particles_number;
        paramas.actual_particles_number = (uint)GUI.HorizontalScrollbar(new Rect(220, 100, 200, 20), paramas.actual_particles_number, 1f, 1f, 12501.0f);
        GUI.Label(new Rect(220, 80, 200, 20), "Particles Number : " + paramas.actual_particles_number.ToString());

        if (particlesNumberPre != paramas.actual_particles_number) reinitialize = true;



        paramas.walls = GUI.Toggle(new Rect(220, 140, 200, 20), paramas.walls, "Walls");


        // cT 
        paramas.cT = GUI.HorizontalScrollbar(new Rect(220, 180, 200, 20), paramas.cT, 0.000001f, 0.0f, 1.0f);
        GUI.Label(new Rect(220, 160, 200, 20), "Collision Constant : " + paramas.cT);



        screenShot.capture = GUI.Toggle(new Rect(220, 120, 200, 20), screenShot.capture, "Screen Capture");






        if (!GUI.changed) return;

        Updatesph(reinitialize);


        if (paramas.walls)
            walls.transform.localPosition = Vector3.zero;
        else
            walls.transform.position = Vector3.one * -10;








    }

    void OnApplicationQuit()
    {
        free_sph();
    }


}
