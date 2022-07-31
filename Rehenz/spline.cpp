#include "spline.h"

namespace Rehenz
{
    inline float CalcBSplineF(float knots[], int i, int n, float x)
    {
        if (knots[i] == knots[i + n])
            return 0;
        else
            return (x - knots[i]) / (knots[i + n] - knots[i]);
    }

    float CalcBSplineBase(int n, float knots[], float x)
    {
        //float knots[n + 2];
        float* N = new float[static_cast<llong>(n) + 1];

        // init N_{i,0}
        for (int i = 0; i < n; i++)
            N[i] = (knots[i] <= x && x < knots[i + 1] ? 1.0f : 0.0f);
        N[n] = (knots[n] <= x && x <= knots[n + 1] ? 1.0f : 0.0f);

        // calc N_{i,nn}
        for (int nn = 1; nn <= n; nn++)
        {
            for (int i = 0; i <= n - nn; i++)
            {
                float f = CalcBSplineF(knots, i, nn, x);
                float g = 1 - CalcBSplineF(knots, i + 1, nn, x);
                N[i] = f * N[i] + g * N[i + 1];
            }
        }

        // return base
        float base = N[0];
        delete[] N;
        return base;
    }

    Point2 CalcBSplineZone(int n, float knots[], Point2 points[], float x)
    {
        //float knots[2 * n + 2];
        //Point2 points[n + 1];
        //x \in [k_n, k_{n+1}]

        Point2 p(0, 0);
        for (int i = 0; i <= n; i++)
            p += CalcBSplineBase(n, knots + i, x) * points[i];
        return p;
    }

    Point2 CalcBSpline(int n, int m, float knots[], Point2 points[], float x)
    {
        //float knots[n + m + 2];
        //Point2 points[m + 1];
        //x \in [k_n, k_{m+1}]

        if (x < knots[n] || x > knots[m + 1])
            return Point2(0, 0);
        for (int i = n; i < m + 1; i++)
        {
            if (x < knots[i + 1])
                return CalcBSplineZone(n, knots + (i - static_cast<llong>(n)), points + (i - static_cast<llong>(n)), x);
        }
        return CalcBSplineZone(n, knots + (m - static_cast<llong>(n)), points + (m - static_cast<llong>(n)), x);
    }

    Point2 CalcBSplineZoneDeBoor(int n, float knots[], Point2 points[], float x)
    {
        //float knots[2 * n + 2];
        //Point2 points[n + 1];
        //x \in [k_n, k_{n+1}]
        Point2* p = new Point2[static_cast<llong>(n) + 1];

        // copy points
        for (int i = 0; i <= n; i++)
            p[i] = points[i];

        // lerp
        for (int nn = 1; nn <= n; nn++)
        {
            for (int i = nn; i <= n; i++)
            {
                float f = CalcBSplineF(knots, i, n + 1 - nn, x);
                p[i] = Lerp(p[i], p[i - 1], f);
            }
        }

        // return point
        Point2 pp = p[n];
        delete[] p;
        return pp;
    }

    Point2 CalcBSplineDeBoor(int n, int m, float knots[], Point2 points[], float x)
    {
        //float knots[n + m + 2];
        //Point2 points[m + 1];
        //x \in [k_n, k_{m+1}]

        if (x < knots[n] || x > knots[m + 1])
            return Point2(0, 0);
        for (int i = n; i < m + 1; i++)
        {
            if (x < knots[i + 1])
                return CalcBSplineZoneDeBoor(n, knots + (i - static_cast<llong>(n)), points + (i - static_cast<llong>(n)), x);
        }
        return CalcBSplineZoneDeBoor(n, knots + (m - static_cast<llong>(n)), points + (m - static_cast<llong>(n)), x);
    }
}
