/******************************************************************************/
/*!
\par        Image Engine
\file       Vector.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Sep 23, 2023

\brief      A simplified vector struct, that allows for basic vector 
            functionality and operations in C#.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

namespace Image
{
    public struct Vector3
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public static Vector3 operator /(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X / scalar, vector.Y / scalar, vector.Z / scalar);
        }
    }

    public struct Vector2
    {
        public float X, Y;

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }
        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        public static Vector2 operator *(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X * scalar, vector.Y * scalar);
        }

        public static Vector2 operator /(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X / scalar, vector.Y / scalar);       
        }
    }
}
