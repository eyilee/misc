using UnityEditor;
using UnityEditor.UI;
using UnityEngine;

[CustomEditor (typeof (XScrollRect))]
public class XScrollRectEditor : ScrollRectEditor
{
    SerializedProperty layoutTypeProp;
    SerializedProperty columnProp;
    SerializedProperty rowProp;
    SerializedProperty defaultItemProp;

    protected override void OnEnable ()
    {
        base.OnEnable ();

        layoutTypeProp = serializedObject.FindProperty ("m_LayoutType");
        columnProp = serializedObject.FindProperty ("m_Columns");
        rowProp = serializedObject.FindProperty ("m_Rows");
        defaultItemProp = serializedObject.FindProperty ("m_DefaultItem");
    }

    public override void OnInspectorGUI ()
    {
        base.OnInspectorGUI ();

        serializedObject.Update ();

        EditorGUILayout.Space ();
        EditorGUILayout.PropertyField (layoutTypeProp, new GUIContent ("Layout Type"));

        EditorGUI.indentLevel++;
        switch ((XScrollRect.LayoutType)layoutTypeProp.enumValueIndex)
        {
            case XScrollRect.LayoutType.Horizontal:
                EditorGUILayout.PropertyField (columnProp, new GUIContent ("Column Size"));
                break;
            case XScrollRect.LayoutType.Vertical:
                EditorGUILayout.PropertyField (rowProp, new GUIContent ("Row Size"));
                break;
            case XScrollRect.LayoutType.Grid:
                EditorGUILayout.PropertyField (columnProp, new GUIContent ("Column Size"));
                EditorGUILayout.PropertyField (rowProp, new GUIContent ("Row Size"));
                break;
        }
        EditorGUI.indentLevel--;

        EditorGUILayout.Space ();
        EditorGUILayout.PropertyField (defaultItemProp, new GUIContent ("Default Item"));

        serializedObject.ApplyModifiedProperties ();
    }
}
