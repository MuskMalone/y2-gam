/******************************************************************************/
/*!
\par        Image Engine
\file       Vector.cs

\author     Ernest Cheo (e.cheo@digipen.edu)
\date       Oct 26, 2023

\brief      A simplified vector struct, that allows for basic vector 
            functionality and operations in C#.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

using System;

namespace Image
{
    public struct Vector4
    {
        public float X, Y, Z, W;

        /*  _________________________________________________________________________ */
        /*! Vector3

        @return *this

        Default constructor for Vector3.
        */
        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        /*  _________________________________________________________________________ */
        /*! operator+
                        
        @param vector
        The LHS vector.

        @param vector
        The RHS vector.

        @return Vector3

        The operator overload for +.
        */
        public static Vector4 operator +(Vector4 a, Vector4 b)
        {
            return new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
        }
    }
    public struct Vector3
    {
        public float X, Y, Z;

        /*  _________________________________________________________________________ */
        /*! Vector3

        @return *this

        Default constructor for Vector3.
        */
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        /*  _________________________________________________________________________ */
        /*! operator+
                        
        @param vector
        The LHS vector.

        @param vector
        The RHS vector.

        @return Vector3

        The operator overload for +.
        */
        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        /*  _________________________________________________________________________ */
        /*! operator*
                
        @param vector
        The LHS vector.

        @param scalar
        The RHS scalar.

        @return Vector3

        The operator overload for *.
        */
        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public static Vector3 operator *(float scalar, Vector3 vector)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        /*  _________________________________________________________________________ */
        /*! operator/
        
        @param vector
        The LHS vector.

        @param scalar
        The RHS scalar.

        @return Vector3

        The operator overload for /.
        */
        public static Vector3 operator /(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X / scalar, vector.Y / scalar, vector.Z / scalar);
        }
    }

    public struct Vector2
    {
        public float X, Y;

        /*  _________________________________________________________________________ */
        /*! Vector2

        @return *this

        Default constructor for Vector2.
        */
        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        /*  _________________________________________________________________________ */
        /*! operator+
                        
        @param vector
        The LHS vector.

        @param vector
        The RHS vector.

        @return Vector2

        The operator overload for +.
        */
        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        /*  _________________________________________________________________________ */
        /*! operator-
                        
        @param vector
        The LHS vector.

        @param vector
        The RHS vector.

        @return Vector2

        The operator overload for -.
        */
        public static Vector2 operator -(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }

        /*  _________________________________________________________________________ */
        /*! operator*
                
        @param vector
        The LHS vector.

        @param scalar
        The RHS scalar.

        @return Vector2

        The operator overload for *.
        */
        public static Vector2 operator *(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X * scalar, vector.Y * scalar);
        }

        /*  _________________________________________________________________________ */
        /*! operator/
        
        @param vector
        The LHS vector.

        @param scalar
        The RHS scalar.

        @return Vector2

        The operator overload for /.
        */
        public static Vector2 operator /(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X / scalar, vector.Y / scalar);       
        }

        /*  _________________________________________________________________________ */
        /*! operator==
        
        @param lhs
        The LHS vector.

        @param rhs
        The RHS vector.

        @return bool

        The operator overload for ==.
        */
        public static bool operator ==(Vector2 lhs, Vector2 rhs)
        {
            return lhs.X == rhs.X && lhs.Y == rhs.Y;
        }

        /*  _________________________________________________________________________ */
        /*! operator!=
        
        @param lhs
        The LHS vector.

        @param rhs
        The RHS vector.

        @return bool

        The operator overload for !=.
        */
        public static bool operator !=(Vector2 lhs, Vector2 rhs)
        {
            return lhs.X != rhs.X || lhs.Y != rhs.Y;
        }

        // Override Equals method to provide custom equality comparison
        public override bool Equals(object obj)
        {
            if (!(obj is Vector2))
                return false;

            Vector2 other = (Vector2)obj;

            return X == other.X && Y == other.Y;
        }

        // Override GetHashCode method to provide a hash code for the struct
        public override int GetHashCode()
        {
            return X.GetHashCode() ^ Y.GetHashCode();
        }

        // Vector zero
        public static Vector2 Zero = new Vector2(0.0f, 0.0f);

        // Vector length
        public float Length()
        {
            return (float)Math.Sqrt(X * X + Y * Y);
        }
    }
}
