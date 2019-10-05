using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace UnityEngine.UI
{
    [RequireComponent (typeof (ScrollRect))]
    public class ScrollView : MonoBehaviour
    {
        [SerializeField]
        private ScrollRect m_ScrollRect;
        public ScrollRect ScrollRect { get { return m_ScrollRect; } set { m_ScrollRect = value; } }

        [SerializeField]
        private RectTransform m_Content;
        public RectTransform Content { get { return m_Content; } set { m_Content = value; } }

        [SerializeField]
        private GameObject m_Item;
        public GameObject Item { get { return m_Item; } set { m_Item = value; } }

        [SerializeField]
        private int m_Visible;
        public int Visible { get { return m_Visible; } set { m_Visible = value; } }

        [SerializeField]
        private int m_Size;
        public int Size { get { return m_Size; } set { m_Size = value; } }

        [Space]
        [SerializeField]
        private bool m_Horizontal = true;
        public bool Horizontal { get { return m_Horizontal; } set { m_Horizontal = value; } }

        [SerializeField]
        private float m_HorizontalGap;
        public float HorizontalGap { get { return m_HorizontalGap; } set { m_HorizontalGap = value; } }

        [Space]
        [SerializeField]
        private bool m_Vertical = true;
        public bool Vertical { get { return m_Vertical; } set { m_Vertical = value; } }

        [SerializeField]
        private float m_VerticalGap;
        public float VerticalGap { get { return m_VerticalGap; } set { m_VerticalGap = value; } }

        private int m_HorizontalIndex = 0;
        private int m_VerticalIndex = 0;

        private float m_UnitWidth = 0.0f;
        private float m_UnitHeight = 0.0f;

        private readonly List<GameObject> m_Items = new List<GameObject> ();

        private void Awake ()
        {
            RectTransform rectTransform = m_Item.GetComponent<RectTransform> ();

            m_UnitWidth = rectTransform.rect.width;
            m_UnitHeight = rectTransform.rect.height;

            for (int i = 0; i < m_Visible + 1; i++)
            {
                float x = 0.0f;
                float y = 0.0f;
                float z = 0.0f;

                if (m_Horizontal)
                {
                    x = (m_UnitWidth + m_HorizontalGap) * i;
                    y = -m_UnitHeight;
                }

                if (m_Vertical)
                {
                    y = -(m_UnitHeight + m_VerticalGap) * i - m_UnitHeight;
                }

                GameObject item = Instantiate (m_Item, m_Content.transform.position + new Vector3 (x, y, z), Quaternion.identity, m_Content.transform);
                m_Items.Add (item);
            }

            float contentWidth = m_Content.rect.width;
            float contentHeight = m_Content.rect.height;

            if (m_Horizontal)
            {
                contentWidth = (m_UnitWidth + m_HorizontalGap) * m_Size - m_HorizontalGap;
            }

            if (m_Vertical)
            {
                contentHeight = (m_UnitHeight + m_VerticalGap) * m_Size - m_VerticalGap;
            }

            m_Content.sizeDelta = new Vector2 (contentWidth, contentHeight);
        }

        private void OnEnable ()
        {
            if (m_Horizontal)
            {
                m_ScrollRect.onValueChanged.AddListener (SetHorizontalPosition);
            }

            if (m_Vertical)
            {
                m_ScrollRect.onValueChanged.AddListener (SetVerticalPosition);
            }
        }

        private void OnDisable ()
        {
            if (m_Horizontal)
            {
                m_ScrollRect.onValueChanged.RemoveListener (SetHorizontalPosition);
            }

            if (m_Vertical)
            {
                m_ScrollRect.onValueChanged.RemoveListener (SetVerticalPosition);
            }
        }

        private void SetHorizontalPosition (Vector2 position)
        {
            int horizontalIndex = GetHorizontalIndex ();

            if (horizontalIndex != m_HorizontalIndex)
            {
                m_HorizontalIndex = horizontalIndex;

                UpdateItems ();
            }
        }

        private int GetHorizontalIndex ()
        {
            float horizontalScroll = Mathf.Abs (m_Content.anchoredPosition.x);

            if (horizontalScroll <= 0)
            {
                return 0;
            }
            else
            {
                return Mathf.FloorToInt (horizontalScroll / (m_UnitWidth + m_HorizontalGap));
            }
        }

        private void SetVerticalPosition (Vector2 position)
        {
            int verticalIndex = GetVerticalIndex ();

            Debug.Log (position);

            if (verticalIndex != m_VerticalIndex)
            {
                m_VerticalIndex = verticalIndex;

                UpdateItems ();
            }
        }

        private int GetVerticalIndex ()
        {
            float verticalScroll = Mathf.Abs (m_Content.anchoredPosition.y);

            if (verticalScroll <= 0)
            {
                return 0;
            }
            else
            {
                return Mathf.FloorToInt (verticalScroll / (m_UnitHeight + m_VerticalGap));
            }
        }

        private void UpdateItems ()
        {
            for (int i = 0; i < m_Items.Count; i++)
            {
                float x = 0.0f;
                float y = 0.0f;
                float z = 0.0f;

                if (m_Horizontal)
                {
                    x = (m_UnitWidth + m_HorizontalGap) * (m_VerticalIndex + i);
                    y = -m_UnitHeight;
                }

                if (m_Vertical)
                {
                    y = -(m_UnitHeight + m_VerticalGap) * (m_VerticalIndex + i) - m_UnitHeight;
                }

                m_Items[i].transform.localPosition = new Vector3 (x, y, z);
            }
        }
    }
}
