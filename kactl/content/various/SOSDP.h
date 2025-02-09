/**
 * Author: dimms
 * Date: 2024
 * License: CC0
 * Source: telegram
 * Description: SOS DP
 * Time: O(N * 2^N)
 * Status: works
 */

//memory optimized, super easy to code. 
for(int i = 0; i<(1<<N); ++i)  F[i] = A[i]; 
for(int i = 0;i < N; ++i) 
  for(int mask = 0; mask < (1<<N); ++mask) {
    if(mask & (1<<i))   F[mask] += F[mask^(1<<i)]; 
  }
