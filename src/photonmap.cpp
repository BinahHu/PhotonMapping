#include "photonmap.h"
#include <cstdlib>
#include "object.h"
#include <cmath>
#include "scene.h"

const int MAX_K = 40;
int Axis = 0;

void KDT::clean()
{
    if(lc){ delete lc;  lc = NULL;}
    if(rc){ delete rc;  rc = NULL;}
    p = NULL;
}

Photonmap::Photonmap(int _max_photon, Scene* _scene)
{
    Box_max = Vector3(-INF, -INF, -INF);
    Box_min = Vector3(INF, INF, INF);
    max_photon = _max_photon;
    K = MAX_K;
    head = NULL;
    scene = _scene;
}

Photonmap::~Photonmap()
{
    if(head)    {head->clean(); delete head;}
    head = NULL;
}

void Photonmap::Store(Photon p)
{
    if(photons.size() >= max_photon)return;
    photons.push_back(p);
    Box_max = Vector3(std::max(Box_max.x, p.pos.x), std::max(Box_max.y, p.pos.y), std::max(Box_max.z, p.pos.z));
    Box_min = Vector3(std::min(Box_min.x, p.pos.x), std::min(Box_min.y, p.pos.y), std::min(Box_min.z, p.pos.z));
    return;
}

void Photonmap::Balance()
{
    std::cout << "Stored Photons = " << photons.size() << std::endl;
    Photon** p = new Photon*[photons.size()];
    for(int i = 0; i < photons.size(); i++)
            p[i] = &photons[i];
    head = SegBalance(p, 0, photons.size());
}

bool cmp(Photon* p1, Photon* p2)
{
    return p1->pos.Project(Axis) < p2->pos.Project(Axis) - EPS;
}

KDT* Photonmap::SegBalance(Photon** p, int l, int r)
{
    if(l >= r)return NULL;
    int mid = (l + r) / 2;
    KDT* node = new KDT();
    int axis = 1;
    if(Box_max.y - Box_min.y > Box_max.x - Box_min.x && Box_max.y - Box_min.y > Box_max.z - Box_min.z)  axis = 2;
    if(Box_max.z - Box_min.z > Box_max.x - Box_min.x && Box_max.z - Box_min.z > Box_max.y - Box_min.y)  axis = 3;
    Axis = axis;
    std::nth_element(p + l, p + mid, p + r, cmp);

    node->p = p[mid];
    node->axis = axis;

    double tmp = Box_max.Project(axis);
    Box_max.Project(axis) = p[mid]->pos.Project(axis);
    node->lc = SegBalance(p, l, mid);
    Box_max.Project(axis) = tmp;

    tmp = Box_min.Project(axis);
    Box_min.Project(axis) = p[mid]->pos.Project(axis);
    node->rc = SegBalance(p, mid + 1, r);
    Box_min.Project(axis) = tmp;

    return node;
}

Color Photonmap::GetColor(Intsct* intsct)
{
    std::priority_queue<KDT> q;
    head->c = intsct->P;

    Detect(q, head, intsct->P);

    Color ret;

    double maxdis = -1;
    double coef = scene->GetCamera()->GetCOEF();
    for(int i = 0; i < K; i++)
    {
        Photon* tmp = q.top().p;
        if(tmp->dir.Dot(intsct->N) < -EPS)
        {
            if(maxdis < 0)
                maxdis = intsct->P.Dist2(tmp->pos) * coef;
            double BRDF = intsct->GetObj()->GetMaterial()->BRDF(tmp->dir, intsct->N, -intsct->I);
            ret += tmp->color * BRDF;
        }
        q.pop();
    }
    if(maxdis > 0)ret /= maxdis;
    ret = ret * intsct->GetObj()->GetColor(intsct->P);
    return ret;
}

void Photonmap::Detect(std::priority_queue<KDT>& q, KDT* node, const Vector3& P)
{
    if(node->lc == NULL && node->rc == NULL)
    {
        if(q.size() < K)
            q.push(*node);
        else if(node->Norm2() < q.top().Norm2())
        {
            q.pop();
            q.push(*node);
        }
        return;
    }
    double dist = P.Project(node->axis) - node->p->pos.Project(node->axis);
    if(dist < 0)
    {
        if(node->lc)
        {
            node->lc->c = P;
            Detect(q, node->lc, P);
        }
        if(q.size() < K)
            q.push(*node);
        else if(node->Norm2() < q.top().Norm2())
        {
            q.pop();
            q.push(*node);
        }
        double dist2 = dist * dist;
        if(node->rc && dist2 < q.top().Norm2())
        {
            node->rc->c = P;
            Detect(q, node->rc, P);
        }
    }
    else
    {
        if(node->rc)
        {
            node->rc->c = P;
            Detect(q, node->rc, P);
        }
        if(q.size() < K)
            q.push(*node);
        else if(node->Norm2() < q.top().Norm2())
        {
            q.pop();
            q.push(*node);
        }
        double dist2 = dist * dist;
        if(node->lc && dist2 < q.top().Norm2())
        {
            node->lc->c = P;
            Detect(q, node->lc, P);
        }
    }
}
