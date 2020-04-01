using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Test : MonoBehaviour
{
    private XScrollRect m_XScrollRect;

    private readonly List<string> m_Names = new List<string> (999);

    public void Awake ()
    {
        m_XScrollRect = gameObject.transform.Find ("XScrollView").GetComponent<XScrollRect> ();
    }

    public void Start ()
    {
        for (int i = 0; i < 1000; i++)
        {
            m_Names.Add (i.ToString ());
        }

        m_XScrollRect.Init<SampleCell> (m_Names.Count);
        m_XScrollRect.RegisterDataSetter (SetData);
        m_XScrollRect.SetData ();
    }

    public void SetData (int index, ScrollViewCell scrollViewCell)
    {
        SampleCell sampleCell = scrollViewCell as SampleCell;

        if (index >= 0 && index <= m_Names.Count - 1)
        {
            sampleCell.SetData (m_Names[index]);
        }
    }
}
