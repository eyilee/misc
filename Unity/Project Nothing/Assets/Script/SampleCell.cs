using UnityEngine;
using UnityEngine.UI;

public sealed class SampleCell : ScrollViewCell
{
    string m_Name;

    Text m_NameText;

    public override void Init (GameObject gameObject)
    {
        base.Init (gameObject);

        m_NameText = gameObject.GetComponent<Text> ();
    }

    public override void SetView ()
    {
        base.SetView ();

        m_NameText.text = m_Name;
    }

    public void SetData (string text)
    {
        m_Name = text;

        SetView ();
    }
}
