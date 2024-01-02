using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

namespace ProjectNothing
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
        private bool m_Paginate = false;

        [SerializeField]
        private int m_PreviewSize = 0;

        [SerializeField]
        private RectTransform m_DefaultItem = null;

        [SerializeField]
        private RectTransform m_DefaultPagination = null;

        private Type m_CellType;
        private readonly List<ScrollViewCell> m_Cells = new List<ScrollViewCell> ();
        private readonly List<ScrollViewCell> m_CellPool = new List<ScrollViewCell> ();

        private readonly Dictionary<int, ScrollViewObstacle> m_Obstacles = new Dictionary<int, ScrollViewObstacle> ();
        private readonly SortedDictionary<int, float> m_ObstacleGapMap = new SortedDictionary<int, float> (new ReverseComparer<int> ());

        private Type m_PaginationType;
        private readonly List<ScrollViewPagination> m_Paginations = new List<ScrollViewPagination> ();
        private readonly List<ScrollViewPagination> m_PaginationPool = new List<ScrollViewPagination> ();

        // set data event.
        public delegate void ScrollViewCellDataHandler (int index, ScrollViewCell scrollViewCell);
        private event ScrollViewCellDataHandler OnSetData = (int index, ScrollViewCell scrollViewCell) => { };

        // current index.
        private int m_Index = 0;
        // capacity.
        private int m_Capacity = 0;
        // obstacle gap, only use for counting index.
        private float m_ObstacleGap = 0f;
        // paginate index.
        private int m_PaginateIndex = 0;
        // scroll animate coroutine.
        private Coroutine m_ScrollAnimateCoroutine = null;

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

            ScrollToIndex (0);
        }

        public void InitPagination<T> () where T : ScrollViewPagination, new()
        {
            if (m_DefaultPagination == null)
            {
                return;
            }

            m_DefaultPagination.gameObject.SetActive (false);

            m_PaginationType = typeof (T);

            SetPaginations ();

            EnablePagination ();
        }

        public void ResizeContent (int capacity)
        {
            if (capacity < 0)
            {
                capacity = 0;
            }

            if (m_Paginate)
            {
                switch (m_LayoutType)
                {
                    case LayoutType.Horizontal:
                        capacity += capacity % m_Columns == 0 ? 0 : m_Columns - capacity % m_Columns;
                        break;
                    case LayoutType.Vertical:
                        capacity += capacity % m_Rows == 0 ? 0 : m_Rows - capacity % m_Rows;
                        break;
                    case LayoutType.Grid:
                        capacity += capacity % (m_Columns * m_Rows) == 0 ? 0 : (m_Columns * m_Rows) - capacity % (m_Columns * m_Rows);
                        break;
                    default:
                        break;
                }
            }

            if (m_Capacity == capacity)
            {
                SetObstaclesAnchoredPosition ();
                return;
            }

            m_Capacity = capacity;

            SetCells ();
            SetPaginations ();

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

        #region Cells
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

        private void MoveCellsToBack (int count)
        {
            if (count > m_Cells.Count)
            {
                count = m_Cells.Count;
            }

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
            if (count > m_Cells.Count)
            {
                count = m_Cells.Count;
            }

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

        public void SetData ()
        {
            for (int i = 0; i < m_Cells.Count; i++)
            {
                OnSetData (m_Index + i, m_Cells[i]);
                m_Cells[i].SetView ();
            }

            EnablePagination ();
        }

        private void SetData (int index)
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
        #endregion

        #region Obstacles
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
        #endregion

        #region Paginations
        private void SetPaginations ()
        {
            if (m_DefaultPagination == null)
            {
                return;
            }

            int count = 0;
            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    count = m_Capacity / m_Columns;
                    count += m_Capacity % m_Columns == 0 ? 0 : 1;
                    break;
                case LayoutType.Vertical:
                    count = m_Capacity / m_Rows;
                    count += m_Capacity % m_Rows == 0 ? 0 : 1;
                    break;
                case LayoutType.Grid:
                    count = m_Capacity / (m_Columns * m_Rows);
                    count += m_Capacity % (m_Columns * m_Rows) == 0 ? 0 : 1;
                    break;
                default:
                    break;
            }

            if (count < m_Paginations.Count)
            {
                PushPaginationToPool (m_Paginations.Count - count);
            }
            else if (count > m_Paginations.Count)
            {
                PopPaginationFromPool (count - m_Paginations.Count);
            }
        }

        private void PushPaginationToPool (int count)
        {
            if (count > m_Paginations.Count)
            {
                count = m_Paginations.Count;
            }

            List<ScrollViewPagination> paginations = m_Paginations.GetRange (0, count);
            m_PaginationPool.AddRange (paginations);
            m_Paginations.RemoveRange (0, count);

            foreach (ScrollViewPagination pagination in paginations)
            {
                pagination.Sleep ();
            }
        }

        private void PopPaginationFromPool (int count)
        {
            while (count - m_PaginationPool.Count > 0)
            {
                GameObject gameObject = Instantiate (m_DefaultPagination.gameObject, m_DefaultPagination.transform.position, Quaternion.identity, m_DefaultPagination.transform.parent);
                ScrollViewPagination pagination = m_PaginationType == null ? new ScrollViewPagination () : Activator.CreateInstance (m_PaginationType) as ScrollViewPagination;
                pagination.Init (gameObject);
                m_PaginationPool.Add (pagination);
            }

            List<ScrollViewPagination> paginations = m_PaginationPool.GetRange (0, count);
            m_Paginations.AddRange (paginations);
            m_PaginationPool.RemoveRange (0, count);

            foreach (ScrollViewPagination pagination in paginations)
            {
                pagination.Wakeup ();
            }
        }

        private void EnablePagination ()
        {
            int direction = m_Index > m_PaginateIndex ? 1 : 0;

            int page = 0;
            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    page = (m_Index + (m_Columns + direction) / 2) / m_Columns;
                    break;
                case LayoutType.Vertical:
                    page = (m_Index + (m_Rows + direction) / 2) / m_Rows;
                    break;
                case LayoutType.Grid:
                    page = (m_Index + (m_Columns * m_Rows + direction) / 2) / (m_Columns * m_Rows);
                    break;
                default:
                    break;
            }

            if (page >= m_Paginations.Count)
            {
                page = m_Paginations.Count - 1;
            }

            if (page < 0)
            {
                page = 0;
            }

            for (int i = 0; i < m_Paginations.Count; i++)
            {
                if (i == page)
                {
                    m_Paginations[i].Enable ();
                }
                else
                {
                    m_Paginations[i].Disable ();
                }
            }
        }
        #endregion

        private void OnScroll (Vector2 position)
        {
            int row = 0;
            int column = 0;
            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    column = Mathf.Max (0, Mathf.FloorToInt (((content.anchoredPosition.x * -m_DirectionX) - m_Left - m_ObstacleGap) / (m_DefaultItem.rect.width + m_SpaceX)));
                    break;
                case LayoutType.Vertical:
                    row = Mathf.Max (0, Mathf.FloorToInt (((content.anchoredPosition.y * -m_DirectionY) - m_Top - m_ObstacleGap) / (m_DefaultItem.rect.height + m_SpaceY)));
                    break;
                case LayoutType.Grid:
                    row = Mathf.Max (0, Mathf.FloorToInt (((content.anchoredPosition.y * -m_DirectionY) - m_Top + m_SpaceY) / (m_DefaultItem.rect.height + m_SpaceY)));
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
                    index = row * m_Columns;
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
                int count = index - m_Index;

                m_Index = index;

                if (m_Cells.Count > 0)
                {
                    if (count > 0)
                    {
                        MoveCellsToBack (count);
                    }
                    else
                    {
                        MoveCellsToFront (-count);
                    }
                }

                if (m_Obstacles.Count > 0)
                {
                    m_ObstacleGap = GetObstacleGap (index);
                }

                if (m_Paginations.Count > 0)
                {
                    EnablePagination ();
                }
            }
        }

        public override void OnBeginDrag (PointerEventData eventData)
        {
            base.OnBeginDrag (eventData);

            if (m_Paginate)
            {
                m_PaginateIndex = m_Index;
            }
        }

        public override void OnEndDrag (PointerEventData eventData)
        {
            base.OnEndDrag (eventData);

            if (m_Paginate)
            {
                int direction = m_Index > m_PaginateIndex ? 1 : 0;

                int page = 0;
                switch (m_LayoutType)
                {
                    case LayoutType.Horizontal:
                        page = (m_Index + (m_Columns + direction) / 2) / m_Columns;
                        break;
                    case LayoutType.Vertical:
                        page = (m_Index + (m_Rows + direction) / 2) / m_Rows;
                        break;
                    case LayoutType.Grid:
                        page = (m_Index + (m_Columns * m_Rows + direction) / 2) / (m_Columns * m_Rows);
                        break;
                    default:
                        break;
                }

                if (page >= m_Paginations.Count)
                {
                    page = m_Paginations.Count - 1;
                }

                if (page < 0)
                {
                    page = 0;
                }

                int index = 0;
                switch (m_LayoutType)
                {
                    case LayoutType.Horizontal:
                        index = page * m_Columns;
                        break;
                    case LayoutType.Vertical:
                        index = page * m_Rows;
                        break;
                    case LayoutType.Grid:
                        index = page * (m_Columns * m_Rows);
                        break;
                    default:
                        break;
                }

                ScrollToIndexAnimate (index, 0.12f);
            }
        }

        public void ScrollToIndex (int index, bool alignCenter = false)
        {
            SetContentAnchoredPosition (GetContentPosition (index, alignCenter));

            OnScroll (content.anchoredPosition);
        }

        public void ScrollToIndexAnimate (int index, float length, bool alignCenter = false)
        {
            Vector2 endPosition = GetContentPosition (index, alignCenter);

            if (m_ScrollAnimateCoroutine == null)
            {
                m_ScrollAnimateCoroutine = StartCoroutine (ScrollAnimate (index, endPosition, length));
            }
        }

        private Vector2 GetContentPosition (int index, bool alignCenter)
        {
            if (index == 0)
            {
                return Vector2.zero;
            }

            switch (m_LayoutType)
            {
                case LayoutType.Horizontal:
                    {
                        float paddingLeft = m_Left;
                        float width = index * m_DefaultItem.rect.width;
                        float gap = index * m_SpaceX;
                        float obstacleGap = GetObstacleGap (index);
                        float translateX = alignCenter ? (m_DefaultItem.rect.width - viewport.rect.width) * 0.5f : 0f;
                        float offsetX = Mathf.Clamp (paddingLeft + width + gap + obstacleGap + translateX, 0, content.rect.width - viewport.rect.width);
                        return new Vector2 (offsetX * -m_DirectionX, content.anchoredPosition.y);
                    }
                case LayoutType.Vertical:
                    {
                        float paddingTop = m_Top;
                        float height = index * m_DefaultItem.rect.height;
                        float gap = index * m_SpaceY;
                        float obstacleGap = GetObstacleGap (index);
                        float translateY = alignCenter ? (m_DefaultItem.rect.height - viewport.rect.height) * 0.5f : 0f;
                        float offsetY = Mathf.Clamp (paddingTop + height + gap + obstacleGap + translateY, 0, content.rect.height - viewport.rect.height);
                        return new Vector2 (content.anchoredPosition.x, offsetY * -m_DirectionY);
                    }
                case LayoutType.Grid:
                    {
                        int row = index / m_Columns;
                        int column = index % m_Columns;

                        float paddingLeft = m_Left;
                        float width = column * m_DefaultItem.rect.width;
                        float gapX = column * m_SpaceX;
                        float translateX = alignCenter ? (m_DefaultItem.rect.width - viewport.rect.width) * 0.5f : 0f;
                        float paddingTop = m_Top;
                        float height = row * m_DefaultItem.rect.height;
                        float gapY = row * m_SpaceY;
                        float translateY = alignCenter ? (m_DefaultItem.rect.height - viewport.rect.height) * 0.5f : 0f;
                        float offsetX = Mathf.Clamp (paddingLeft + width + gapX + translateX, 0, content.rect.width - viewport.rect.width);
                        float offsetY = Mathf.Clamp (paddingTop + height + gapY + translateY, 0, content.rect.height - viewport.rect.height);
                        return new Vector2 (offsetX * -m_DirectionX, offsetY * -m_DirectionY);
                    }
                default:
                    return Vector2.zero;
            }
        }

        private IEnumerator ScrollAnimate (int index, Vector2 endPosition, float length)
        {
            float sensitivity = scrollSensitivity;

            Vector2 startPosition = content.anchoredPosition;
            float totalLength = (Math.Abs (m_Index - index) + 1) * length;

            for (float time = 0f; time < totalLength; time += Time.deltaTime)
            {
                SetContentAnchoredPosition (Ease (startPosition, endPosition, time / totalLength));

                yield return null;
            }

            scrollSensitivity = sensitivity;
            m_ScrollAnimateCoroutine = null;
        }

        private Vector2 Ease (Vector2 startPosition, Vector2 endPosition, float normalizeTime)
        {
            // currently only support one method.
            return Hermite (startPosition, endPosition, normalizeTime);
        }

        private float Hermite (float startPosition, float endPosition, float normalizeTime)
        {
            return Mathf.Lerp (startPosition, endPosition, normalizeTime * normalizeTime * (3.0f - 2.0f * normalizeTime));
        }

        private Vector2 Hermite (Vector2 startPosition, Vector2 endPosition, float normalizeTime)
        {
            return new Vector2 (Hermite (startPosition.x, endPosition.x, normalizeTime), Hermite (startPosition.y, endPosition.y, normalizeTime));
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
