#include <bits/stdc++.h>
#include <x86intrin.h>
using namespace std;
using namespace numbers;



int main(){
	cin.tie(0)->sync_with_stdio(0);
	cin.exceptions(ios::badbit | ios::failbit);
	int n;
	cin >> n;
	// req, bal
	vector<array<int, 2>> a(n);
	vector<int> pos, neg;
	for(auto i = 0; i < n; ++ i){
		string s;
		cin >> s;
		for(auto c: s){
			if(c == '('){
				++ a[i][1];
			}
			else{
				-- a[i][1];
				a[i][0] = max(a[i][0], -a[i][1]);
			}
		}
		if(a[i][1] >= 0){
			pos.push_back(i);
		}
		else{
			neg.push_back(i);
		}
	}
	auto fail = [&]()->void{
		cout << "impossible\n";
		exit(0);
	};
	vector<int> res;
	int balance = 0;
	{
		ranges::sort(pos, [&](int i, int j){ return a[i][0] < a[j][0]; });
		for(auto i: pos){
			if(balance < a[i][0]){
				fail();
			}
			balance += a[i][1];
			res.push_back(i);
		}
	}
	{
		ranges::sort(neg, [&](int i, int j){ return a[i][0] + a[i][1] > a[j][0] + a[j][1]; });
		for(auto i: neg){
			if(balance < a[i][0]){
				fail();
			}
			balance += a[i][1];
			res.push_back(i);
		}
	}
	if(balance){
		fail();
	}
	for(auto i: res){
		cout << i + 1 << "\n";
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