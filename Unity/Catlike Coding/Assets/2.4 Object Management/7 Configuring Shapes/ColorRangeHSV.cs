﻿using UnityEngine;

namespace ObjectManagement
{
    namespace ConfiguringShapes
    {
        [System.Serializable]
        public struct ColorRangeHSV
        {
            [FloatRangeSlider (0f, 1f)]
            public FloatRange hue;

            [FloatRangeSlider (0f, 1f)]
            public FloatRange saturation;

            [FloatRangeSlider (0f, 1f)]
            public FloatRange value;

            public Color RandomInRange {
                get {
                    return Random.ColorHSV (
                        hue.min, hue.max,
                        saturation.min, saturation.max,
                        value.min, value.max,
                        1f, 1f
                    );
                }
            }
        }
    }
}