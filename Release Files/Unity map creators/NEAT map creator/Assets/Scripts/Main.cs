using System.IO;
using UnityEngine;
using UnityEditor;
using UnityEngine.SceneManagement;

public class Main : MonoBehaviour
{
    private bool isPlaying;
    private string path;

    private void Start()
    {
        isPlaying = true;

        if (isPlaying)
        {
            path = EditorUtility.SaveFilePanel("Save map", "", "new_map", "csv");

            FileStream fout = new FileStream(path, FileMode.Create);
            using (StreamWriter writer = new StreamWriter(fout))
            {
                writer.WriteLine("number-of-objects:;" + SceneManager.GetActiveScene().GetRootGameObjects().Length);
                for (int i = 0; i < SceneManager.GetActiveScene().GetRootGameObjects().Length; i++)
                {
                    writer.WriteLine();
                    writer.WriteLine(SceneManager.GetActiveScene().GetRootGameObjects()[i].name);

                    writer.Write("position:;");
                    writer.Write(SceneManager.GetActiveScene().GetRootGameObjects()[i].transform.position.x.ToString().Replace(',','.'));
                    writer.Write(';');
                    writer.Write(SceneManager.GetActiveScene().GetRootGameObjects()[i].transform.position.y.ToString().Replace(',', '.'));
                    writer.Write(';');
                    writer.WriteLine(SceneManager.GetActiveScene().GetRootGameObjects()[i].transform.position.z.ToString().Replace(',', '.'));

                    writer.Write("rotation:;");
                    writer.Write(SceneManager.GetActiveScene().GetRootGameObjects()[i].transform.rotation.w.ToString().Replace(',', '.'));
                    writer.Write(';');
                    writer.Write(SceneManager.GetActiveScene().GetRootGameObjects()[i].transform.rotation.x.ToString().Replace(',', '.'));
                    writer.Write(';');
                    writer.Write(SceneManager.GetActiveScene().GetRootGameObjects()[i].transform.rotation.y.ToString().Replace(',', '.'));
                    writer.Write(';');
                    writer.WriteLine(SceneManager.GetActiveScene().GetRootGameObjects()[i].transform.rotation.z.ToString().Replace(',', '.'));

                    writer.Write("localScale:;");
                    writer.Write(SceneManager.GetActiveScene().GetRootGameObjects()[i].transform.localScale.x.ToString().Replace(',', '.'));
                    writer.Write(';');
                    writer.Write(SceneManager.GetActiveScene().GetRootGameObjects()[i].transform.localScale.y.ToString().Replace(',', '.'));
                    writer.Write(';');
                    writer.WriteLine(SceneManager.GetActiveScene().GetRootGameObjects()[i].transform.localScale.z.ToString().Replace(',', '.'));
                }
            }
            fout.Close();

            Application.Quit();
            isPlaying = false;
        }
    }
}
