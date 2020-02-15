using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class XScrollRect : ScrollRect
{
    public enum LayoutType
    {
        Horizontal,
        Vertical,
        Grid
    }

    public LayoutType layoutType;
    public int column;
    public int row;

    public GameObject defaultItem;

    class ItemInfo
    {

    }

    public override void OnScroll (PointerEventData data)
    {
        base.OnScroll (data);
    }
}
