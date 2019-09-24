﻿using System;
using System.Collections;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class TheCube : Selectable
{
    bool isSelected = false;
    Coroutine move = null;

    public void Update ()
    {
        if (Input.GetMouseButton (1))
        {
            if (isSelected)
            {
                OnMove (Input.mousePosition);
            }
        }
    }

    void OnMove (Vector3 mousePosition)
    {
        if(move != null)
        {
            StopCoroutine (move);
        }

        move = StartCoroutine (Move (mousePosition));
    }

    public IEnumerator Move (Vector3 target)
    {
        Vector3 distance = target - transform.position;
        Vector3 direction = distance.normalized;
        float length = distance.magnitude;

        Vector3 step = direction * 200.0f;
        float time = length / 200.0f;
        float deltaTime = 0.0f;

        while (deltaTime < time)
        {
            deltaTime += Time.deltaTime;
            transform.Translate(step * Time.deltaTime);
            yield return null;
        }
    }

    public override void OnSelect (BaseEventData eventData)
    {
        isSelected = true;
    }

    public override void OnDeselect (BaseEventData eventData)
    {
    }
}
