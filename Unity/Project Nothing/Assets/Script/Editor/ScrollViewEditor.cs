using UnityEditor;
using UnityEditor.UI;
using UnityEngine;

[CustomEditor (typeof (ScrollView))]
public class ScrollViewEditor : ScrollRectEditor
{
    SerializedProperty layoutTypeProp;
    SerializedProperty columnProp;
    SerializedProperty rowProp;
    SerializedProperty spaceXProp;
    SerializedProperty spaceYProp;
    SerializedProperty leftProp;
    SerializedProperty topProp;
    SerializedProperty defaultItemProp;

    protected override void OnEnable ()
    {
        base.OnEnable ();

        layoutTypeProp = serializedObject.FindProperty ("m_LayoutType");
        columnProp = serializedObject.FindProperty ("m_Columns");
        rowProp = serializedObject.FindProperty ("m_Rows");
        spaceXProp = serializedObject.FindProperty ("m_SpaceX");
        spaceYProp = serializedObject.FindProperty ("m_SpaceY");
        leftProp = serializedObject.FindProperty ("m_Left");
        topProp = serializedObject.FindProperty ("m_Top");
        defaultItemProp = serializedObject.FindProperty ("m_DefaultItem");
    }

    public override void OnInspectorGUI ()
    {
        base.OnInspectorGUI ();

        serializedObject.Update ();

        EditorGUILayout.Space ();
        EditorGUILayout.LabelField ("Layout", EditorStyles.boldLabel);
        EditorGUILayout.PropertyField (layoutTypeProp, new GUIContent ("Type"));

        EditorGUI.indentLevel++;
        switch ((ScrollView.LayoutType)layoutTypeProp.enumValueIndex)
        {
            case ScrollView.LayoutType.Horizontal:
                EditorGUILayout.PropertyField (columnProp, new GUIContent ("Column Size"));
                break;
            case ScrollView.LayoutType.Vertical:
                EditorGUILayout.PropertyField (rowProp, new GUIContent ("Row Size"));
                break;
            case ScrollView.LayoutType.Grid:
                EditorGUILayout.PropertyField (columnProp, new GUIContent ("Column Size"));
                EditorGUILayout.PropertyField (rowProp, new GUIContent ("Row Size"));
                break;
        }
        EditorGUI.indentLevel--;

        switch ((ScrollView.LayoutType)layoutTypeProp.enumValueIndex)
        {
            case ScrollView.LayoutType.Horizontal:
                EditorGUILayout.PropertyField (spaceXProp, new GUIContent ("Space X"));
                break;
            case ScrollView.LayoutType.Vertical:
                EditorGUILayout.PropertyField (spaceYProp, new GUIContent ("Space Y"));
                break;
            case ScrollView.LayoutType.Grid:
                EditorGUILayout.PropertyField (spaceXProp, new GUIContent ("Space X"));
                EditorGUILayout.PropertyField (spaceYProp, new GUIContent ("Space Y"));
                break;
        }

        EditorGUILayout.PropertyField (leftProp, new GUIContent ("Left"));
        EditorGUILayout.PropertyField (topProp, new GUIContent ("Top"));

        EditorGUILayout.Space ();
        EditorGUILayout.PropertyField (defaultItemProp, new GUIContent ("Default Item"));

        serializedObject.ApplyModifiedProperties ();
    }
}
