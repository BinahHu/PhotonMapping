#ifndef PHOTONMAP_H
#define PHOTONMAP_H
#include "typdef.h"
#include "photon.h"
#include "vector3.h"
#include "color.h"
#include "intsct.h"
#include <vector>
#include <algorithm>
#include <queue>

extern const int MAX_K;

struct KDT
{
	Photon* p;
	KDT* lc;
	KDT* rc;
	int axis;
	Vector3 c;
	KDT()	{lc = rc = NULL; p = NULL;}
	void clean();
	double Norm2() const				{	return p->pos.Dist2(c);}
	bool operator<(const KDT& k) const	{	return Norm2() < k.Norm2() - EPS;}
};

class Scene;
class Photonmap
{
	public:
		Photonmap(int _max_photon = 0, Scene* _scene = NULL);
		~Photonmap();
		void Store(Photon p);
		void Balance();
		KDT* SegBalance(Photon** p, int l, int r);
		Color GetColor(Intsct* intsct);
		void Detect(std::priority_queue<KDT>& q, KDT* node, const Vector3& P);
	private:
		Vector3 Box_min, Box_max;
		int max_photon, K;
		std::vector<Photon> photons;
		KDT* head;
		Scene* scene;
};

bool cmp(Photon* p1, Photon* p2);

#endif
