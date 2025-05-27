/**
 * Author: chilli, SJTU, pajenegod
 * Date: 2020-03-04
 * License: CC0
 * Source: own
 * Description: Pollard-rho randomized factorization algorithm. Returns prime
 * factors of a number, in arbitrary order (e.g. 2299 -> \{11, 19, 11\}).
 * Time: $O(n^{1/4})$, less for numbers with small factors.
 * Status: stress-tested
 *
 * Details: This implementation uses the improvement described here
 * (https://en.wikipedia.org/wiki/Pollard%27s_rho_algorithm#Variants), where
 * one can accumulate gcd calls by some factor (40 chosen here through
 * exhaustive testing). This improves performance by approximately 6-10x
 * depending on the inputs and speed of gcd. Benchmark found here:
 * (https://ideone.com/nGGD9T)
 *
 * GCD can be improved by a factor of 1.75x using Binary GCD
 * (https://lemire.me/blog/2013/12/26/fastest-way-to-compute-the-greatest-common-divisor/).
 * However, with the gcd accumulation the bottleneck moves from the gcd calls
 * to the modmul. As GCD only constitutes ~12% of runtime, speeding it up
 * doesn't matter so much.
 *
 * This code can probably be sped up by using a faster mod mul - potentially
 * montgomery reduction on 128 bit integers.
 * Alternatively, one can use a quadratic sieve for an asymptotic improvement,
 * which starts being faster in practice around 1e13.
 *
 * Brent's cycle finding algorithm was tested, but doesn't reduce modmul calls
 * significantly.
 *
 * Subtle implementation notes:
 * - we operate on residues in [1, n]; modmul can be proven to work for those
 * - prd starts off as 2 to handle the case n = 4; it's harmless for other n
 *   since we're guaranteed that n > 2. (Pollard rho has problems with prime
 *   powers in general, but all larger ones happen to work.)
 * - t starts off as 30 to make the first gcd check come earlier, as an
 *   optimization for small numbers.
 */
#pragma once

ul modMul(ul a, ul b, const ul mod) {
    ll ret = a*b-mod*(ul)((db)a*b/mod);
    return ret+((ret<0)-(ret>=(ll)mod))*mod; }
ul modPow(ul a, ul b, const ul mod) {
    if (b == 0) return 1;
    ul res = modPow(a,b/2,mod); res = modMul(res,res,mod);
    return b&1 ? modMul(res,a,mod) : res;
}
bool prime(ul n) { // not ll!
    if (n < 2 || n % 6 % 4 != 1) return n-2 < 2;
    ul A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022}, s = __builtin_ctzll(n-1), d = n>>s;
    for (auto &a : A) {   // ^ count trailing zeroes
        ul p = modPow(a,d,n), i = s;
        while (p != 1 && p != n-1 && a%n && i--) p = modMul(p,p,n);
        if (p != n-1 && i != s) return 0;
    }
    return 1;
}
ul pollard(ul n) { // return some nontrivial factor of n
    auto f = [n](ul x) { return modMul(x, x, n) + 1; };
    ul x = 0, y = 0, t = 30, prd = 2, i = 1, q;
    while (t++ % 40 || __gcd(prd, n) == 1) { /// speedup: don't take gcd every it
        if (x == y) x = ++i, y = f(x);
        if ((q = modMul(prd, max(x,y)-min(x,y), n))) prd = q;
        x = f(x), y = f(f(y));
    }
    return __gcd(prd, n);
}
void factor_rec(ul n, map<ul,int>& cnt) {
    if (n == 1) return;
    if (prime(n)) { ++cnt[n]; return; }
    ul u = pollard(n);
    factor_rec(u,cnt), factor_rec(n/u,cnt);
}
