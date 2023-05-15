#ifndef __UTILITY_H__
#define __UTILITY_H__
class PatienceNode {
public:
    int val, ln;
    PatienceNode *next;
    PatienceNode(int val) {
        this->val = val;
        ln = 0;
        next = nullptr;
    }
};
#endif