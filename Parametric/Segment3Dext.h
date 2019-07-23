#ifndef SEGMENT_3D_EXTENTION_H
#define SEGMENT_3D_EXTENTION_H

#include "..\Gauss\Segment3D.h"

class Segment3D_Ext : public Segment3D
{
private:
    struct Ratio { double x, y, z; bool isX, isY, isZ; }; // bool - no axis

    Vector3D getPointFromRario(Ratio ratio) const
    {
        Vector3D res;
        Vector3D dir = getDirection();
        if (ratio.isX)      res = start + dir * ratio.x;
        else if (ratio.isY) res = start + dir * ratio.y;
        else if (ratio.isZ) res = start + dir * ratio.z;
        else                res = start + dir * ratio.x; // any axis
        return res;
    }

    Ratio getRatio(const Vector3D &point) const
    {
        Vector3D diff = point - start;
        Vector3D dir = getDirection();
        Ratio ratio;

        ratio.isX = !equal_real(dir.getX(), 0);
        if (ratio.isX) ratio.x = diff.getX() / dir.getX();
        ratio.isY = !equal_real(dir.getY(), 0);
        if (ratio.isY) ratio.y = diff.getY() / dir.getY();
        ratio.isZ = !equal_real(dir.getZ(), 0);
        if (ratio.isZ) ratio.z = diff.getZ() / dir.getZ();
        return ratio;
    }

    bool isInsideSegment(const Vector3D &point) const
    {
        Ratio ratio = getRatio(point);
        Ratio ratio_start = getRatio(start);
        Ratio ratio_end = getRatio(end);
        bool res;
        if (ratio.isX)      res = less_or_equal_real(ratio_start.x, ratio.x) && less_or_equal_real(ratio.x, ratio_end.x);
        else if (ratio.isY) res = less_or_equal_real(ratio_start.y, ratio.y) && less_or_equal_real(ratio.y, ratio_end.y);
        else if (ratio.isZ) res = less_or_equal_real(ratio_start.z, ratio.z) && less_or_equal_real(ratio.z, ratio_end.z);
        else                res = less_or_equal_real(ratio_start.x, ratio.x) && less_or_equal_real(ratio.x, ratio_end.x); // any axis
        return res;
    }

    bool ratioIntersect(double a1, double a2, double b1, double b2, double dif1, double dif2, double &ratio) const
    {
        bool res = false; // mark empty ratio
        if (!equal_real(a2 * b1, a1 * b2)) { res = true; ratio = (a2 * dif1 - a1 * dif2) / (a2 * b1 - a1 * b2); } // check divide by zero
        return res;
    }

    Ratio getRatioIntersect(const Segment3D &s) const
    {
        Vector3D s1_v = getDirection();
        Vector3D s2_v = s.getDirection();
        Vector3D dif = start - s.getStart();
        Ratio ratio;
        ratio.isX = ratioIntersect(s1_v.getY(), s1_v.getZ(), s2_v.getY(), s2_v.getZ(), dif.getY(), dif.getZ(), ratio.x);
        ratio.isY = ratioIntersect(s1_v.getX(), s1_v.getZ(), s2_v.getX(), s2_v.getZ(), dif.getX(), dif.getZ(), ratio.y);
        ratio.isZ = ratioIntersect(s1_v.getX(), s1_v.getY(), s2_v.getX(), s2_v.getY(), dif.getX(), dif.getY(), ratio.z);
        return ratio;
    }

public:
    Segment3D_Ext(const Vector3D &_start, const Vector3D &_end) : Segment3D(_start, _end)
    {}

    Segment3D_Ext(const Segment3D &s) : Segment3D(s.getStart(), s.getEnd())
    {}

    int Intersect(const Segment3D_Ext &s, Vector3D &point) const
    {
        if (isCollinearity(s)) return -1; // lines belong to a common line
        if (isParallel(s)) return -2; // lines is parallel
        if (!isCoplanarity(s)) return 2; // not belong to a common surface
        Ratio ratio_s = getRatioIntersect(s);
        point = s.getPointFromRario(ratio_s);

        if (!isInsideSegment(point) || !s.isInsideSegment(point)) return 1;
        return 0;
    }
};

#endif // SEGMENT_3D_EXTENTION_H
