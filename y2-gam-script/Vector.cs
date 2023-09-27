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
    }
}
