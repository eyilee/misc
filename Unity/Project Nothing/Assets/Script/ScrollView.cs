using System;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

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
    private RectTransform m_DefaultItem = null;

    private Type m_CellType;
    private readonly List<ScrollViewCell> m_Cells = new List<ScrollViewCell> ();
    private readonly List<ScrollViewCell> m_CellPool = new List<ScrollViewCell> ();

    // set data event.
    public delegate void ScrollViewCellDataHandler (int index, ScrollViewCell scrollViewCell);
    private event ScrollViewCellDataHandler OnSetData;

    // current index.
    private int m_Index = 0;
    // capacity.
    private int m_Capacity = 0;

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
    }

    public void ResizeContent (int capacity)
    {
        if (capacity < 0)
        {
            capacity = 0;
        }

        if (m_Capacity == capacity)
        {
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

        float paddingLeft = m_Left * m_DirectionX;
        float width = columns * m_DefaultItem.rect.width;
        float gapX = (columns - 1) * m_SpaceX;
        float paddingTop = m_Top * m_DirectionY * -1;
        float height = rows * m_DefaultItem.rect.height;
        float gapY = (rows - 1) * m_SpaceY;

        content.sizeDelta = new Vector2 (paddingLeft + width + gapX, paddingTop + height + gapY);
        content.anchoredPosition = Vector2.zero;

        m_Index = 0;

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
        switch (m_LayoutType)
        {
            case LayoutType.Horizontal:
                for (int i = 0; i < m_Cells.Count; i++)
                {
                    float paddingLeft = m_Left * m_DirectionX;
                    float width = (m_Index + i) * m_DefaultItem.rect.width * m_DirectionX;
                    float gap = m_Index * m_SpaceX * m_DirectionX;
                    float translateX = m_DefaultItem.rect.width * m_DirectionX * 0.5f;
                    m_Cells[i].SetAnchoredPositionX (paddingLeft + width + gap + translateX);
                }
                break;
            case LayoutType.Vertical:
                for (int i = 0; i < m_Cells.Count; i++)
                {
                    float paddingTop = m_Top * m_DirectionY;
                    float height = (m_Index + i) * m_DefaultItem.rect.height * m_DirectionY;
                    float gap = m_Index * m_SpaceY * m_DirectionY;
                    float translateY = m_DefaultItem.rect.height * m_DirectionY * 0.5f;
                    m_Cells[i].SetAnchoredPositionY (paddingTop + height + gap + translateY);
                }
                break;
            case LayoutType.Grid:
                for (int i = 0; i < m_Cells.Count; i++)
                {
                    int row = (m_Index + i) / m_Columns;
                    int column = (m_Index + i) % m_Columns;

                    float paddingLeft = m_Left * m_DirectionX;
                    float width = column * m_DefaultItem.rect.width * m_DirectionX;
                    float gapX = column * m_SpaceX * m_DirectionX;
                    float translateX = m_DefaultItem.rect.width * m_DirectionX * 0.5f;
                    float paddingTop = m_Top * m_DirectionY;
                    float height = row * m_DefaultItem.rect.height * m_DirectionY;
                    float gapY = row * m_SpaceY * m_DirectionY;
                    float translateY = m_DefaultItem.rect.height * m_DirectionY * 0.5f;
                    Vector2 position = new Vector2 (paddingLeft + width + gapX + translateX, paddingTop + height + gapY + translateY);
                    m_Cells[i].SetAnchoredPosition (position);
                }
                break;
            default:
                break;
        }
    }

    private void SetCellsAnchoredPosition (int index)
    {
        switch (m_LayoutType)
        {
            case LayoutType.Horizontal:
                {
                    float paddingLeft = m_Left * m_DirectionX;
                    float width = (m_Index + index) * m_DefaultItem.rect.width * m_DirectionX;
                    float gap = m_Index * m_SpaceX * m_DirectionX;
                    float translateX = m_DefaultItem.rect.width * m_DirectionX * 0.5f;
                    m_Cells[index].SetAnchoredPositionX (paddingLeft + width + gap + translateX);
                }
                break;
            case LayoutType.Vertical:
                {
                    float paddingTop = m_Top * m_DirectionY;
                    float height = (m_Index + index) * m_DefaultItem.rect.height * m_DirectionY;
                    float gap = m_Index * m_SpaceY * m_DirectionY;
                    float translateY = m_DefaultItem.rect.height * m_DirectionY * 0.5f;
                    m_Cells[index].SetAnchoredPositionY (paddingTop + height + gap + translateY);
                }
                break;
            case LayoutType.Grid:
                {
                    int row = (m_Index + index) / m_Columns;
                    int column = (m_Index + index) % m_Columns;

                    float paddingLeft = m_Left * m_DirectionX;
                    float width = column * m_DefaultItem.rect.width * m_DirectionX;
                    float gapX = column * m_SpaceX * m_DirectionX;
                    float translateX = m_DefaultItem.rect.width * m_DirectionX * 0.5f;
                    float paddingTop = m_Top * m_DirectionY;
                    float height = row * m_DefaultItem.rect.height * m_DirectionY;
                    float gapY = row * m_SpaceY * m_DirectionY;
                    float translateY = m_DefaultItem.rect.height * m_DirectionY * 0.5f;
                    Vector2 position = new Vector2 (paddingLeft + width + gapX + translateX, paddingTop + height + gapY + translateY);
                    m_Cells[index].SetAnchoredPosition (position);
                }
                break;
            default:
                break;
        }
    }

    private void OnScroll (Vector2 position)
    {
        int row = 0;
        int column = 0;
        switch (m_LayoutType)
        {
            case LayoutType.Horizontal:
                column = Mathf.FloorToInt ((content.anchoredPosition.x + m_Left * m_DirectionX) / (m_DefaultItem.rect.width + m_SpaceX));
                break;
            case LayoutType.Vertical:
                row = Mathf.FloorToInt ((content.anchoredPosition.y + m_Top * m_DirectionY) / (m_DefaultItem.rect.height + m_SpaceY));
                break;
            case LayoutType.Grid:
                row = Mathf.FloorToInt (((content.anchoredPosition.y + m_Top * m_DirectionY) + m_SpaceY) / (m_DefaultItem.rect.height + m_SpaceY));
                column = Mathf.FloorToInt (((content.anchoredPosition.x + m_Left * m_DirectionX) + m_SpaceX) / (m_DefaultItem.rect.width + m_SpaceX));
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

        if (m_Index != index && m_Cells.Count > 0)
        {
            int count = Math.Abs (m_Index - index);
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
            SetCellsAnchoredPosition (m_Cells.Count - i - 1);
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
            SetCellsAnchoredPosition (i);
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
            ScrollViewCell cell = Activator.CreateInstance (m_CellType) as ScrollViewCell;
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
}
