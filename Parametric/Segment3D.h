#ifndef SEGMENT_3D_H
#define SEGMENT_3D_H

#include "..\Gauss\Vector3D.h"

class Segment3D
{
private:
    Vector3D start;
    Vector3D end;

    struct Ratio { double x = 0, y = 0, z = 0; bool isX = true, isY = true, isZ = true; }; // bool - no axis

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
        ratio.x = ratio.isX ? diff.getX() / dir.getX() : -1;
        ratio.isY = !equal_real(dir.getY(), 0);
        ratio.y = ratio.isY ? diff.getY() / dir.getY() : -1;
        ratio.isZ = !equal_real(dir.getZ(), 0);
        ratio.z = ratio.isZ ? diff.getZ() / dir.getZ() : -1;
        return ratio;
    }

    bool isInsideSegment(const Vector3D &point) const
    {
        Ratio ratio = getRatio(point);
        Ratio ratio_start = getRatio(start);
        Ratio ratio_end = getRatio(end);
        bool res;
        if (ratio.isX)
        {
            //res = ratio_start.x <= ratio.x && ratio.x <= ratio_end.x;
            // TODO: отдельная функция больше или равно / меньше или равно
            res = (equal_real(ratio_start.x, ratio.x) || ratio_start.x > ratio.x) && (equal_real(ratio.x, ratio_end.x) || ratio.x > ratio_end.x);
        }
        else if (ratio.isY) res = ratio_start.y <= ratio.y && ratio.y <= ratio_end.y;
        else if (ratio.isZ) res = ratio_start.z <= ratio.z && ratio.z <= ratio_end.z;
        else                res = ratio_start.x <= ratio.x && ratio.x <= ratio_end.x; // any axis
        return res;
    }

    bool ratioIntersect(double a1, double a2, double b1, double b2, double dif1, double dif2, double &ratio) const
    {
        bool res = false; ratio = -1; // mark empty ratio
        if (!equal_real(a2 * b1, a1 * b2)) { res = true; ratio = (a2 * dif1 - a1 * dif2) / (a2 * b1 - a1 * b2); } // check divide by zero
        return res;
    }

    Ratio getRatioIntersect(const Segment3D &s) const
    {
        Vector3D s1_v = getDirection();
        Vector3D s2_v = s.getDirection();
        Vector3D dif = start - s.start;
        Ratio ratio;
        ratio.isX = ratioIntersect(s1_v.getY(), s1_v.getZ(), s2_v.getY(), s2_v.getZ(), dif.getY(), dif.getZ(), ratio.x);
        ratio.isY = ratioIntersect(s1_v.getX(), s1_v.getZ(), s2_v.getX(), s2_v.getZ(), dif.getX(), dif.getZ(), ratio.y);
        ratio.isZ = ratioIntersect(s1_v.getX(), s1_v.getY(), s2_v.getX(), s2_v.getY(), dif.getX(), dif.getY(), ratio.z);
        return ratio;
    }

public:
    Segment3D(const Vector3D &_start, const Vector3D &_end) : start(_start), end(_end)
    {
        if (getDirection() == Vector3D(0, 0 ,0)) throw Exception("start and end points of the segment must be different");
    }
    bool isCollinearity(const Segment3D &s) const // belong to a line of segment
    {
        Vector3D vec1 = s.start - start;
        Vector3D vec2 = s.end - start;
        return vec1.isCollinearity(getDirection()) && vec2.isCollinearity(getDirection());
    }
    bool isParallel(const Segment3D &s) const { return getDirection().isCollinearity(s.getDirection()); }
    Vector3D getDirection() const { return end - start; }
    bool isCoplanarity(const Segment3D &s) const // belong to a common surface
    {
        Vector3D vec = s.start - start;
        return equal_real(vec.mixed_multi(getDirection(), s.getDirection()), 0);
    }
    int Intersect(const Segment3D &s, Vector3D &point) const
    {
        if (isCollinearity(s)) return -1; // lines belong to a common line
        if (isParallel(s)) return -2; // lines is parallel
        if (!isCoplanarity(s)) return 2; // not belong to a common surface
        Ratio ratio_s = getRatioIntersect(s);
        point = s.getPointFromRario(ratio_s);
        if (!isInsideSegment(point) || !s.isInsideSegment(point)) return 1;
        return 0;
    }
    friend ostream& operator<<(ostream &, const Segment3D &);
};

ostream& operator<<(ostream &out, const Segment3D &s) { out << s.start << " -> " << s.end; return out; }

#endif // SEGMENT_3D_H
