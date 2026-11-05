using UnityEngine;

namespace SproutLands
{
    public enum MapDirection
    {
        None,
        Up,
        Right,
        Down,
        Left
    }

    public static class MapDirectionExtensions
    {
        public static Vector2Int ToVector2Int (this MapDirection direction) => direction switch
        {
            MapDirection.None => Vector2Int.zero,
            MapDirection.Up => Vector2Int.up,
            MapDirection.Right => Vector2Int.right,
            MapDirection.Down => Vector2Int.down,
            MapDirection.Left => Vector2Int.left,
            _ => throw new System.NotImplementedException ()
        };

        public static Vector3Int ToVector3Int (this MapDirection direction) => direction switch
        {
            MapDirection.None => Vector3Int.zero,
            MapDirection.Up => Vector3Int.up,
            MapDirection.Right => Vector3Int.right,
            MapDirection.Down => Vector3Int.down,
            MapDirection.Left => Vector3Int.left,
            _ => throw new System.NotImplementedException ()
        };

        public static Vector2 ToVector2 (this MapDirection direction) => direction switch
        {
            MapDirection.None => Vector2.zero,
            MapDirection.Up => Vector2.up,
            MapDirection.Right => Vector2.right,
            MapDirection.Down => Vector2.down,
            MapDirection.Left => Vector2.left,
            _ => throw new System.NotImplementedException ()
        };

        public static Vector3 ToVector3 (this MapDirection direction) => direction switch
        {
            MapDirection.None => Vector3.zero,
            MapDirection.Up => Vector3.up,
            MapDirection.Right => Vector3.right,
            MapDirection.Down => Vector3.down,
            MapDirection.Left => Vector3.left,
            _ => throw new System.NotImplementedException ()
        };
    }
}
