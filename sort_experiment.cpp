#include <bits/stdc++.h>
using namespace std;

struct Result {
    double seconds;
    unsigned long long operations;
};

static inline void counted_swap(vector<int>& a, int i, int j, unsigned long long &swaps) {
    if (i != j) {
        std::swap(a[i], a[j]);
        swaps++;
    }
}

Result insertion_sort_count(vector<int> a) {
    unsigned long long movements = 0;
    auto start = chrono::high_resolution_clock::now();
    int n = (int)a.size();
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            movements++;
            j--;
        }
        a[j + 1] = key;
        movements++;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    return {elapsed.count(), movements};
}

void merge_sort_rec(vector<int>& a, vector<int>& temp, int l, int r, unsigned long long &movements) {
    if (l >= r) return;
    int m = l + (r - l) / 2;
    merge_sort_rec(a, temp, l, m, movements);
    merge_sort_rec(a, temp, m + 1, r, movements);
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (a[i] <= a[j]) temp[k++] = a[i++];
        else temp[k++] = a[j++];
        movements++; // escrita no vetor auxiliar
    }
    while (i <= m) { temp[k++] = a[i++]; movements++; }
    while (j <= r) { temp[k++] = a[j++]; movements++; }
    for (int p = l; p <= r; p++) {
        a[p] = temp[p];
        movements++; // escrita de volta no vetor principal
    }
}

Result merge_sort_count(vector<int> a) {
    unsigned long long movements = 0;
    vector<int> temp(a.size());
    auto start = chrono::high_resolution_clock::now();
    if (!a.empty()) merge_sort_rec(a, temp, 0, (int)a.size() - 1, movements);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    return {elapsed.count(), movements};
}

int median_of_three_index(const vector<int>& a, int l, int m, int r) {
    int x = a[l], y = a[m], z = a[r];
    if ((x <= y && y <= z) || (z <= y && y <= x)) return m;
    if ((y <= x && x <= z) || (z <= x && x <= y)) return l;
    return r;
}

int partition_lomuto_median(vector<int>& a, int low, int high, unsigned long long &swaps) {
    int mid = low + (high - low) / 2;
    int pivotIndex = median_of_three_index(a, low, mid, high);
    counted_swap(a, pivotIndex, high, swaps);
    int pivot = a[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (a[j] <= pivot) {
            i++;
            counted_swap(a, i, j, swaps);
        }
    }
    counted_swap(a, i + 1, high, swaps);
    return i + 1;
}

Result quick_sort_count(vector<int> a) {
    unsigned long long swaps = 0;
    auto start = chrono::high_resolution_clock::now();
    if (a.size() > 1) {
        vector<pair<int,int>> st;
        st.reserve(64);
        st.push_back({0, (int)a.size() - 1});
        while (!st.empty()) {
            auto [low, high] = st.back();
            st.pop_back();
            while (low < high) {
                int p = partition_lomuto_median(a, low, high, swaps);
                // Processa a menor partição primeiro para reduzir uso de pilha.
                if (p - low < high - p) {
                    if (p + 1 < high) st.push_back({p + 1, high});
                    high = p - 1;
                } else {
                    if (low < p - 1) st.push_back({low, p - 1});
                    low = p + 1;
                }
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    return {elapsed.count(), swaps};
}

int main() {
    vector<int> sizes = {1000, 10000, 100000};
    mt19937 rng(2026);
    uniform_int_distribution<int> dist(0, 1000000000);

    cout << fixed << setprecision(6);
    cout << "algoritmo,tamanho,execucao,tempo_segundos,operacoes\n";

    for (int n : sizes) {
        vector<int> original(n);
        for (int i = 0; i < n; i++) original[i] = dist(rng);

        for (int rep = 1; rep <= 3; rep++) {
            Result r = insertion_sort_count(original);
            cout << "Insertion Sort," << n << "," << rep << "," << r.seconds << "," << r.operations << "\n";
        }
        for (int rep = 1; rep <= 3; rep++) {
            Result r = merge_sort_count(original);
            cout << "Merge Sort," << n << "," << rep << "," << r.seconds << "," << r.operations << "\n";
        }
        for (int rep = 1; rep <= 3; rep++) {
            Result r = quick_sort_count(original);
            cout << "Quick Sort," << n << "," << rep << "," << r.seconds << "," << r.operations << "\n";
        }
    }
    return 0;
}
