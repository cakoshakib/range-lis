#include "rlis.h"
#include <random>
#include <algorithm>
#include <math.h>
#include <time.h>
using namespace std;

map<int, vector<PatienceNode*>> RLIS::beta_sample(double beta) {
    if (beta <= 0) {
        cout << "Invalid beta " << beta << "\n";
        exit(0);
    }
    int n = (this->seq).size();
    double p = (this->c * log(n)) / beta;
    //cout << "Sampling with probability " << p << endl; 

    map<int, vector<PatienceNode*>> sampled_elements;
    mt19937 gen(random_device{}());
    uniform_real_distribution<> dis(0, 1); //uniform distribution between 0 and 1

    for (int i = 0; i < n; i++) {
        if (dis(gen) > p) continue;
        // TODO: need to make this report the seq itself later
        vector<PatienceNode*> bcw = backward_lis(0, i);
        vector<PatienceNode*> frw = forward_lis(i, n-1);
        cout << frw.size() + bcw.size() << endl;
        vector<PatienceNode*> sample_lis;
        sample_lis.reserve(bcw.size() + frw.size());
        sample_lis.insert(sample_lis.end(), bcw.begin(), bcw.end());
        sample_lis.insert(sample_lis.end(), frw.begin()+1, frw.end());
       
        sampled_elements[i] = sample_lis;
    }
    //cout << "Finished sampling " << beta << endl;
    return sampled_elements;
}

vector<int> RLIS::long_query(query_t query, map<int, vector<PatienceNode*>> sample) {
    cout << "Computing long query\n";
    int l = query.first, r = query.second;

    int longest = 0;
    PatienceNode *lower_lis = nullptr, *upper_lis = nullptr;
    for (int i = l; i <= r; i++) {
        // Check if this element in the range is a stitching element
        if (sample.find(i) != sample.end()) {
            if (sample[i][l]->ln + sample[i][r]->ln - 1 >= longest) {
                cout << "Stitching element " << seq[i] << endl;
                cout << sample[i].size() << endl;
                lower_lis = sample[i][l];
                upper_lis = sample[i][r];
                longest = lower_lis->ln + upper_lis->ln - 1;
            }
        }
    }

    vector<int> lis;
    int lower_ln = lower_lis->ln;
    while (lower_lis->next != nullptr) {
        lis.push_back(lower_lis->val);
        lower_lis = lower_lis->next;
    }
    while (upper_lis != nullptr) {
        lis.push_back(upper_lis->val);
        upper_lis = upper_lis->next;
    }
    reverse(lis.begin()+lower_ln, lis.end());

    cout << "Long query (" << l << "," << r << ") result: " << " ";
    for (int i = 0; i < lis.size(); i++) {
        cout << lis[i] << ",";
    }
    cout << endl;
    return lis;
}