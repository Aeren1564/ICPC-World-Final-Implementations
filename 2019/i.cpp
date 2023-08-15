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
	int r, c, nproc, nprog;
	cin >> r >> c >> nproc >> nprog;
	vector<string> a(r);
	copy_n(istream_iterator<string>(cin), r, a.begin());
	array<string, 26> proc;
	for(auto i = 0; i < nproc; ++ i){
		string s;
		cin >> s;
		proc[s[0] - 'A'] = s.substr(2);
	}
	static const vector<pair<int, int>> dr{{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
	string pool = "senw";
	vector<int> convert(300);
	for(auto i = 0; i < 4; ++ i){
		convert[pool[i]] = i;
	}
	auto next = [&](int i, int j, int dir)->array<int, 2>{
		assert(0 <= min(i, j) && i < r && j < c && a[i][j] != '#');
		auto [di, dj] = dr[dir];
		int ni = i + di, nj = j + dj;
		return 0 <= min(ni, nj) && ni < r && nj < c && a[ni][nj] == '.' ? array{ni, nj} : array{i, j};
	};
	auto blocked = [&](int i, int j, int dir)->bool{
		assert(0 <= min(i, j) && i < r && j < c && a[i][j] != '#');
		auto [ni, nj] = next(i, j, dir);
		return i == ni && j == nj;
	};
	vector mem(r, vector(c, vector<map<string, array<int, 3>>>(4)));
	set<tuple<int, int, int, string>> stack;
	auto find_closing_bracket = [&](const string &s, int p)->int{
		assert(0 <= p && p < (int)s.size() && s[p] == '(');
		int balance = 1;
		while(p < (int)s.size() && balance){
			++ p;
			if(s[p] == '('){
				++ balance;
			}
			else if(s[p] == ')'){
				-- balance;
			}
		}
		assert(p < (int)s.size() && s[p] == ')');
		return p;
	};
	auto run = [&](auto self, int ci, int cj, int cdir, const string &s)->array<int, 3>{
		int i = ci, j = cj, dir = cdir;
		if(!~i){
			return {i, j, dir};
		}
		assert(0 <= min(i, j) && i < r && j < c && a[i][j] != '#');
		if(mem[i][j][dir].contains(s)){
			return mem[i][j][dir][s];
		}
		if(stack.contains({i, j, dir, s})){
			return {-1, -1, -1};
		}
		stack.insert({i, j, dir, s});
		for(auto p = 0; ~i && p < (int)s.size(); ){
			if(s[p] == 'm'){
				auto [ni, nj] = next(i, j, dir);
				i = ni, j = nj;
				++ p;
			}
			else if(s[p] == 'l'){
				dir = dir + 1 & 3;
				++ p;
			}
			else if(isupper(s[p])){
				auto [ni, nj, ndir] = self(self, i, j, dir, proc[s[p] - 'A']);
				i = ni, j = nj, dir = ndir;
				++ p;
			}
			else if(s[p] == 'i'){
				++ p;
				char condition = s[p];
				++ p;
				string sfirst, ssecond;
				{
					assert(s[p] == '(');
					int q = find_closing_bracket(s, p);
					sfirst = s.substr(p + 1, q - p - 1);
					p = q + 1;
				}
				{
					assert(s[p] == '(');
					int q = find_closing_bracket(s, p);
					ssecond = s.substr(p + 1, q - p - 1);
					p = q + 1;
				}
				if(condition == 'b'){
					auto [ni, nj, ndir] = self(self, i, j, dir, blocked(i, j, dir) ? sfirst : ssecond);
					i = ni, j = nj, dir = ndir;
				}
				else{
					auto [ni, nj, ndir] = self(self, i, j, dir, dir == convert[condition] ? sfirst : ssecond);
					i = ni, j = nj, dir = ndir;
				}
			}
			else if(s[p] == 'u'){
				++ p;
				char condition = s[p];
				++ p;
				string snext;
				{
					assert(s[p] == '(');
					int q = find_closing_bracket(s, p);
					snext = s.substr(p + 1, q - p - 1);
					p = q + 1;
				}
				if(condition == 'b'){
					if(!blocked(i, j, dir)){
						auto [ni, nj, ndir] = self(self, i, j, dir, snext);
						i = ni, j = nj, dir = ndir;
						{
							auto [ni, nj, ndir] = self(self, i, j, dir, string("u") + condition + "(" + snext + ")");
							i = ni, j = nj, dir = ndir;
						}
					}
				}
				else{
					if(dir != convert[condition]){
						auto [ni, nj, ndir] = self(self, i, j, dir, snext);
						i = ni, j = nj, dir = ndir;
						{
							auto [ni, nj, ndir] = self(self, i, j, dir, string("u") + condition + ("(" + snext + ")"));
							i = ni, j = nj, dir = ndir;
						}
					}
				}
			}
			else{
				assert(false);
			}
		}
		stack.erase({ci, cj, cdir, s});
		return mem[ci][cj][cdir][s] = {i, j, dir};
	};
	for(auto ip = 0; ip < nprog; ++ ip){
		int i, j;
		char type;
		string prog;
		cin >> i >> j >> type >> prog, -- i, -- j;
		int dir = convert[type];
		auto [ni, nj, ndir] = run(run, i, j, dir, prog);
		if(!~ni){
			cout << "inf\n";
		}
		else{
			cout << ni + 1 << " " << nj + 1 << " " << pool[ndir] << "\n";
		}
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
