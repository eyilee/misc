using UnityEngine;

namespace ProjectNothing.Utility
{
    public sealed class SampleCell : ScrollViewCell
    {
        public override void Init (GameObject gameObject)
        {
            base.Init (gameObject);
        }

        public override void SetView ()
        {
            base.SetView ();

            m_GameObject.name = "Sampe Cell";
        }
    }
}
