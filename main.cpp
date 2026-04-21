#include <bits/stdc++.h>
using namespace std;

/*
Problem: Expect days to eat exactly m apples when starting with n apples.
Each day, with k apples remaining, choose uniformly x in [1..k] and eat x.
We want expected number of days until total eaten >= m (i.e., at least m apples have been eaten).

Interpretation: On a day with k remaining (k = n - eaten so far), the process ends that day if x >= k - (n - m) + 1? Better reframing:
Let T be days until cumulative eaten >= m. Equivalently, until remaining R <= n - m.
Define state by remaining apples r. Start r = n. Each day choose x ~ Uniform{1..r}, new r' = r - x.
We stop once r <= n - m. Let target threshold t = n - m. We need E[days] from r = n to hit r <= t.

Let E[r] = expected remaining days to reach r <= t given current remaining r (> t).
Base: for r <= t, E[r] = 0.
For r > t:
E[r] = 1 + (1/r) * sum_{x=1..r} E[max(r - x, t)] because if r - x <= t, we cap at t giving 0 future days. But more precisely, when r - x <= t, E[...] = 0.
So E[r] = 1 + (1/r) * sum_{x=1..r} E[max(r - x, t)].

We can compute DP from small r upwards. For r from 0..n, E[r] known for r <= t (0). For r = t+1..n:
E[r] = 1 + (1/r) * (sum_{x=1..r} E[max(r - x, t)]).
But max(r-x, t) equals r-x when x < r - t, else t for x >= r - t.
Number of x with r - x > t is when x < r - t => x = 1..(r - t - 1). For x >= r - t, E[...] = E[t] = 0.
Therefore sum simplifies: S = sum_{x=1}^{r - t - 1} E[r - x] = sum_{y=t+1}^{r-1} E[y].
Thus:
E[r] = 1 + (1/r) * sum_{y=t+1}^{r-1} E[y].

We can maintain prefix sums P[r] = sum_{y=0}^{r} E[y]. Then for r>t: sum_{y=t+1}^{r-1} E[y] = P[r-1] - P[t].
So recurrence:
E[r] = 1 + (P[r-1] - P[t]) / r.
Compute r from t+1..n, update P accordingly.

Finally answer is E[n]. Print to two decimals.
Numerical: rational or double is fine for n<=20; doubles ok.
*/

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n, m;
    if(!(cin >> n >> m)) return 0;
    if(m <= 0){
        cout << fixed << setprecision(2) << 0.00 << "\n";
        return 0;
    }
    if(m > n){
        // If need to eat more than available, you will finish in some expected days until r<=n-m is negative; threshold t = n-m < 0
        // Our recurrence still works if we clamp t>=0 effectively. But logically, once we eat all apples (r=0) we have eaten n<m, can't reach m.
        // However constraints guarantee m<=n, so just guard.
        cout << fixed << setprecision(2) << 0.00 << "\n";
        return 0;
    }
    long long t = n - m; // stop when remaining r <= t
    vector<long double> E(n+1, 0.0L);
    vector<long double> P(n+1, 0.0L);
    // E[r]=0 for r<=t, P initialized accordingly
    for(long long r = 0; r <= t && r <= n; ++r){
        E[r] = 0.0L;
        P[r] = (r==0?0.0L:P[r-1]) + E[r];
    }
    for(long long r = t + 1; r <= n; ++r){
        long double sumPrev = P[r-1] - (t>=0?P[t]:0.0L);
        E[r] = 1.0L + sumPrev / (long double)r;
        P[r] = P[r-1] + E[r];
    }
    long double ans = E[n];
    cout << fixed << setprecision(2) << (double)ans << "\n";
    return 0;
}

