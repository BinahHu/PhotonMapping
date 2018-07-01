#include "bezier.h"
#include <cmath>
#include <iostream>
using std::cout;
using std::endl;
using std::cin;


Intsct* Bezier::Intersect(const Ray& rO, const Ray& rt)
{
    double ut[2] = {0.0, 1.0};
    bool v = Gend0(rO, rt);
    if(!v)return NULL;
    int k[2], K;
    double e[2], eps, ult[2], lt;
    for(int i = 0; i < 2; i++)
    {
        ult[i] = INF;
        t = ut[i];
        for(k[i] = 0; k[i] < 20; k[i]++)
        {
            GenB(t);
            e[i] = iter();
            if(e[i] < EPS)break;
        }
        ut[i] = t;
        if(e[i] == -1)continue;
        if(k[i] == 20 && e[i] >= EPS)continue;
        Vector3 ud = d0 + pn * Pos.x;
        Vector3 udp = O - rO;
        ult[i] = (udp + Dx * Pos.x + ud).Norm();
    }
    if(ult[0] < ult[1])
    {
        t = ut[0];
        GenB(t);
        eps = e[0];
        K = k[0];
        lt = ult[0];
    }
    else
    {
        eps = e[1];
        K = k[1];
        lt = ult[1];
    }
    if(eps == -1)return NULL;
    if(K == 20 && eps >= EPS)return NULL;
    Vector3 d = d0 + pn * Pos.x;
    Vector3 dp = O - rO;

    Vector3 v1 = Dx.Cross(d);
    Vector3 v2 = GenDv(d);
    Vector3 N = v1.Cross(v2).Unit();
    Vector3 P = O + d + Dx * Pos.x;
    if((P - rO).Norm2() < EPS)
    {
        double pos = (rO - O - pn * Pos.x).Norm2() - d.Norm2();
        if(pos > 0 && rt.Dot(N) > EPS)  return NULL;
        if(pos < 0 && rt.Dot(-N) > EPS) return NULL;
    }

    if(lt < BEPS)return NULL;

    double delta = N.Dot(rt);
    if(delta > EPS)
        N = -N;
    P += N * EPS * 10;

    Intsct* intsct = new Intsct();
    intsct->dep = lt;
    intsct->P = P;
    intsct->N = N;
    intsct->I = rt.Unit();
    intsct->SetObj(this);
    return intsct;
}

void Bezier::Read(str var, ssm& finv)
{
    if      (var == "Deg")  finv >> n;
    else if (var == "Dx")   finv >> Dx;
    else if (var == "Dy")   finv >> Dy;
    else if (var == "O")    finv >> O;
    else if (var == "P")
    {
        Vector3 tmp;
        finv >> tmp;
        P.push_back(tmp);
    }
    else
        Object::Read(var, finv);
}

void Bezier::Init()
{
    B.resize(n+1, std::vector<double>(n+1));
    dB.resize(n+1, 0.0);
    t = 0.5;
    alpha = 1.0;
    len = 0.0;
    for(int i = 0; i <= n; i++)
    {
        double tmp = P[i].x;
        if(tmp > len)
            len = tmp;
    }
}

bool Bezier::Gend0(const Ray& rO, const Ray& rt)
{
    Vector3 dp = O - rO;
    Vector3 rn = rt.Unit();
    double dot = Dx.Dot(rn);

    if(std::fabs(dot) < EPS)return false;
    pn = rn / dot - Dx;
    d0 = rn * Dx.Dot(dp) / dot - dp;
    c1 = d0.Norm2();
    c2 = pn.Norm2();
    c3 = 2 * d0.Dot(pn);
    return true;

}

void Bezier::GenB(double t0)
{
    B[0][0] = 1.0;
    for(int j = 1; j <= n; j++)
        B[0][j] = B[0][j-1] * (1 - t0);
    dB[0] = -n * B[0][n-1];
    for(int i = 1; i <= n; i++)
    {
        for(int j = 0; j <= n; j++)
        {
            if(j == 0)
            {
                B[i][j] = 0.0;
                continue;
            }
            B[i][j] = (1 - t0) * B[i][j-1] + t0 * B[i-1][j-1];
        }
        dB[i] = n * (B[i-1][n-1] - B[i][n-1]);
    }
    Pos = Vector3();
    dPos = Pos;
    for(int i = 0; i <= n; i++)
    {
        Pos += P[i] * B[i][n];
        dPos += P[i] * dB[i];
    }
    
}

Vector3 Bezier::GenDv(const Vector3& d)
{
    GenB(t);
    double theta = acos(Dy.Dot(d) / (d.Norm()));
    if(Dy.Cross(d).Dot(Dx) < -EPS)
        theta = 2 * PI - theta;
    std::vector<Vector3> RP;
    for(int i = 0; i <= n; i++)
    {
        Vector3 tmp = Dx * P[i].x + Dy * P[i].y;
        RP.push_back(tmp.Rotate(Dx, theta));
    }

    Vector3 v = RP[0] * n * (-B[0][n-1]);
    for(int i = 1; i <= n; i++)
        v += RP[i] * n * (B[i-1][n-1] - B[i][n-1]);
    return v;
}

double Bezier::iter()
{
    double x = Pos.x, y = Pos.y;
    double dx = dPos.x, dy = dPos.y;
    double f = c1 + c2 * x * x + c3 * x - y * y;
    double df = 2 * c2 * x * dx + c3 * dx - 2 * y * dy;
    if(df < EPS && -df < EPS)
        cout << "Warning" << endl;
    double eps = f  / df * alpha;
    double abseps = std::fabs(eps);
    if(abseps < EPS)  return abseps;
    double tu = t - eps;
    if(tu < -EPS)       tu = 0.0;
    if(tu > 1 + EPS)    tu = 1.0;
    if((t == 1.0 && tu == 1.0)|| (t == 0.0 && tu == 0.0)) return -1.0;
    t = tu;
    return abseps;
}
