#ifndef RBX_MATH_H
#define RBX_MATH_H
#include <G3DAll.h>

#define RBX_PI  3.1415927
#define RBX_TAU 6.2831855

namespace RBX
{
    namespace Math
    {
        static double angleToE0(const G3D::Vector2 v)
        {
            double result; // st7
            G3D::Vector2 u; // [esp+0h] [ebp-Ch] BYREF
            float y; // [esp+8h] [ebp-4h]

            u.y = v.x;
            y = v.y;
            u.unitize(0.000001f);
            result = acos(u.y);
            if (y < 0.0)
                return RBX_TAU - result;
            return result;
        }
        static double radWrap(float rad)
        {
            double result; // st7
            float v2; // [esp+Ch] [ebp-4h]

            result = rad;
            if (rad < -RBX_PI || result >= RBX_PI)
            {
                v2 = floor((result + RBX_PI) * 0.15915495);
                return rad - (double)(int)v2 * RBX_TAU;
            }
            return result;
        }
    }
}
#endif