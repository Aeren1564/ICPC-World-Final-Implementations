#include <bits/stdc++.h>
using namespace std;
#if __cplusplus > 201703L
#include <ranges>
using namespace numbers;
#endif



int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	int n;
	cin >> n;
	vector<array<int, 3>> a(n), b(n);
	for(auto rep = 2; rep; -- rep){
		for(auto j = 0; j < 2; ++ j){
			for(auto i = 0; i < n; ++ i){
				cin >> a[i][j];
				a[i][2] = i;
			}
		}
		ranges::sort(a);
		swap(a, b);
	}
	vector<array<int, 2>> res;
	// h, i
	multiset<pair<int, int>> rema;
	multiset<pair<int, int>, greater<>> remb;
	for(auto al = 0, ar = 0, bl = 0, br = 0; ar < n || br < n; ){
		if(ar <= br){
			al = ar;
			while(ar < n && a[al][0] == a[ar][0]){
				rema.insert({a[ar][1], a[ar][2]});
				++ ar;
			}
		}
		else{
			bl = br;
			while(br < n && b[bl][0] == b[br][0]){
				remb.insert({b[br][1], b[br][2]});
				++ br;
			}
		}
		if(ar <= br){
			for(auto [h, i]: rema){
				auto it = remb.upper_bound({h, numeric_limits<int>::min()});
				if(it == remb.end()){
					cout << "impossible\n";
					return 0;
				}
				res.push_back({i, it->second});
				remb.erase(it);
			}
			rema.clear();
		}
		else{
			for(auto [h, i]: remb){
				auto it = rema.upper_bound({h, numeric_limits<int>::max()});
				if(it == rema.end()){
					cout << "impossible\n";
					return 0;
				}
				res.push_back({it->second, i});
				rema.erase(it);
			}
			remb.clear();
		}
	}
	for(auto j = 0; j < 2; ++ j){
		for(auto i = 0; i < n; ++ i){
			cout << res[i][j] + 1 << " ";
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