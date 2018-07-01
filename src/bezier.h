#ifndef BEZIER_H
#define BEZIER_H

#include "object.h"
#include <vector>

class Bezier: public Object
{
    public:
        virtual Intsct* Intersect(const Ray& rO, const Ray& rt);
        virtual void Read(str var, ssm& finv);
        virtual void Init();
    private:
        int n;
        double c1, c2, c3, t, alpha, len;
        Vector3 O, Dx, Dy, Pos, dPos, d0, pn;
        std::vector<Vector3> P;
        std::vector< std::vector<double> > B;
        std::vector<double> dB;
        bool Gend0(const Ray& rO, const Ray& rt);
        void GenB(double t0);
        Vector3 GenDv(const Vector3& d);
        double iter();
};


#endif
