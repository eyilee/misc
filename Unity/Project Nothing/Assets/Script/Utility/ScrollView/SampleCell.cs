using UnityEngine;
using UnityEngine.UI;

public sealed class SampleCell : ScrollViewCell
{
    private string m_Name;
    private Text m_NameText;

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
}
