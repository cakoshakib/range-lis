#include "rlis.h"
#include <iostream>
#include <math.h>
using namespace std;

RLIS::RLIS(vector<int> _sequence, vector<query_t> _queries) {
    this->seq = _sequence;
    this->queries = _queries;
    this->n = _sequence.size();
}

vector<vector<int>> RLIS::run() {
    cout << "SEQ: ";
    for (int a_i : seq) {
        cout << a_i << " ";
    }
    cout << "\n---\n";
    query_map_t approximation = two_approx();
    double barrier = c * sqrt(n);

    // Sample Betas
    cout << "Computing Beta Samples\n";
    vector<double> betas;
    vector<map<int, vector<int>>> samples;
    for (int i = 0; i < log(n); i++) {
        double beta_i = barrier * pow(2, i);
        betas.push_back(beta_i);
        samples.push_back(beta_sample(beta_i));
    }

    // Get LIS for each query
    vector<vector<int>> results;
    for (query_t q : queries) {
        int approx = approximation[q];
        cout << "---\n";
        cout << "Computing LIS of (" << q.first << "," << q.second << ")\n"; 
        if (2*approx > barrier) {
            cout << "Long LIS!" << endl;
            int beta_index = lower_bound(betas.begin(), betas.end(), approx) - betas.begin();
            map<int, vector<int>> sample = samples[beta_index];
            results.push_back(long_query(q, sample));
        } else {
            cout << "Small LIS!" << endl;
        }
    }

    return results;
}