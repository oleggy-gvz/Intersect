#ifndef SEGMENT_3D_H
#define SEGMENT_3D_H

#include "Vector3D.h"

class Segment3D
{
protected:
    Vector3D start;
    Vector3D end;

public:
    Segment3D(const Vector3D &_start, const Vector3D &_end) : start(_start), end(_end)
    {
        if (getDirection() == Vector3D(0, 0 ,0)) throw Exception(Exception::SEGMENT_EQUAL_POINTS);
    }
    bool isCollinearity(const Segment3D &s) const // belong to a line of segment
    {
        Vector3D vec1 = s.start - start;
        Vector3D vec2 = s.end - start;
        Vector3D dir = getDirection();
        return vec1.isCollinearity(dir) && vec2.isCollinearity(dir);
    }
    bool isParallel(const Segment3D &s) const { return getDirection().isCollinearity(s.getDirection()); }
    Vector3D getStart() const { return start; }
    bool isLyingOnSegment(const Vector3D &point) const
    {
        Vector3D vec1 = point - start;
        Vector3D vec2 = point - end;
        if (!getDirection().isCollinearity(vec1)) return false;
        double len = getDirection().getLenght();
        return more_or_equal_real(len, vec1.getLenght()) && more_or_equal_real(len, vec2.getLenght());
    }
    double getLeght() const
    {
        return getDirection().getLenght();
    }
    Vector3D getEnd() const { return end; }
    Vector3D getDirection() const { return end - start; }
    bool isCoplanarity(const Segment3D &line) const // belong to a common 2D surface
    {
        Vector3D vec = line.start - start;
        return equal_real(vec.mixed_multi(getDirection(), line.getDirection()), 0);
    }
    friend ostream& operator<<(ostream &, const Segment3D &);
};

ostream& operator<<(ostream &out, const Segment3D &s)
{
    out << s.start << " -> " << s.end;
    return out;
}

#endif // SEGMENT_3D_H
