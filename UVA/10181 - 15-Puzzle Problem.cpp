#include "bits/stdc++.h"

using namespace std;

using T = unsigned long long;

class Node {
 public:
  T state;
  T parent;
  char action;
  int cost;
  int h;
  Node(T a, T b, char c, int d, int e) {
    state = a;
    parent = b;
    action = c;
    cost = d;
    h = e;
  }
  bool operator<(const Node& rhs) const { return cost + h > rhs.cost + rhs.h; }  
};

const char* dir = "LRUD";
const int dr[] = { 0,  0, -1, +1};
const int dc[] = {-1, +1,  0,  0};

const int LENGTH = 4;
const int SIZE = LENGTH * LENGTH;
const int BITS = 4;
const T GOAL = 17134975606245761295;

bool solvable(vector<int>& p) {
  int inv = 0;
  for (int i = 0; i < SIZE; ++i) {
    for (int j = i + 1; j < SIZE; ++j) {
      if (p[i] && p[j] && p[i] > p[j])
        ++inv;
    }
  }
  if (LENGTH % 2 == 1) {
    return inv % 2 == 0;
  } else {
    int row = LENGTH - ((find(p.begin(), p.end(), 0) - p.begin()) / LENGTH);
    return row % 2 != inv % 2;
  }
}

inline bool valid(int x, int y) { return x >= 0 && x < LENGTH && y >= 0 && y < LENGTH; }

T myHash(vector<int>& a) {
  T ans = 0;
  for (int i = 0; i < SIZE; ++i) {
    ans |= (T)i << (a[i] * BITS);
  }
  return ans;
}

int manhattanDistance(T a, T b) {
  int ans = 0;
  a >>= BITS;
  b >>= BITS;
  for (int i = 0; i < SIZE - 1; ++i) {
    int pos1 = a & ((1 << BITS) - 1);
    int pos2 = b & ((1 << BITS) - 1);
    ans += abs(pos1 / LENGTH - pos2 / LENGTH) + abs(pos1 % LENGTH - pos2 % LENGTH);
    a >>= BITS;
    b >>= BITS;
  }
  return ans;
}

vector<pair<T, int>> actions(T t) {
  vector<pair<T, int>> ans;

  int pos = t & ((1 << BITS) - 1);
  int row = pos / LENGTH;
  int col = pos % LENGTH;
  
  for (int d = 0; d < 4; ++d) {
    int nrow = row + dr[d];
    int ncol = col + dc[d];

    if (valid(nrow, ncol)) {
      int npos = nrow * LENGTH + ncol;
      int loc = -1;
      for (int i = 0; i < SIZE; ++i) {
        if ((int)((t >> (i * BITS)) & ((1 << BITS) - 1)) == npos) {
          loc = i;
          break;
        }
      }
      assert(loc != -1);

      T to = t;
      to &= ~((1 << BITS) - 1);
      to &= ~(((T(1) << BITS) - 1) << (loc * BITS));
      to |= npos;
      to |= (T)pos << (loc * BITS);

      ans.emplace_back(to, d);
    }
  }

  return ans;
}

string solve(T src) {
  priority_queue<Node> q[2];
  map<T, T> parent[2];
  map<T, char> action[2];
  T goal[2] = {GOAL, src};

  // $ -> no action
  q[0].emplace(src, 0, '$', 0, manhattanDistance(src, GOAL));
  q[1].emplace(GOAL, 0, '$', 0, manhattanDistance(src, GOAL));
  
  vector<T> candidates;
  bool done = false;
  
  while (!done) {
    done = true;
    
    for (int i : {0, 1}) {
      if (!q[i].empty()) {
        done = false;

        Node t = q[i].top();
        q[i].pop();
        
        if (action[i].count(t.state))
          continue;
        
        parent[i][t.state] = t.parent;
        action[i][t.state] = t.action;
        
        candidates.push_back(t.state);
        
        for (auto& a : actions(t.state)) {
          if (!action[i].count(a.first)) {
            q[i].emplace(a.first, t.state, a.second, t.cost + 1, manhattanDistance(a.first, goal[i]));
          }
        }
      }
    }

    for (T& x : candidates) {
      if (action[0].count(x) && action[1].count(x)) {
        string ans;
        
        T cur = x;
        while (action[0][cur] != '$') {
          ans.push_back(dir[(int)action[0][cur]]);
          cur = parent[0][cur];
        }

        reverse(ans.begin(), ans.end());

        cur = x;
        while (action[1][cur] != '$') {
          ans.push_back(dir[action[1][cur] ^ 1]);
          cur = parent[1][cur];
        }

        return ans;
      }
    }

    candidates.clear();

    if (done)
      break;
  }

  return "-1";
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int t;
  cin >> t;
  while (t--) {
    vector<int> puzzle(SIZE);
    for (auto &x : puzzle) {
      cin >> x;
    }
    if (solvable(puzzle)) {
      cout << solve(myHash(puzzle)) << '\n';
    } else {
      cout << "This puzzle is not solvable.\n";
    }
  }

  return 0;
}
