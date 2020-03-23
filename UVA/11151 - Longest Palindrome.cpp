#include <bits/stdc++.h>

using namespace std;

const int N = 1010;
int dp[N][N], n;

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  cin.ignore();
  while (t--) {
    string s;
    getline(cin, s);
    n = s.size();
    if (n == 0) {
      cout << 0 << '\n';
      continue;
    }
    for (int i = 0; i < n; ++i)
      dp[i][i] = 1;
    for (int i = 0; i < n - 1; ++i)
      dp[i][i + 1] = s[i] == s[i + 1] ? 2 : 1;
    for (int l = n - 3; l >= 0; --l) {
      for (int r = l + 2; r < n; ++r) {
        dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
        if (s[l] == s[r])
          dp[l][r] = max(dp[l][r], 2 + dp[l + 1][r - 1]);
      }
    }
    cout << dp[0][n - 1] << '\n';
  }
  return 0;
}