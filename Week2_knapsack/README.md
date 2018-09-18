| test_data | algorithm | time_spent | score |
| :------ | :------ | :------ | :------ |
| ks\_30_0 | DP | < 1s | 10 |
| ks\_50_0 | DP | < 1s | 10 |
| ks\_200_0 | DP | < 1s | 10 |
| ks\_400_0 | BB | < 1s | 10 |
| ks\_1000_0 | DP | < 3s | 10 |
| ks\_10000_0 | BB | < 1s | 10 |

Comment:  
DP = Dynamic Programming  
BB = Branch & Bound

Tips for Bound in BB:  
Sort the items by v/w (i.e. unit value) in decreasing order.  
The upper bound for item i, i+1, ..., n would be (vi/wi) * remaining_capacity.
