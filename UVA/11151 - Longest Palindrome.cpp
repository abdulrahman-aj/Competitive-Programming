#include <bits/stdc++.h>

using namespace std;

const int N = 1010;

string s;
int dp[N][N], n, t;

int solve(int l, int r) {
  assert(l <= r);
  if (dp[l][r] != -1) return dp[l][r];
  if (l == r) return dp[l][r] = 1;
  if (l + 1 == r) return dp[l][r] = s[l] == s[r] ? 2 : 1;
  dp[l][r] = max(solve(l + 1, r), solve(l, r - 1));
  if (s[l] == s[r])
    dp[l][r] = max(dp[l][r], 2 + solve(l + 1, r - 1));
  return dp[l][r];
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cin >> t;
  cin.ignore();
  while (t--) {
    memset(dp, -1, sizeof(dp));
    getline(cin, s);
    n = s.size();
    cout << (n == 0 ? 0 : solve(0, n - 1)) << '\n'; // empty string allowed
  }
  return 0;
}
