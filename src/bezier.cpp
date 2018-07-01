#include "bezier.h"
#include <cmath>
#include <iostream>
using std::cout;
using std::endl;
using std::cin;


Intsct* Bezier::Intersect(const Ray& rO, const Ray& rt)
{
    //To be continue...
    bool flag = (std::fabs((O - Vector3(rO.x,rO.y,-5)).Norm() - 1) < EPS);
    flag = false;
    if(flag)cout << "Self" << endl;
    t = 0.5;
    double x0 = 0.0;
    bool v = Gend0(rO, rt);
    if(!v)
    {
        if(flag)cout << "Rej fot V" << endl;
        return NULL;
        x0 = Dx.Dot(rO - O);
        if(x0 < 0.0 || x0 > len)return NULL;
    }
    int k;
    double eps;
    for(k = 0; k < 20; k++)
    {
        GenB(t);
        if(v)   eps = iter();
        else    eps = iterx(x0);
        if(eps < EPS)break;
    }
    if(flag)cout << "Rej for not Int" << endl;
    if(eps == -1)return NULL;
    if(k == 20 && eps >= EPS)return NULL;
    Vector3 d = d0 + pn * Pos.x;
    Vector3 dp = O - rO;
    double lt = (dp + Dx * Pos.x + d).Norm();
    if(!v)
    {
        lt = rt.Dot(dp);
        d = rt * lt - dp;
        double delta = d.Norm2() - Pos.y * Pos.y;
        if(std::fabs(delta) < EPS)return NULL;
    }
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
    if(flag)cout << "Rej for neg" << endl;
    if(lt < BEPS && !flag)return NULL;
    if(flag)cout << "Acp" << endl;


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
    if(!flag)return intsct;
    cout << "Show rO" << endl;
    rO.Show();
    cout << "Show rt" << endl;
    rt.Show();
    cout << "dep = " << lt << endl;
    cout << "Show P" << endl;
    intsct->P.Show();
    cout << "Show N" << endl;
    intsct->N.Show();
    cout << "Show d0" << endl;
    d0.Show();
    cout << "Show pn" << endl;
    pn.Show();
    cout << "Show Pos" << endl;
    Pos.Show();
    cout << "Show d" << endl;
    d.Show();
    cout << "Show O" << endl;
    O.Show();
    cout << "Show v1" << endl;
    v1.Show();
    cout << "Show v2" << endl;
    v2.Show();
    double x = Pos.x, y = Pos.y;
    double f = c1 + c2 * x * x + c3 * x - y * y;
    cout << "f = " << f << endl;
    cout << "c1 = " << c1 << " c2 = " << c2 << " c3 = " << c3 << endl;
    cout << "t = " << t << endl;
    cout << "x = " << x << endl;
    cout << "y = " << y << endl;
    cout << "eps = " << eps << endl;
    cin.get();

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
    /*if(std::fabs(dot) >= EPS && rt.z < -1.0)
    {
    cout << "Show rO" << endl;
    rO.Show();
    cout << "Show dp" << endl;
    dp.Show();
    cout << "Show rt" << endl;
    rt.Show();
    cout << "Show rn" << endl;
    rn.Show();
    cout << "dot = " << dot << endl;
    cin.get();
    }*/
    if(std::fabs(dot) < EPS)return false;
    pn = rn / dot - Dx;
    d0 = rn * Dx.Dot(dp) / dot - dp;
    c1 = d0.Norm2();
    c2 = pn.Norm2();
    c3 = 2 * d0.Dot(pn);
    return true;
    /*
    cout << "c1 = " << c1 << " c2 = " << c2 << " c3 = " << c3 << endl;
    cout << "Show pn:" << endl;
    pn.Show();
    cout << "Show d0" << endl;
    d0.Show();
    */
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
    /*
    for(int i = 0; i <= n; i++)
    for(int j = 0; j <= n; j++)
    cout << "B[" << i << "][" << j << "] = " << B[i][j] << (j == n ? '\n' : '\t');
    cout << "t = " << t0 << endl;
    cout << "Show Pos" << endl;
    Pos.Show();
    cin.get();
    cout << "Show d" << endl;
    (d0 + pn * Pos.x).Show();
    */
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
    /*
    cout << "theta = " << theta << endl;
    RP[0].Show();
    RP[1].Show();
    RP[2].Show();
    RP[3].Show();
    for(int i = 0; i <= n; i++)
    for(int j = 0; j <= n; j++)
    cout << "B[" << i << "][" << j << "] = " << B[i][j] << (j == n ? '\n' : '\t');
    */
    Vector3 v = RP[0] * n * (-B[0][n-1]);
    for(int i = 1; i <= n; i++)
        v += RP[i] * n * (B[i-1][n-1] - B[i][n-1]);
    return v;
}

double Bezier::iterx(double x0)
{
    double x = Pos.x, dx = dPos.x;
    double f = x - x0;
    double df = dx;
    if(dx < EPS && -df < EPS)
        cout << "Warning" << endl;
    double eps = f / df * alpha;
    double abseps = std::fabs(eps);
    if(abseps < EPS)  return abseps;
    double tu = t - eps;
    if(tu < -EPS)       tu = 0.0;
    if(tu > 1 + EPS)    tu = 1.0;
    if((t == 1.0 && tu == 1.0)|| (t == 0.0 && tu == 0.0)) return -1.0;
    t = tu;
    return abseps;
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
