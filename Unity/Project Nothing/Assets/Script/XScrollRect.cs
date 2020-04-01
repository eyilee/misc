using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class XScrollRect : ScrollRect
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
    private RectTransform m_DefaultItem = null;

    private readonly List<ScrollViewCell> m_Cells = new List<ScrollViewCell> ();
    private readonly List<ScrollViewCell> m_CellPool = new List<ScrollViewCell> ();

    // set data event.
    public delegate void ScrollViewCellDataHandler (int index, ScrollViewCell scrollViewCell);
    private event ScrollViewCellDataHandler OnSetData;

    // current index.
    private int m_Index = 0;
    // number of cells.
    private int m_Capacity = 1;

    // right.
    private const int m_DirectionX = 1;
    // down.
    private const int m_DirectionY = -1;

    protected override void Awake ()
    {
        base.Awake ();

        m_DefaultItem.anchorMin = new Vector2 (0f, 1f);
        m_DefaultItem.anchorMax = new Vector2 (0f, 1f);
        m_DefaultItem.pivot = new Vector2 (0f, 1f);

        content.anchorMin = new Vector2 (0f, 1f);
        content.anchorMax = new Vector2 (0f, 1f);
        content.pivot = new Vector2 (0f, 1f);

        onValueChanged.AddListener (OnScroll);
    }

    public void Init<T> (int capacity) where T : ScrollViewCell, new()
    {
        if (m_DefaultItem == null)
        {
            return;
        }

        m_DefaultItem.gameObject.SetActive (false);

        SetCells<T> ();

        ResizeContent (capacity);
    }

    public void ResizeContent (int capacity)
    {
        if (capacity < 0)
        {
            capacity = 0;
        }

        m_Capacity = capacity;

        int rows = 1;
        int columns = 1;
        switch (m_LayoutType)
        {
            case LayoutType.Horizontal:
                rows = 1;
                columns = capacity;
                break;
            case LayoutType.Vertical:
                rows = capacity;
                columns = 1;
                break;
            case LayoutType.Grid:
                rows = (capacity % m_Columns) == 0 ? capacity / m_Columns : capacity / m_Columns + 1;
                columns = m_Columns;
                break;
            default:
                break;
        }

        content.sizeDelta = new Vector2 (columns * m_DefaultItem.rect.width, rows * m_DefaultItem.rect.height);
        SetCellsAnchoredPosition ();
    }

    private void SetCells<T> () where T : ScrollViewCell, new()
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

        if (count < m_Cells.Count)
        {
            PushCellToPool (m_Cells.Count - count);
        }
        else if (count > m_Cells.Count)
        {
            PopCellFromPool<T> (count - m_Cells.Count);
        }
    }

    private void SetCellsAnchoredPosition ()
    {
        switch (m_LayoutType)
        {
            case LayoutType.Horizontal:
                for (int i = 0; i < m_Cells.Count; i++)
                {
                    m_Cells[i].SetAnchoredPositionX ((m_Index + i) * m_DefaultItem.rect.width * m_DirectionX);
                }
                break;
            case LayoutType.Vertical:
                for (int i = 0; i < m_Cells.Count; i++)
                {
                    m_Cells[i].SetAnchoredPositionY ((m_Index + i) * m_DefaultItem.rect.height * m_DirectionY);
                }
                break;
            case LayoutType.Grid:
                for (int i = 0; i < m_Cells.Count; i++)
                {
                    int row = (m_Index + i) / m_Columns;
                    int column = (m_Index + i) % m_Columns;

                    Vector2 position = new Vector2 (column * m_DefaultItem.rect.width * m_DirectionX, row * m_DefaultItem.rect.height * m_DirectionY);
                    m_Cells[i].SetAnchoredPosition (position);
                }
                break;
            default:
                break;
        }
    }

    private void OnScroll (Vector2 position)
    {
        float width = m_DefaultItem.rect.width;
        float height = m_DefaultItem.rect.height;

        int row = Mathf.FloorToInt (Math.Abs (content.anchoredPosition.y) / height);
        int column = Mathf.FloorToInt (Math.Abs (content.anchoredPosition.x) / width);

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
        else if (index > m_Capacity - m_Cells.Count)
        {
            index = m_Capacity - m_Cells.Count;
        }

        if (m_Index != index)
        {
            if (index > m_Index)
            {
                ScrollViewCell cell = m_Cells[0];
                m_Cells.RemoveAt (0);
                m_Cells.Add (cell);
            }
            else
            {
                ScrollViewCell cell = m_Cells[m_Cells.Count - 1];
                m_Cells.RemoveAt (m_Cells.Count - 1);
                m_Cells.Insert (0, cell);
            }

            m_Index = index;

            SetCellsAnchoredPosition ();
            SetData ();
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

    private void PopCellFromPool<T> (int count) where T : ScrollViewCell, new()
    {
        while (count - m_CellPool.Count > 0)
        {
            GameObject gameObject = Instantiate (m_DefaultItem.gameObject, m_DefaultItem.transform.position, Quaternion.identity, m_DefaultItem.transform.parent);
            T cell = new T ();
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
        }
    }

    public void RegisterDataSetter (ScrollViewCellDataHandler scrollViewCellDataHandler)
    {
        OnSetData = scrollViewCellDataHandler;
    }
}
