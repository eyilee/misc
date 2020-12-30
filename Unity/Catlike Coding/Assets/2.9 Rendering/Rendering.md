# 2.9 Rendering

## 1 Matrices

- 組件(Component)的排序會影響執行順序
- 縮放矩陣和位移矩陣可以用一個四維矩陣來表示，當第四維為 0 時表示為向量，1 時表示為點
- 透過捨棄 z 維度可以達成正交投影，但在相機後的點會被錯誤的投影，因此必須保證所有點在相機前面

## 2 Shader Fundamentals

- 4x4 的 MVP 矩陣在 UnityShaderVariables 中定義為 UNITY_MATRIX_MVP
- TRANSFORM_TEX 結果為計算過 tiling 和 offset 的 uv

## 3 Combining Textures

- Unity 色彩空間預設使用 Gamma 空間，在線性空間中渲染時會將材質與貼圖顏色先進行轉換，輸出才再轉換回 Gamma 空間，使用 unity_ColorSpaceDouble 可以確保使用正確的顏色

## 4 The First Light

- Dynamic batching 合併 mesh 時會從 object space 轉換到 world space，因此必須在 world space 中計算法線，使用 unity_ObjectToWorld 將向量轉換到 world space 時各法線受到的縮放不同
，因此標準化後的長度並不是正確的，必須再將法線轉換回 object space，此過程可以用 UnityObjectToWorldNormal 完成
- DotClamped 執行內積並保證值不為負
- Saturate 限制結果為 0 到 1

## 5 Multiple Lights

- multi_compile 定義多個著色器變體
- texture type 選擇 cookie 可以做為光照紋理使用

## 6 Bumpiness

- 切線空間是指在頂點上以切線(T)為 X 軸、副切線(B)為 Y 軸、法線(N)為 Z 軸的座標系