using System;

namespace SproutLands
{
    public static class MapExtension
    {
        public static T? GetValue<T> (this Map<T> map, Coord coord) => map.GetValue (coord.X, coord.Y);

        public static void SetValue<T> (this Map<T> map, Coord coord, T? value) => map.SetValue (coord.X, coord.Y, value);

        public static void ForEach<T> (this Map<T> map, Action<T?> action)
        {
            for (int x = 0; x < map.Width; x++)
            {
                for (int y = 0; y < map.Height; y++)
                {
                    if (map.TryGetValue (x, y, out T? value))
                    {
                        action (value);
                    }
                }
            }
        }

        public static bool ForEachNeighbors4<T> (this Map<T> map, int x, int y, Action<T?> action)
        {
            foreach (Coord neighbor in Coord.Neighbors4)
            {
                if (map.TryGetValue (x + neighbor.X, y + neighbor.Y, out T? value))
                {
                    action (value);
                }
            }

            return false;
        }

        public static bool ForEachNeighbors4<T> (this Map<T> map, Coord coord, Action<T?> action) => ForEachNeighbors4 (map, coord.X, coord.Y, action);

        public static bool ForEachNeighbors8<T> (this Map<T> map, int x, int y, Action<T?> action)
        {
            foreach (Coord neighbor in Coord.Neighbors8)
            {
                if (map.TryGetValue (x + neighbor.X, y + neighbor.Y, out T? value))
                {
                    action (value);
                }
            }

            return false;
        }

        public static bool ForEachNeighbors8<T> (this Map<T> map, Coord coord, Action<T?> action) => ForEachNeighbors8 (map, coord.X, coord.Y, action);

        public static bool AnyNeighbors4<T> (this Map<T> map, int x, int y, Func<T?, bool> predict)
        {
            foreach (Coord neighbor in Coord.Neighbors4)
            {
                if (map.TryGetValue (x + neighbor.X, y + neighbor.Y, out T? value))
                {
                    if (predict (value))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        public static bool AnyNeighbors4<T> (this Map<T> map, Coord coord, Func<T?, bool> predict) => AnyNeighbors4 (map, coord.X, coord.Y, predict);

        public static bool AnyNeighbors8<T> (this Map<T> map, int x, int y, Func<T?, bool> predict)
        {
            foreach (Coord neighbor in Coord.Neighbors8)
            {
                if (map.TryGetValue (x + neighbor.X, y + neighbor.Y, out T? value))
                {
                    if (predict (value))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        public static bool AnyNeighbors8<T> (this Map<T> map, Coord coord, Func<T?, bool> predict) => AnyNeighbors8 (map, coord.X, coord.Y, predict);

        public static bool InBounds<T> (this Map<T> map, Coord coord) => map.InBounds (coord.X, coord.Y);
    }
}
