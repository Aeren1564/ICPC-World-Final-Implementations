#include <bits/stdc++.h>
// #include <x86intrin.h>
using namespace std;
using namespace numbers;

template<class T>
struct point{
	T x, y;
	point operator+(const point &p) const{
		return {x + p.x, y + p.y};
	}
	point operator-(const point &p) const{
		return {x - p.x, y - p.y};
	}
	T operator*(const point &p) const{
		return x * p.x + y * p.y;
	}
	T operator/(const point &p) const{
		return x * p.y - y * p.x;
	}
};
template<class T>
T square(T x){
	return x * x;
}
template<class T>
double dist(point<T> p, point<T> q){
	return sqrt(square(p.x - q.x) + square(p.y - q.y));
}
template<class T>
point<double> split(point<T> p, point<T> q, double t){
	return {p.x * (1 - t) + q.x * t, p.y * (1 - t) + q.y * t};
}
using pointll = point<long long>;
using pointd = point<double>;

int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	cout << fixed << setprecision(15);
	int ng, np;
	cin >> ng >> np;
	vector<vector<pointll>> gs(ng);
	for(auto i = 0; i < ng; ++ i){
		int n;
		cin >> n;
		gs[i].resize(n);
		for(auto &[x, y]: gs[i]){
			cin >> x >> y;
		}
	}
	vector<tuple<pointll, int, pointll, int>> line(np);
	for(auto i = 0; i < np; ++ i){
		pointll p, q;
		int zp, zq;
		cin >> p.x >> p.y >> zp >> q.x >> q.y >> zq;
		line[i] = {p, zp, q, zq};
	}
	double res = numeric_limits<double>::min();
	for(auto g: gs){
		double min_angle = numeric_limits<double>::max();
		for(auto [p, zp, q, zq]: line){
			double cur_angle = numeric_limits<double>::min();
			for(auto r: g){
				if((q - p) * (r - p) < 0 || (q - p) * (r - p) > (q - p) * (q - p)){
					goto NEXT;
				}
				double t = 1.0 * ((q - p) * (r - p)) / ((q - p) * (q - p));
				double z = lerp(zp, zq, t);
				double d = dist(pointd(r.x, r.y), split(p, q, t));
				cur_angle = max(cur_angle, atan2(d, z));
			}
			min_angle = min(min_angle, cur_angle);
			NEXT:;
		}
		if(min_angle == numeric_limits<double>::max()){
			cout << "impossible\n";
			return 0;
		}
		res = max(res, min_angle);
	}
	cout << res / pi * 180 << "\n";
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////