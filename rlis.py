from typing import List, Tuple
from bisect import bisect_right, bisect_left
import random
import math

INF = 1e9

class Node:
    def __init__(self, start, end):
        self.start, self.end = start, end
        self.mid = self.mid_point(start, end)
        self.memo = {}
        self.left, self.right = None, None

    def mid_point(self, x, y):
        return x + ((y - x) // 2)

    def __str__(self):
        return str(f'START: {self.start} MID: {self.mid} END: {self.end}')

class LIS():
    def __init__(self, seq):
        self.seq = seq

    def standard_lis(self, seq: List[int]):
        return self.forward_lis(seq)[-1]

    def forward_lis(self, seq):
        n = len(seq)
        d = [INF]*(n+1)
        d[0] = -INF
        ans = 0
        res = [INF]*n
        for i in range(n):
            l = bisect_right(d, seq[i])
            if d[l-1] < seq[i] and seq[i] < d[l]:
                d[l] = seq[i]
                ans = max(ans, l)
                res[i] = ans
        return res

    def reversed_lis(self, seq):
        n = len(seq)
        d = [-INF]*(n+1)
        d[0] = INF
        ans, res = 0, [INF]*n
        for i in range(n-1, -1, -1):
            l = self.rev_bisect(d, seq[i])
            if d[l-1] > seq[i] and seq[i] > d[l]:
                d[l] = seq[i]
                ans = max(ans, l)
                res[i] = ans
        return res

    def rev_bisect(self, a, x):
        lo, hi = 0, len(a)
        while lo < hi:
            mid = (lo+hi)//2
            if x > a[mid]: hi = mid
            else: lo = mid+1
        return lo

class RLIS(LIS):
    c = 2

    def __init__(self, seq: List[int], queries: List[Tuple[int, int]]):
        self.seq = seq
        self.n = len(seq)
        self.queries = queries
        self.BARRIER = self.c * int(self.n ** (1/2))

    def two_approx(self):
        seq, n = self.seq, self.n

        # Build a Binary Search Tree
        root = Node(0, n-1)
        visited = set()
        def rec(node):
            start, mid, end = node.start, node.mid, node.end
            if mid in visited:
                return
            visited.add(mid)

            # Calculate LIS([i, n/2])
            rev = self.reversed_lis(seq[start:mid+1])
            for i in range(start, mid+1):
                node.memo[i] = rev[i-start]

            # LIS([n/2+1, j])
            frw = self.forward_lis(seq[mid+1:end+1])
            for j in range(mid+1, end+1):
                node.memo[j] = frw[j-mid-1]
            
            # recurse
            node.left = Node(start, mid)
            node.right = Node(mid+1, end)
            rec(node.left), rec(node.right)
        rec(root)

        # Approximate queries
        def answer_query(node, i, j):
            if i <= node.mid and node.mid < j:
                return max(node.memo[i], node.memo[j])
            elif j <= node.mid:
                return answer_query(node.left, i, j)
            else:
                return answer_query(node.right, i, j)
        approx = {}
        for q in self.queries:
            i, j = q[0], q[1]
            approx[q] = answer_query(root, i, j)

        print("APPROXIMATION:", approx)
        return approx

    def beta_sample(self, beta: int):
        # solve query with a long LIS
        seq, n = self.seq, self.n

        # Sample
        c = self.c
        p = c * math.log(n) / beta
        R = random.choices(seq, weights=[p]*n)

        memo = {}
        for i, a in enumerate(seq):
            memo[i] = {}
            rev = self.reversed_lis(seq[0:i+1])
            for l in range(i+1):
                memo[i][l] = rev[l]
            frw = self.forward_lis(seq[i:n])
            for r in range(i,n):
                memo[i][r] = frw[r-i]
        return memo

    def long_q(self, q: Tuple[int,int], ln: int, beta_samples):
        # solve query with a long LIS seq, n = self.seq, self.n
        seq = self.seq
        l, r = q[0], q[1]

        # Find which beta range this interval falls under
        betas = list(beta_samples.keys())
        beta = betas[bisect_left(betas, ln) - 1]
    
        # Find stitching elements
        R = set(beta_samples[beta].keys())
        longest = 0
        for j in range(l, r+1):
            if seq[j] in R:
                longest = max(longest, beta_samples[beta][seq[j]][l] + beta_samples[beta][seq[j]][r] - 1)

        return longest

    def preprocess_dp(self):
        # Build a Binary Search Tree
        n = self.n
        root = Node(0, n-1)
        visited = set()
        def r_range_max(matrix, lb, r, beta):
            cur_max = []
            a_r = self.seq[r]
            for i in range(lb, r):
                a_i = self.seq[i]
                if a_i < a_r and len(cur_max) < len(matrix[i][beta-1]):
                    cur_max = matrix[i][beta-1] + [a_r]
            return cur_max

        def l_range_max(matrix, l, ub, alpha):
            cur_max = []
            a_l = self.seq[l]
            for i in range(l+1, ub+1):
                a_i = self.seq[i]
                if a_l < a_i and len(cur_max) < len(matrix[i][alpha-1]):
                    cur_max = [a_l] + matrix[i][alpha-1]
            return cur_max

        def rec(node):
            start, mid, end = node.start, node.mid, node.end
            if mid in visited:
                return
            visited.add(mid)

            # Calculate B[r][beta] for all n/2+1, r
            B = {}
            for r in range(mid+1, end+1):
                a_r = self.seq[r]https://rutgers.zoom.us/my/ks1744?pwd=YmVrd211OXE0RnVreUloWmM1Qm5EQT09
                B[r] = [[] for _ in range(self.BARRIER)]
                node.memo[r] = [[] for _ in range(self.BARRIER)]
                for beta in range(self.BARRIER):
                    if beta == 1:
                        B[r][beta] = [a_r]
                    else:
                        B[r][beta] = r_range_max(B, mid+1, r, beta)
                    # Use B to calculate R
                    node.memo[r][beta] = B[r][beta]
                    if r-1 in node.memo and len(node.memo[r-1][beta]) > len(B[r][beta]):
                        node.memo[r][beta] = node.memo[r-1][beta]

            # Calculate C[l][alpha] for all 1, n/2
            C = {}
            for l in range(mid, start-1, -1):
                a_l = self.seq[l]
                C[l] = [[] for _ in range(self.BARRIER)]
                node.memo[l] = [[] for _ in range(self.BARRIER)]
                for alpha in range(self.BARRIER):
                    if alpha == 1:
                        C[l][alpha] = [a_l]
                    else:
                        C[l][alpha] = l_range_max(C, l, mid, alpha)
                    # Use C to calculate L
                    node.memo[l][alpha] = C[l][alpha]
                    if l-1 in node.memo and len(node.memo[l-1][alpha]) > len(C[l][alpha]):
                        node.memo[l][alpha] = node.memo[l-1][alpha]

            # recurse
            node.left = Node(start, mid)
            node.right = Node(mid+1, end)
            rec(node.left), rec(node.right)
        rec(root)
        return root

    def small_q(self, root: "Node", q: Tuple[int,int]):
        # Approximate queries
        def compatible_beta(R, peak, alpha, node):
            best_beta = []
            # binary search
            lo, hi = 1, min(self.BARRIER, node.end - node.mid + 1)
            while lo < hi:
                beta = (lo + hi)//2
                # compatible
                if peak < R[beta][0]:
                    if len(R[beta]) > len(best_beta):
                        best_beta = R[beta]
                    lo = beta+1
                else:
                    hi = beta
            return best_beta

        def answer_query(node, i, j):
            if i <= node.mid and node.mid < j:
                L, R = node.memo[i], node.memo[j]
                max_seq = []
                # nlog(n) answer of query
                for alpha, l in enumerate(L):
                    if len(l) == 0: continue
                    peak = l[-1]
                    # Find corresponding beta to maximize alpha + beta
                    maxima_seq = L[alpha] + compatible_beta(R, peak, alpha, node)
                    if len(maxima_seq) > len(max_seq): max_seq = maxima_seq
                return max_seq
            elif j <= node.mid:
                return answer_query(node.left, i, j)
            else:
                return answer_query(node.right, i, j)
        return answer_query(root, q[0], q[1])

    def solve(self):
        n = self.n
        approx = self.two_approx()
        tree = self.preprocess_dp()

        beta_samples = {}
        for i in range(int(math.log(n))):
            beta_i = self.BARRIER * 2**i
            beta_samples[beta_i] = self.beta_sample(beta_i)

        res = []
        for q in self.queries:
            if 2*approx[q] > self.BARRIER:
                res.append(self.long_q(q, approx[q], beta_samples))
            else:
                res.append(self.small_q(tree, q))
        return res

res = RLIS([1,2,3,4,5,6], [(0,5), (0,2)]).solve()
print(res)
