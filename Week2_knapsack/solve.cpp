#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <string>
#include <random>
#include <chrono>

#define LL long long

using namespace std;

struct dwell
{
    int v;
    int w;
    int label;
    double ratio;

    dwell(int _v, int _w, int _label) : v(_v), w(_w), label(_label), ratio(1.0 * _v / _w) {}

    bool operator< (const dwell& that) const
    {
        return (LL)v * that.w > (LL)that.v * w;
    }
};

vector<dwell> goods;

int v[400000], w[400000], f[400000], pre[400000], label[400000];
bool select[400000], global_select[400000];
int n, k;

void knapsack_dp(const string& filename)
{
    freopen(filename.c_str(), "r", stdin);

    scanf("%d%d", &n, &k);
    for (int i = 0; i < n; ++i)
    {
        scanf("%d%d", &v[i], &w[i]);
    }

    int value = 0;
    while (true)
    {
        memset(f, 128, sizeof(f));
        memset(pre, 0, sizeof(pre));
        f[0] = 0;
        for (int i = 0; i < n; ++i)
        {
            if (select[i]) continue;
            for (int j = k; j >= w[i]; --j)
            {
                if (f[j - w[i]] + v[i] > f[j])
                {
                    f[j] = f[j - w[i]] + v[i];
                    pre[j] = i;
                }
            }
        }
        int where = 0;
        for (int i = k; i >= 1; --i)
        {
            if (f[i] > f[where]) where = i;
        }
        if (!where) break;

        select[pre[where]] = true;
        k -= w[pre[where]];
        value += v[pre[where]];
    }

    freopen((filename + ".ans").c_str(), "w", stdout);
    printf("%d 0\n", value);
    for (int i = 0; i < n; ++i)
    {
        if (select[i]) printf("1"); else printf("0");
        if (i != n - 1) printf(" ");
    }
    
    fclose(stdin);
    fclose(stdout);
}

void knapsack_sa(const string& filename, int para1, double para2, double para3, int para4)
{
    auto fin = fopen(filename.c_str(), "r");

    fscanf(fin, "%d%d", &n, &k);
    for (int i = 0; i < n; ++i)
    {
        fscanf(fin, "%d%d", &v[i], &w[i]);
    }
    fclose(fin);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> int_gen(0, n - 1);
    uniform_real_distribution<double> double_gen(0.0, 1.0);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);

    int global_value = 0;
    int sa_count = 0;

    while (true)
    {
        ++sa_count;
        if (sa_count % 1000 == 0) printf("sa_count = %d\n", sa_count);

        int weight = 0, value = 0, opt = 0, markov = 0, markov_max = 20;
        double T = para1, alpha = para3, eps = 1e-7;
        double random_select = para2;
        int findcnt = para4;
        memset(select, false, sizeof(select));

        std::default_random_engine e(seed);
        for (int i = 0; i < n; ++i) label[i] = i;
        shuffle(label, label + n, rng);

        for (int i = 0; i < n; ++i)
        {
            if (weight + w[label[i]] < k && double_gen(gen) > random_select)
            {
                weight += w[label[i]];
                value += v[label[i]];
                select[label[i]] = true;
                ++opt;
            }
        }
        
        while (T > eps)
        {
            if (value > global_value)
            {
                auto fout = fopen((filename + ".ans").c_str(), "w");
                fprintf(fout, "%d 0\n", value);
                for (int i = 0; i < n; ++i)
                {
                    if (select[i]) fprintf(fout, "1"); else fprintf(fout, "0");
                    if (i != n - 1) fprintf(fout, " ");
                }

                global_value = value;
                for (int i = 0; i < n; ++i)
                {
                    global_select[i] = select[i];
                }
                printf("cur value = %d %d\n", global_value, opt);
                fclose(fout);  
            }

            if (markov > markov_max)
            {
                break;
            }

            int i = int_gen(gen);
            if (!select[i])
            {
                if (weight + w[i] > k) continue;
                weight += w[i];
                value += v[i];
                select[i] = true;
                ++opt;
                markov = 0;
            }
            else
            {
                bool find = false;
                for (int x = 0; x < findcnt; ++x)
                {
                    int j = int_gen(gen);
                    if (select[j]) continue;
                    if (weight - w[i] + w[j] > k) continue;
                    if (v[j] > v[i])
                    {
                        weight += w[j] - w[i];
                        value += v[j] - v[i];
                        select[i] = false;
                        select[j] = true;
                        markov = 0;
                    }
                    else
                    {
                        int delta = v[j] - v[i];
                        if (double_gen(gen) < exp(1.0 * delta / T))
                        {
                            weight += w[j] - w[i];
                            value += v[j] - v[i];
                            select[i] = false;
                            select[j] = true;
                            markov = 0;
                        }
                    }
                    find = true;
                    break;
                }
                if (!find)
                {
                    ++markov;
                    continue;
                }
            }
            T *= alpha;
        }

        if (value > global_value)
        {
            auto fout = fopen((filename + ".ans").c_str(), "w");
            fprintf(fout, "%d 0\n", value);
            for (int i = 0; i < n; ++i)
            {
                if (select[i]) fprintf(fout, "1"); else fprintf(fout, "0");
                if (i != n - 1) fprintf(fout, " ");
            }

            global_value = value;
            for (int i = 0; i < n; ++i)
            {
                global_select[i] = select[i];
            }
            printf("cur value = %d %d\n", global_value, opt);
            fclose(fout);
        }
    }
}

void dfs(int id, int curv, int curw, int& global_value, const string& filename)
{
    if (id >= n) return;
    if (curv + (k - curw) * goods[id].ratio < global_value + 1e-7) return;

    if (curv > global_value)
    {
        auto fout = fopen((filename + ".ans").c_str(), "w");
        fprintf(fout, "%d 0\n", curv);
        for (int i = 0; i < n; ++i)
        {
            if (select[i]) fprintf(fout, "1"); else fprintf(fout, "0");
            if (i != n - 1) fprintf(fout, " ");
        }
        printf("cur value = %d\n", curv);
        fclose(fout);
        global_value = curv;
    }

    // select
    if (curw + goods[id].w < k)
    {
        select[goods[id].label] = true;
        dfs(id + 1, curv + goods[id].v, curw + goods[id].w, global_value, filename);
        select[goods[id].label] = false;
    }

    // not select
    dfs(id + 1, curv, curw, global_value, filename);
}

void knapsack_bb(const string& filename)
{
    auto fin = fopen(filename.c_str(), "r");
    fscanf(fin, "%d%d", &n, &k);
    for (int i = 0; i < n; ++i)
    {
        fscanf(fin, "%d%d", &v[i], &w[i]);
        goods.push_back(dwell(v[i], w[i], i));
    }
    fclose(fin);

    sort(goods.begin(), goods.end());

    int global_value = 0;
    dfs(0, 0, 0, global_value, filename);
}

int main(int argc, char** argv)
{
    if (!strcmp(argv[1], "./data/ks_30_0"))
    {
        knapsack_dp(argv[1]);
    }
    else if (!strcmp(argv[1], "./data/ks_50_0"))
    {
        knapsack_dp(argv[1]);
    }
    else if (!strcmp(argv[1], "./data/ks_200_0"))
    {
        knapsack_dp(argv[1]);
    }
    else if (!strcmp(argv[1], "./data/ks_400_0"))
    {
        knapsack_bb(argv[1]);
    }
    else if (!strcmp(argv[1], "./data/ks_1000_0"))
    {
        knapsack_dp(argv[1]);
    }
    else
    {
        knapsack_bb(argv[1]);
    }
    return 0;
}   