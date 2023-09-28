#include <bits/stdc++.h>
// #include <x86intrin.h>
using namespace std;
using namespace numbers;

ostream &operator<<(ostream &out, __int128_t x){
	if(x < 0){
		return out << "-" << -x;
	}
	if(x == 0){
		return out << "0";
	}
	vector<int> a;
	while(x){
		a.push_back(x % 1000000000);
	}
	ranges::reverse(a);
	for(auto x: a){
		out << x;
	}
	return out;
}

__int128_t gcd(__int128_t x, __int128_t y){
	if(x < 0){
		x = -x;
	}
	if(y < 0){
		y = -y;
	}
	return x ? gcd(y % x, x) : y;
}

struct fraction{
	using T = __int128_t;
	T n = 0, d = 1;
	fraction(T n = 0, T d = 1): n(n), d(d){
		assert(d > 0);
	}
	fraction &reduce(){
		T g = gcd(n, d);
		n /= g;
		d /= g;
		return *this;
	}
	friend istream &operator>>(istream &in, fraction &f){
		int x;
		in >> x;
		f = {x, 1};
		return in;
	}
	friend ostream &operator<<(ostream &out, const fraction &f){
		return out << f.n << "/" << f.d;
	}
	bool operator==(const fraction &f) const{
		return n * f.d == d * f.n;
	}
	bool operator!=(const fraction &f) const{
		return n * f.d != d * f.n;
	}
	fraction operator+(const fraction &f) const{
		return fraction{n * f.d + d * f.n, d * f.d}.reduce();
	}
	fraction operator-(const fraction &f) const{
		return fraction({n * f.d - d * f.n, d * f.d}).reduce();
	}
	fraction operator*(const fraction &f) const{
		return fraction{n * f.n, d * f.d}.reduce();
	}
	fraction operator/(const fraction &f) const{
		assert(f.n);
		if(f.n > 0){
			return fraction{n * f.d, d * f.n}.reduce();
		}
		else{
			return fraction{-n * f.d, -d * f.n}.reduce();
		}
	}
	fraction operator-() const{
		return {-n, d};
	}
	bool operator<(const fraction &f) const{
		return n * f.d < d * f.n;
	}
	bool operator<=(const fraction &f) const{
		return n * f.d <= d * f.n;
	}
	bool operator>(const fraction &f) const{
		return n * f.d > d * f.n;
	}
	bool operator>=(const fraction &f) const{
		return n * f.d >= d * f.n;
	}
	explicit operator bool() const{
		return bool(n);
	}
	explicit operator double() const{
		return 1.0 * n / d;
	}
	friend double sqrt(const fraction &f){
		return sqrt(double(f));
	}
};

template<class T>
struct point{
	T x, y;
	friend ostream &operator<<(ostream &out, const point &p){
		return out << "{" << p.x << ", " << p.y << "}";
	}
	point operator+(const point &p) const{
		return {x + p.x, y + p.y};
	}
	point operator-(const point &p) const{
		return {x - p.x, y - p.y};
	}
	T operator*(const point &p) const{
		return {x * p.x + y * p.y};
	}
	T operator^(const point &p) const{
		return {x * p.y - y * p.x};
	}
	point operator*(T c) const{
		return {x * c, y * c};
	}
	point operator/(T c) const{
		return {x / c, y / c};
	}
	bool operator==(const point &p) const{
		return x == p.x && y == p.y;
	}
	bool operator!=(const point &p) const{
		return x != p.x || y != p.y;
	}
	friend T doubled_signed_area(const point &p, const point &q, const point &r){
		return q - p ^ r - p;
	}
	friend bool ccw(const point &p, const point &q, const point &r){
		return doubled_signed_area(p, q, r) > T();
	}
	friend bool cw(const point &p, const point &q, const point &r){
		return doubled_signed_area(p, q, r) < T();
	}
	friend bool colinear(const point &p, const point &q, const point &r){
		return doubled_signed_area(p, q, r) == T();
	}
	friend double dist(const point &p, const point &q){
		return sqrt((p.x - q.x) * (p.x - q.x) + (p.y - q.y) * (p.y - q.y));
	}
	// Returns the closest point from p on segment q-r
	friend point closest_point(const point &p, const point &q, const point &r){
		auto prod = (p - q) * (r - q);
		if(prod <= T()){
			return q;
		}
		if(prod >= (r - q) * (r - q)){
			return r;
		}
		return q + (r - q) * prod / ((r - q) * (r - q));
	}
	// closed segment p-q is contained in poly, including boundary, given that endpoints are already in it.
	friend bool contained_in(const point &p, const point &q, const vector<point> &poly){
		if(p == q){
			return true;
		}
		int n = (int)poly.size();
		for(auto u = 0; u < n; ++ u){
			auto r = poly[u], s = poly[(u + 1) % n];
			auto prod = q - p ^ s - r;
			auto x = r - p ^ q - p;
			auto y = s - r ^ p - r;
			if(prod < T()){
				prod = -prod;
				x = -x;
				y = -y;
			}
			if(T() < x && x < prod && T() <= y && y <= prod){
				// between r and s
				if(cw(r, s, p) || cw(r, s, q)){
					return false;
				}
			}
			if((s - p ^ s - q) == T() && (s - p) * (s - q) <= T()){
				// at point s
				auto t = poly[(u + 2) % n];
				if(ccw(p, q, r) && ccw(q, p, t)){
					if(s != p){
						return false;
					}
				}
				else if(ccw(p, q, t) && ccw(q, p, r)){
					if(s != q){
						return false;
					}
				}
				else if(ccw(p, q, r) || ccw(p, q, t)){
					if(ccw(r, s, t) && !(s == q && colinear(p, r, s)) && !(s == p && colinear(s, t, q))){
						return false;
					}
				}
				else{
					if(ccw(r, s, t) && !(s == p && colinear(s, r, q)) && !(s == q && colinear(p, t, s))){
						return false;
					}
				}
			}
		}
		return true;
	}
};

int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	cout << fixed << setprecision(15);
	int n;
	cin >> n;
	using P = point<fraction>;
	vector<P> a(n + 2);
	for(auto &[x, y]: a){
		cin >> x >> y;
	}
	auto from = a[n], to = a[n + 1];
	auto poly = a;
	poly.resize(n);
	const double inf = numeric_limits<double>::max() / 2;
	vector<double> d(n + 1, inf);
	{
		vector<int> done(n + 1, false);
		d[n] = 0;
		for(auto rep = n + 1; rep; -- rep){
			int u = -1;
			for(auto v = 0; v <= n; ++ v){
				if(!done[v] && (!~u || d[u] > d[v])){
					u = v;
				}
			}
			assert(~u);
			done[u] = true;
			for(auto v = 0; v <= n; ++ v){
				if(!done[v] && contained_in(a[u], a[v], poly)){
					d[v] = min(d[v], d[u] + dist(a[u], a[v]));
				}
			}
		}
	}
	double res = inf;
	for(auto s = 0; s <= n; ++ s){
		// inter, lblocked, rblocked
		vector<tuple<P, bool, bool>> inter;
		for(auto u = 0; u < n; ++ u){
			auto p = a[u], q = a[(u + 1) % n];
			if(colinear(to, a[s], p) && colinear(to, a[s], q)){
				continue;
			}
			if(colinear(to, a[s], p)){
				swap(p, q);
			}
			if(colinear(to, a[s], q)){
				if((a[s] - to) * (q - to) <= fraction()){
					continue;
				}
				if(ccw(to, a[s], p)){
					inter.push_back({q, true, false});
				}
				else{
					inter.push_back({q, false, true});
				}
			}
			else{
				auto prod = a[s] - to ^ q - p;
				auto x = p - to ^ a[s] - to;
				auto y = q - p ^ to - p;
				if(prod < fraction()){
					prod = -prod;
					x = -x;
					y = -y;
				}
				if(x < fraction() || prod < x || y < fraction()){
					continue;
				}
				inter.push_back({p + (q - p) / prod * x, true, true});
			}
		}
		assert(!inter.empty());
		ranges::sort(inter, [&](auto &x, auto &y){
			return (a[s] - to) * get<0>(x) < (a[s] - to) * get<0>(y);
		});
		for(auto i = 0; i < (int)inter.size(); ++ i){
			if(i){
				get<1>(inter[i]) |= get<1>(inter[i - 1]);
				get<2>(inter[i]) |= get<2>(inter[i - 1]);
			}
			if(get<1>(inter[i]) && get<2>(inter[i])){
				inter.resize(i + 1);
				break;
			}
		}
		auto end = get<0>(inter.back());
		for(auto u = 0; u <= n; ++ u){
			auto p = closest_point(a[u], to, end);
			if(contained_in(a[u], p, poly)){
				res = min(res, d[u] + dist(a[u], p));
			}
		}
	}
	cout << res << "\n";
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////