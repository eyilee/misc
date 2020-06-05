using UnityEngine;
using UnityEngine.UI;

public class Test : MonoBehaviour
{
    private Image m_Image;

    private float m_Rx = 0f;
    private readonly float m_Ry = 0f;

    private readonly float m_Gx = 0f;
    private float m_Gy = 0f;

    private float m_Bx = 0f;
    private float m_By = 0f;

    void Awake ()
    {
        m_Image = GetComponent<Image> ();
    }

    void Update ()
    {
        float delta = Time.deltaTime / 10f;

        byte r = (byte)(255 * Mathf.PerlinNoise (m_Rx, m_Ry));
        m_Rx += delta;

        byte g = (byte)(255 * Mathf.PerlinNoise (m_Gx, m_Gy));
        m_Gy += delta;

        byte b = (byte)(255 * Mathf.PerlinNoise (m_Bx, m_By));
        m_Bx += delta;
        m_By += delta;

        m_Image.color = new Color32 (r, g, b, 255);
    }
}
