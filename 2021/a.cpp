#include <x86intrin.h>
#include <bits/stdc++.h>
using namespace std;
#if __cplusplus > 201703L
#include <ranges>
using namespace numbers;
#endif



int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	string pool = ".#";
	int r, c, n;
	cin >> r >> c >> n;
	vector res(r, string(c, '?'));
	auto ind = [&](int x, int y)->int{
		return c * x + y;
	};
	vector<array<vector<int>, 2>> adj(r * c);
	for(auto i = 0; i < n; ++ i){
		int dx, dy, m;
		cin >> dx >> dy >> m;
		static vector state(r, vector<int>(c, -1));
		for(auto j = 0; j < m; ++ j){
			int x, y;
			cin >> x >> y, -- x, -- y;
			state[x][y] = i;
		}
		auto valid = [&](int x, int y)->int{
			return 0 <= min(x, y) && x < r && y < c;
		};
		for(auto sx = 0; sx < r; ++ sx){
			for(auto sy = 0; sy < c; ++ sy){
				if(valid(sx - dx, sy - dy)){
					continue;
				}
				int x = sx, y = sy;
				while(valid(x, y) && state[x][y] != i){
					res[x][y] = '.';
					x += dx, y += dy;
				}
				if(!valid(x, y)){
					continue;
				}
				while(valid(x, y)){
					assert(state[x][y] == i);
					res[x][y] = '#';
					if(valid(x - dx, y - dy)){
						res[x - dx][y - dy] = '.';
					}
					assert(!valid(x + dx, y + dy) || state[x + dx][y + dy] != i);
					int nx = x + 2 * dx, ny = y + 2 * dy;
					while(valid(nx, ny) && state[nx][ny] != i){
						adj[ind(nx - dx, ny - dy)][0].push_back(ind(nx, ny));
						adj[ind(nx, ny)][1].push_back(ind(nx - dx, ny - dy));
						nx += dx, ny += dy;
					}
					x = nx, y = ny;
				}
			}
		}
	}
	vector<vector<int>> vis(r, vector<int>(c));
	for(auto x = 0; x < r; ++ x){
		for(auto y = 0; y < c; ++ y){
			if(vis[x][y] || res[x][y] == '?'){
				continue;
			}
			auto update = [&](auto self, int x, int y)->void{
				vis[x][y] = true;
				for(auto v: adj[ind(x, y)][res[x][y] == '#']){
					auto [nx, ny] = div(v, c);
					if(vis[nx][ny]){
						continue;
					}
					res[nx][ny] = res[x][y];
					self(self, nx, ny);
				}
			};
			update(update, x, y);
		}
	}
	{
		for(auto x = 0; x < c; ++ x){
			for(auto y = 0; y < r; ++ y){
				res[y][x] == '#' ? cout << '#' : cout << '.';
			}
			cout << "\n";
		}
		cout << "\n";
	}
	{
		for(auto x = 0; x < c; ++ x){
			for(auto y = 0; y < r; ++ y){
				res[y][x] == '.' ? cout << '.' : cout << '#';
			}
			cout << "\n";
		}
		cout << "\n";
	}
	return 0;
}

/*

*/

////////////////////////////////////////////////////////////////////////////////////////
//                                                                                    //
//                                   Coded by Aeren                                   //
//                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////
