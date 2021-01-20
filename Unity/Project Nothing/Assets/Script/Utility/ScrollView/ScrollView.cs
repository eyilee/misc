using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

namespace ProjectNothing.Utility
{
    public class ScrollView : ScrollRect
    {
        public enum LayoutType
        {
            Horizontal,
            Vertical,
            Grid
        }

        [SerializeField]
        private LayoutType m_LayoutType = LayoutType.Horizontal;

        [SerializeField]
        private int m_Columns = 1;

        [SerializeField]
        private int m_Rows = 1;

        [SerializeField]
        private float m_SpaceX = 0;

        [SerializeField]
        private float m_SpaceY = 0;

        [SerializeField]
        private float m_Left = 0f;

        [SerializeField]
        private float m_Top = 0f;

        [SerializeField]
        private int m_PreviewSize = 0;

        [SerializeField]
        private RectTransform m_DefaultItem = null;

        private Type m_CellType;
        private readonly List<ScrollViewCell> m_Cells = new List<ScrollViewCell> ();
        private readonly List<ScrollViewCell> m_CellPool = new List<ScrollViewCell> ();

        private readonly Dictionary<int, ScrollViewObstacle> m_Obstacles = new Dictionary<int, ScrollViewObstacle> ();
        private readonly SortedDictionary<int, float> m_ObstacleGapMap = new SortedDictionary<int, float> (new ReverseComparer<int> ());

        // set data event.
        public delegate void ScrollViewCellDataHandler (int index, ScrollViewCell scrollViewCell);
        private event ScrollViewCellDataHandler OnSetData = (int index, ScrollViewCell scrollViewCell) => { };

        // current index.
        private int m_Index = 0;
        // capacity.
        private int m_Capacity = 0;
        // obstacle gap, only use for counting index.
        private float m_ObstacleGap = 0f;

        // right.
        private const int m_DirectionX = 1;
        // down.
        private const int m_DirectionY = -1;

        protected override void Start ()
        {
            base.Start ();

            onValueChanged.AddListener (OnScroll);
        }

        public void Init<T> (int capacity) where T : ScrollViewCell, new()
        {
            if (m_DefaultItem == null)
            {
                return;
            }

            m_DefaultItem.anchorMin = new Vector2 (0f, 1f);
            m_DefaultItem.anchorMax = new Vector2 (0f, 1f);
            m_DefaultItem.pivot = new Vector2 (0.5f, 0.5f);

            content.anchorMin = new Vector2 (0f, 1f);
            content.anchorMax = new Vector2 (0f, 1f);
            content.pivot = new Vector2 (0f, 1f);

            m_DefaultItem.gameObject.SetActive (false);

            m_CellType = typeof (T);

            ResizeContent (capacity);

            ScrollToTop ();
        }

        public void ResizeContent (int capacity)
        {
            if (capacity < 0)
            {
                capacity = 0;
            }

            if (m_Capacity == capacity)
            {
                SetObstaclesAnchoredPosition ();
                return;
            }

            m_Capacity = capacity;

            SetCells ();

            int rows = 1;
            int columns = 1;
            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    rows = 1;
                    columns = m_Capacity;
                    break;
                case LayoutType.Vertical:
                    rows = m_Capacity;
                    columns = 1;
                    break;
                case LayoutType.Grid:
                    rows = (m_Capacity % m_Columns) == 0 ? m_Capacity / m_Columns : m_Capacity / m_Columns + 1;
                    columns = m_Columns;
                    break;
                default:
                    break;
            }

            float paddingLeft = m_Left;
            float width = columns * m_DefaultItem.rect.width;
            float gapX = (columns - 1) * m_SpaceX;
            float paddingTop = m_Top;
            float height = rows * m_DefaultItem.rect.height;
            float gapY = (rows - 1) * m_SpaceY;

            float obstacleGapX = 0f;
            float obstacleGapY = 0f;
            foreach (KeyValuePair<int, ScrollViewObstacle> item in m_Obstacles)
            {
                ScrollViewObstacle scrollViewObstacle = item.Value;
                if (item.Key < m_Capacity)
                {
                    switch (m_LayoutType)
                    {
                        case LayoutType.Horizontal:
                            obstacleGapX += scrollViewObstacle.Width;
                            break;
                        case LayoutType.Vertical:
                            obstacleGapY += scrollViewObstacle.Height;
                            break;
                        case LayoutType.Grid:
                            // Not support.
                            break;
                        default:
                            break;
                    }
                }
            }

            content.sizeDelta = new Vector2 (paddingLeft + width + gapX + obstacleGapX, paddingTop + height + gapY + obstacleGapY);

            m_Index = 0;
            m_ObstacleGap = 0;

            SetObstaclesAnchoredPosition ();
            SetCellsAnchoredPosition ();
        }

        private void SetCells ()
        {
            int count = 0;
            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    count = m_Columns + 1;
                    break;
                case LayoutType.Vertical:
                    count = m_Rows + 1;
                    break;
                case LayoutType.Grid:
                    count = (m_Rows + 1) * m_Columns;
                    break;
                default:
                    break;
            }

            if (count > m_Capacity)
            {
                count = m_Capacity;
            }

            if (count < m_Cells.Count)
            {
                PushCellToPool (m_Cells.Count - count);
            }
            else if (count > m_Cells.Count)
            {
                PopCellFromPool (count - m_Cells.Count);
            }
        }

        private void SetCellsAnchoredPosition ()
        {
            for (int i = 0; i < m_Cells.Count; i++)
            {
                SetCellAnchoredPosition (i);
            }
        }

        private void SetCellAnchoredPosition (int index)
        {
            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    {
                        float paddingLeft = m_Left;
                        float width = (m_Index + index) * m_DefaultItem.rect.width;
                        float gap = (m_Index + index) * m_SpaceX;
                        float obstacleGap = GetObstacleGap (m_Index + index);
                        float translateX = m_DefaultItem.rect.width * 0.5f;
                        m_Cells[index].SetAnchoredPositionX ((paddingLeft + width + gap + obstacleGap + translateX) * m_DirectionX);
                    }
                    break;
                case LayoutType.Vertical:
                    {
                        float paddingTop = m_Top;
                        float height = (m_Index + index) * m_DefaultItem.rect.height;
                        float gap = (m_Index + index) * m_SpaceY;
                        float obstacleGap = GetObstacleGap (m_Index + index);
                        float translateY = m_DefaultItem.rect.height * 0.5f;
                        m_Cells[index].SetAnchoredPositionY ((paddingTop + height + gap + obstacleGap + translateY) * m_DirectionY);
                    }
                    break;
                case LayoutType.Grid:
                    {
                        int row = (m_Index + index) / m_Columns;
                        int column = (m_Index + index) % m_Columns;

                        float paddingLeft = m_Left;
                        float width = column * m_DefaultItem.rect.width;
                        float gapX = column * m_SpaceX;
                        float translateX = m_DefaultItem.rect.width * 0.5f;
                        float paddingTop = m_Top;
                        float height = row * m_DefaultItem.rect.height;
                        float gapY = row * m_SpaceY;
                        float translateY = m_DefaultItem.rect.height * 0.5f;
                        Vector2 position = new Vector2 ((paddingLeft + width + gapX + translateX) * m_DirectionX, (paddingTop + height + gapY + translateY) * m_DirectionY);
                        m_Cells[index].SetAnchoredPosition (position);
                    }
                    break;
                default:
                    break;
            }
        }

        private void SetObstaclesAnchoredPosition ()
        {
            m_ObstacleGapMap.Clear ();

            float obstacleGap = 0f;
            foreach (KeyValuePair<int, ScrollViewObstacle> item in m_Obstacles)
            {
                int index = item.Key;
                ScrollViewObstacle scrollViewObstacle = item.Value;

                if (index < m_Capacity)
                {
                    scrollViewObstacle.Wakeup ();
                }
                else
                {
                    scrollViewObstacle.Sleep ();
                }

                switch (m_LayoutType)
                {
                    case LayoutType.Horizontal:
                        {
                            float paddingLeft = m_Left;
                            float width = index * m_DefaultItem.rect.width;
                            float gap = index * m_SpaceX;
                            float translateX = scrollViewObstacle.Width * 0.5f;
                            scrollViewObstacle.SetAnchoredPositionX ((paddingLeft + width + gap + obstacleGap + translateX) * m_DirectionX);
                            obstacleGap += scrollViewObstacle.Width;
                            m_ObstacleGapMap.Add (index, obstacleGap);
                        }
                        break;
                    case LayoutType.Vertical:
                        {
                            float paddingTop = m_Top;
                            float height = index * m_DefaultItem.rect.height;
                            float gap = index * m_SpaceY;
                            float translateY = scrollViewObstacle.Height * 0.5f;
                            scrollViewObstacle.SetAnchoredPositionY ((paddingTop + height + gap + obstacleGap + translateY) * m_DirectionY);
                            obstacleGap += scrollViewObstacle.Height;
                            m_ObstacleGapMap.Add (index, obstacleGap);
                        }
                        break;
                    case LayoutType.Grid:
                        // Not support.
                        break;
                    default:
                        break;
                }
            }
        }

        private void OnScroll (Vector2 position)
        {
            int row = 0;
            int column = 0;
            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    column = Mathf.FloorToInt ((-content.anchoredPosition.x + m_Left + m_ObstacleGap) * m_DirectionX / (m_DefaultItem.rect.width + m_SpaceX));
                    break;
                case LayoutType.Vertical:
                    row = Mathf.FloorToInt ((-content.anchoredPosition.y + m_Top + m_ObstacleGap) * m_DirectionY / (m_DefaultItem.rect.height + m_SpaceY));
                    break;
                case LayoutType.Grid:
                    row = Mathf.FloorToInt (((-content.anchoredPosition.y + m_Top) * m_DirectionY + m_SpaceY) / (m_DefaultItem.rect.height + m_SpaceY));
                    column = Mathf.FloorToInt (((-content.anchoredPosition.x + m_Left) * m_DirectionX + m_SpaceX) / (m_DefaultItem.rect.width + m_SpaceX));
                    break;
                default:
                    break;
            }

            int index = 0;
            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    index = column;
                    break;
                case LayoutType.Vertical:
                    index = row;
                    break;
                case LayoutType.Grid:
                    index = row * m_Columns + column;
                    break;
                default:
                    break;
            }

            if (index < 0)
            {
                index = 0;
            }

            if (m_Index != index)
            {
                if (m_Cells.Count > 0)
                {
                    int count = Math.Abs (m_Index - index);
                    if (count > m_Cells.Count)
                    {
                        count = m_Cells.Count;
                    }

                    if (index > m_Index)
                    {
                        m_Index = index;
                        MoveCellsToBack (count);
                    }
                    else
                    {
                        m_Index = index;
                        MoveCellsToFront (count);
                    }
                }

                if (m_Obstacles.Count > 0)
                {
                    m_ObstacleGap = GetObstacleGap (index);
                }
            }
        }

        private float GetObstacleGap (int index)
        {
            foreach (KeyValuePair<int, float> item in m_ObstacleGapMap)
            {
                if (index >= item.Key)
                {
                    return item.Value;
                }
            }
            return 0f;
        }

        private void MoveCellsToBack (int count)
        {
            for (int i = 0; i < count; i++)
            {
                ScrollViewCell cell = m_Cells[0];
                m_Cells.RemoveAt (0);
                m_Cells.Insert (m_Cells.Count, cell);
            }

            for (int i = 0; i < count; i++)
            {
                SetCellAnchoredPosition (m_Cells.Count - i - 1);
                SetData (m_Cells.Count - i - 1);
            }
        }

        private void MoveCellsToFront (int count)
        {
            for (int i = 0; i < count; i++)
            {
                ScrollViewCell cell = m_Cells[m_Cells.Count - 1];
                m_Cells.RemoveAt (m_Cells.Count - 1);
                m_Cells.Insert (0, cell);
            }

            for (int i = 0; i < count; i++)
            {
                SetCellAnchoredPosition (i);
                SetData (i);
            }
        }

        private void PushCellToPool (int count)
        {
            if (count > m_Cells.Count)
            {
                count = m_Cells.Count;
            }

            List<ScrollViewCell> cells = m_Cells.GetRange (0, count);
            m_CellPool.AddRange (cells);
            m_Cells.RemoveRange (0, count);

            foreach (ScrollViewCell cell in cells)
            {
                cell.Sleep ();
            }
        }

        private void PopCellFromPool (int count)
        {
            while (count - m_CellPool.Count > 0)
            {
                GameObject gameObject = Instantiate (m_DefaultItem.gameObject, m_DefaultItem.transform.position, Quaternion.identity, m_DefaultItem.transform.parent);
                ScrollViewCell cell = m_CellType == null ? new ScrollViewCell () : Activator.CreateInstance (m_CellType) as ScrollViewCell;
                cell.Init (gameObject);
                m_CellPool.Add (cell);
            }

            List<ScrollViewCell> cells = m_CellPool.GetRange (0, count);
            m_Cells.AddRange (cells);
            m_CellPool.RemoveRange (0, count);

            foreach (ScrollViewCell cell in cells)
            {
                cell.Wakeup ();
            }
        }

        public void SetData ()
        {
            for (int i = 0; i < m_Cells.Count; i++)
            {
                OnSetData (m_Index + i, m_Cells[i]);
                m_Cells[i].SetView ();
            }
        }

        public void SetData (int index)
        {
            if (index >= 0 && index <= m_Cells.Count - 1)
            {
                OnSetData (m_Index + index, m_Cells[index]);
                m_Cells[index].SetView ();
            }
        }

        public void RegisterDataSetter (ScrollViewCellDataHandler scrollViewCellDataHandler)
        {
            OnSetData = scrollViewCellDataHandler;
        }

        /// <summary>
        /// The obstacle will be placed in front of the cell with the same index. Grid layout is not currently supported.
        /// </summary>
        /// <param name="index"></param>
        /// <param name="gameObject"></param>
        public void RegisterObstacle (int index, GameObject gameObject)
        {
            ScrollViewObstacle scrollViewObstacle;
            if (m_Obstacles.TryGetValue (index, out scrollViewObstacle))
            {
                scrollViewObstacle.Init (gameObject);
                scrollViewObstacle.SetAnchorAndPivot ();
            }
            else
            {
                scrollViewObstacle = new ScrollViewObstacle ();
                scrollViewObstacle.Init (gameObject);
                scrollViewObstacle.SetAnchorAndPivot ();
                m_Obstacles.Add (index, scrollViewObstacle);
            }
        }

        public void ClearObstacles ()
        {
            foreach (KeyValuePair<int, ScrollViewObstacle> item in m_Obstacles)
            {
                item.Value.Sleep ();
            }
            m_Obstacles.Clear ();
        }

        public void ScrollToTop ()
        {
            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    content.anchoredPosition = new Vector2 (0, content.anchoredPosition.y);
                    break;
                case LayoutType.Vertical:
                    content.anchoredPosition = new Vector2 (content.anchoredPosition.x, 0);
                    break;
                case LayoutType.Grid:
                    content.anchoredPosition = new Vector2 (0, 0);
                    break;
                default:
                    break;
            }

            OnScroll (content.anchoredPosition);
        }

        public void ScrollToIndex (int index, bool alignCenter = false)
        {
            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    {
                        float paddingLeft = m_Left;
                        float width = index * m_DefaultItem.rect.width;
                        float gap = index * m_SpaceX;
                        float obstacleGap = GetObstacleGap (index);
                        float translateX = alignCenter ? (m_DefaultItem.rect.width - viewport.rect.width) * 0.5f : m_DefaultItem.rect.width * 0.5f;
                        float offsetX = Mathf.Clamp (paddingLeft + width + gap + obstacleGap + translateX, 0, content.rect.width - viewport.rect.width);
                        content.anchoredPosition = new Vector2 (offsetX * -m_DirectionX, content.anchoredPosition.y);
                    }
                    break;
                case LayoutType.Vertical:
                    {
                        float paddingTop = m_Top;
                        float height = index * m_DefaultItem.rect.height;
                        float gap = index * m_SpaceY;
                        float obstacleGap = GetObstacleGap (index);
                        float translateY = alignCenter ? (m_DefaultItem.rect.height - viewport.rect.height) * 0.5f : m_DefaultItem.rect.height * 0.5f;
                        float offsetY = Mathf.Clamp (paddingTop + height + gap + obstacleGap + translateY, 0, content.rect.height - viewport.rect.height);
                        content.anchoredPosition = new Vector2 (content.anchoredPosition.x, offsetY * -m_DirectionY);
                    }
                    break;
                case LayoutType.Grid:
                    {
                        int row = index / m_Columns;
                        int column = index % m_Columns;

                        float paddingLeft = m_Left;
                        float width = column * m_DefaultItem.rect.width;
                        float gapX = column * m_SpaceX;
                        float translateX = (m_DefaultItem.rect.width - viewport.rect.width) * 0.5f;
                        float paddingTop = m_Top;
                        float height = row * m_DefaultItem.rect.height;
                        float gapY = row * m_SpaceY;
                        float translateY = (m_DefaultItem.rect.height - viewport.rect.height) * 0.5f;
                        float offsetX = Mathf.Clamp (paddingLeft + width + gapX + translateX, 0, content.rect.width - viewport.rect.width);
                        float offsetY = Mathf.Clamp (paddingTop + height + gapY + translateY, 0, content.rect.height - viewport.rect.height);
                        content.anchoredPosition = new Vector2 (offsetX * -m_DirectionX, offsetY * -m_DirectionY);
                    }
                    break;
                default:
                    break;
            }

            OnScroll (content.anchoredPosition);
        }

        public T Find<T> (Predicate<T> predicate) where T : ScrollViewCell
        {
            for (int i = 0; i < m_Cells.Count; i++)
            {
                if (predicate (m_Cells[i] as T))
                {
                    return m_Cells[i] as T;
                }
            }

            return null;
        }

        private class ReverseComparer<T> : IComparer<T> where T : IComparable<T>
        {
            public int Compare (T x, T y)
            {
                return -x.CompareTo (y);
            }
        }

        #region Editor
        [System.Diagnostics.Conditional ("UNITY_EDITOR")]
        public void EditorUpdateLayout ()
        {
            m_Capacity = 0;

            if (m_CellType != null)
            {
                ResizeContent (m_PreviewSize);
            }
            else
            {
                Init<ScrollViewCell> (m_PreviewSize);
            }
        }
        #endregion
    }
}
