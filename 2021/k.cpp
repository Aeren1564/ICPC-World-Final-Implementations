#include <bits/stdc++.h>
// #include <x86intrin.h>
using namespace std;
using namespace numbers;
template<class T, class U>
T &ctmax(T &x, U y){
	return x = max<T>(x, y);
}

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
	point operator*(T c) const{
		return {c * x, c * y};
	}
	T operator^(const point &p) const{
		return x * p.y - y * p.x;
	}
	T squared_norm() const{
		return x * x + y * y;
	}
	bool operator!=(const point &p) const{
		return x != p.x || y != p.y;
	}
	bool operator==(const point &p) const{
		return x == p.x && y == p.y;
	}
	point perp() const{
		return {-y, x};
	}
};

int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	int n;
	cin >> n;
	using P = point<long long>;
	vector<P> a(n);
	vector<vector<int>> adj(n);
	for(auto u = 0; u < n; ++ u){
		int deg;
		cin >> deg;
		if(deg){
			adj[u].resize(deg);
			for(auto &v: adj[u]){
				cin >> v, -- v;
			}
		}
		else{
			cin >> a[u].x >> a[u].y;
		}
	}
	const P init{numeric_limits<int>::max(), numeric_limits<int>::max()};
	// Returns the extreme point in dir and -dir direction, where tie is broken by being on the left.
	auto solve_for = [&](const P &dir)->array<P, 2>{
		static vector<array<P, 2>> dp(n);
		for(auto u = n - 1; u >= 0; -- u){
			if(adj[u].empty()){
				dp[u] = {a[u], a[u]};
				continue;
			}
			array<P, 2> sum{};
			for(auto v: adj[u]){
				for(auto t = 0; t < 2; ++ t){
					sum[t] = sum[t] + dp[v][t];
				}
			}
			dp[u] = {init, init};
			for(auto v: adj[u]){
				array<P, 2> cur{
					dp[v][0] + dp[v][1] - sum[1],
					dp[v][0] + dp[v][1] - sum[0]
				};
				if(dp[u][0] == init || dp[u][0] * dir < cur[0] * dir){
					dp[u][0] = cur[0];
				}
				if(dp[u][1] == init || dp[u][1] * dir > cur[1] * dir){
					dp[u][1] = cur[1];
				}
			}
		}
		return dp[0];
	};
	auto [p, q] = solve_for({-1, 0});
	long long res = max(p.squared_norm(), q.squared_norm());
	if(p != q){
		auto solve = [&](auto self, const P &p, const P &q)->void{
			auto r = solve_for((p - q).perp())[0];
			if(r == p || r == q){
				return;
			}
			ctmax(res, r.squared_norm());
			self(self, p, r);
			self(self, r, q);
		};
		solve(solve, p, q);
		solve(solve, q, p);
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