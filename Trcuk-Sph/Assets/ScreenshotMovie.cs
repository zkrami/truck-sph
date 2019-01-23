using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Threading;
using UnityEngine;

public class ScreenshotMovie : MonoBehaviour
{
    // The folder we place all screenshots inside.
    // If the folder exists we will append numbers to create an empty folder.
    public string folder = "ScreenshotMovieOutput";
    public int frameRate = 25;
    public int sizeMultiplier = 1;
    public bool capture; 
    private string realFolder = "";

    void Start()
    {
        
        // Set the playback framerate!
        // (real time doesn't influence time anymore)
        Time.captureFramerate = frameRate;

        // Find a folder that doesn't exist yet by appending numbers!
        realFolder = folder;
        int count = 1;
        while (System.IO.Directory.Exists(realFolder))
        {
            realFolder = folder + count;
            count++;
        }
        // Create the folder
        System.IO.Directory.CreateDirectory(realFolder);
    }

    void Update()
    {

        if (capture)
        {

            var name = string.Format("{0}/shot {1:D04}.jpg", realFolder, Time.frameCount);


            
            RenderTexture rt = new RenderTexture(Screen.width, Screen.height, 24, RenderTextureFormat.ARGB32);
            rt.useMipMap = false;
            rt.antiAliasing = 1;
            RenderTexture.active = rt;
            Camera.main.targetTexture = rt;


            Texture2D shot = new Texture2D(Screen.width, Screen.height, TextureFormat.ARGB32, false);


            Camera.main.Render();

            shot.ReadPixels(new Rect(0, 0, Screen.width, Screen.height), 0, 0, false);
            shot.Apply();

            Camera.main.targetTexture = null;
            RenderTexture.active = null;
            Destroy(rt);
            byte[] bytes = shot.EncodeToJPG(100);

            new Thread(() =>
            {               
                File.WriteAllBytes(name, bytes);
            }).Start();
        }
    }
}