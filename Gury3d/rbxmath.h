#ifndef RBX_MATH_H
#define RBX_MATH_H
#include <G3DAll.h>

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
            u.unitize(0.000001);
            result = acos(u.y);
            if (y < 0.0)
                return 6.2831855 - result;
            return result;
        }
        static double radWrap(float rad)
        {
            double result; // st7
            float v2; // [esp+Ch] [ebp-4h]

            result = rad;
            if (rad < -3.1415927 || result >= 3.1415927)
            {
                v2 = floor((result + 3.1415927) * 0.15915495);
                return rad - (double)(int)v2 * 6.283185;
            }
            return result;
        }
    }
}
#endif