#include <iostream>
#include <fstream>
#include <vector>
#include <climits>

using std::vector;
using std::ifstream;
using std::max;
using std::cout;

vector<vector<int> > a;
vector<vector<int> > dp;
ifstream fin("date.in");
int n,m;

void Drum(int l, int c) {
    if(l==1) {
        printf("(%d,%d)", l, c);
        return;
    }
    int vmax = INT_MIN, vmaxpos;
    if(dp[l-1][c] > vmax) vmax = dp[l-1][c], vmaxpos = c;
    if(dp[l-1][c-1] > vmax) vmax = dp[l-1][c-1], vmaxpos = c-1;
    if(dp[l-1][c+1] > vmax) vmax = dp[l-1][c+1], vmaxpos = c+1;
    Drum(l-1,vmaxpos);
    printf("-> (%d,%d)", l, c);
}

int main() {
    fin >> n >> m;
    a.resize(n+2,vector<int>(m+2,0));
    dp.resize(n+2,vector<int>(m+2,0));
    for(int i = 1; i <= n; ++i) 
        for(int j = 1; j <= m; ++j)
            fin >> a[i][j];
    /// bordarea cu 0 din resize
    for(int j = 1; j <= m; ++j)
        dp[1][j] = a[1][j];

    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j) 
            dp[i][j] = a[i][j] + max(max(dp[i-1][j-1],dp[i-1][j+1]),dp[i-1][j]);
    
    int smax = INT_MIN, smaxpos = -1;
    for(int j = 1; j <= m; ++j)
        if(dp[n][j] > smax) smax = dp[n][j], smaxpos = j;
    
    cout << "Suma maxima este " << smax << '\n';
    cout << "Drumul (linie,coloana): ";
    Drum(n,smaxpos);
    cout << "\n";
    return 0;
}