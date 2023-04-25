#include "rlis.h"
#include <random>
#include <math.h>
#include <time.h>
using namespace std;

map<int, vector<int>> RLIS::beta_sample(double beta) {
    if (beta <= 0) {
        cout << "Invalid beta " << beta << "\n";
        exit(0);
    }
    int n = (this->seq).size();
    double p = (this->c * log(n)) / beta;
    //cout << "Sampling with probability " << p << endl; 

    map<int, vector<int>> sampled_elements;
    mt19937 gen(random_device{}());
    uniform_real_distribution<> dis(0, 1); //uniform distribution between 0 and 1

    for (int i = 0; i < n; i++) {
        if (dis(gen) > p) continue;
        // TODO: need to make this report the seq itself later
        vector<int> bcw = backward_lis(0, i);
        vector<int> frw = forward_lis(i, n-1);
        vector<int> sample_lis;
        sample_lis.reserve(bcw.size() + frw.size());
        sample_lis.insert(sample_lis.end(), bcw.begin(), bcw.end());
        sample_lis.insert(sample_lis.end(), frw.begin()+1, frw.end());
       
        sampled_elements[i] = sample_lis;
    }
    //cout << "Finished sampling " << beta << endl;
    return sampled_elements;
}

vector<int> RLIS::long_query(query_t query, map<int, vector<int>> sample) {
    cout << "Computing long query\n";
    int l = query.first, r = query.second;

    int lis = 0;
    for (int i = l; i <= r; i++) {
        // Check if this element in the range is a stitching element
        if (sample.find(i) != sample.end()) {
            lis = max(lis,  sample[i][l] + sample[i][r] - 1);
        }
    }

    cout << "Long query (" << l << "," << r << ") result: " << lis << endl;
    return {lis};
}