using System;

namespace SproutLands
{
    public readonly struct Coord : IEquatable<Coord>
    {
        public static readonly Coord[] Neighbors4 =
        {
            new (0, 1),
            new (1, 0),
            new (0, -1),
            new (-1, 0),
        };

        public static readonly Coord[] Neighbors8 =
        {
            new (0, 1),
            new (1, 1),
            new (1, 0),
            new (1, -1),
            new (0, -1),
            new (-1, -1),
            new (-1, 0),
            new (-1, 1),
        };

        public readonly int X;
        public readonly int Y;

        public Coord (int x, int y)
        {
            X = x;
            Y = y;
        }

        public static Coord operator + (Coord operand) => operand;

        public static Coord operator - (Coord operand) => new (-operand.X, -operand.Y);

        public static Coord operator + (Coord lhs, Coord rhs) => new (lhs.X + rhs.X, lhs.Y + rhs.Y);

        public static Coord operator - (Coord lhs, Coord rhs) => new (lhs.X - rhs.X, lhs.Y - rhs.Y);

        public static bool operator == (Coord lhs, Coord rhs) => lhs.Equals (rhs);

        public static bool operator != (Coord lhs, Coord rhs) => !lhs.Equals (rhs);

        public readonly bool Equals (Coord other) => X == other.X && Y == other.Y;

        public override readonly bool Equals (object obj) => obj is Coord other && Equals (other);

        public override readonly int GetHashCode () => HashCode.Combine (X, Y);
    }
}
