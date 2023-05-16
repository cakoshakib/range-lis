#ifndef __UTILITY_H__
#define __UTILITY_H__
class PatienceNode {
public:
    int val, ln;
    int base, peak;
    PatienceNode *next;
    PatienceNode(int val) {
        this->val = val;
        ln = 1;
        next = nullptr;
    }
};
#endif