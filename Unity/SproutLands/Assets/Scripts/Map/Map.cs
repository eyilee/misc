namespace SproutLands
{
    public class Map<T>
    {
        public int Width { get; private set; }
        public int Height { get; private set; }
        public T?[] Values { get; private set; }

        public Map (int width, int height)
        {
            Width = width;
            Height = height;
            Values = new T[width * height];
        }

        private T? this[int x, int y]
        {
            get => Values[x + (y * Width)];
            set => Values[x + (y * Width)] = value;
        }

        public T? GetValue (int x, int y)
        {
            if (x < 0 || x >= Width || y < 0 || y >= Height)
            {
                return default;
            }

            return this[x, y];
        }

        public void SetValue (int x, int y, T? value)
        {
            if (x < 0 || x >= Width || y < 0 || y >= Height)
            {
                return;
            }

            this[x, y] = value;
        }

        public bool TryGetValue (int x, int y, out T? value)
        {
            if (x < 0 || x >= Width || y < 0 || y >= Height)
            {
                value = default;
                return false;
            }

            value = this[x, y];
            return true;
        }

        public bool InBounds (int x, int y)
        {
            if (x < 0 || x >= Width || y < 0 || y >= Height)
            {
                return false;
            }

            return true;
        }
    }
}
