# Proiecte-PC2
Contents: One project per Programming Method ( D&I, Greedy, Backtracking, Dynamic Programming )

1. Divide et Impera - Check whether or not a given tree ( in Preorder, with NULL values at dead ends ) is a Binary Search Tree or not in linear time. \
  -> The tree is read from the file and parsed by a recursive function that dynamically allocates memory for each node. \
  -> The tree is recursively checked for BST requirements using lower and upper bound values. The desired behavior is that each path from the root to a leaf should narrow down (converge) to a value, so the lower bound is updated on each right step, while the upper bound is updated on each left step. \
  -> Memory is freed by TreeDestructor recursive function. 
2. Greedy - Given a list of numbers, sort them by simulating a Solitaire game. \
  -> Create a stack: 1) when no stack exists 2) When the current number is greater than all the stack tops existing. \
  -> If there is a stack where the number fits, pick the one with the smallest top. ( done using a minheap ) \
  -> After the heap was created, start removing the top value of the top-most stack in the heap. Remove the stack and reinsert it to assure correct heap ordering. 
3. BackTracking - basic RegEx interpretor. Supports: + ? * . \w \t \s \S \W {min,max} [chr1-chr2CHR3-CHR4] ; captures (and captures inside captures recursively).
^More of a state machine approach than backtracking^ \

4. Dynamic Programming - Find max sum of a path that starts anywhere on the first line of a matrix and ends on the last line \
  dp[i][j] = a[i][j] + max(dp[i-1][j-1],dp[i-1][j],dp[i-1][j+1])

Extras: \
  \> e^x approximation in x64 Assembly (NASM) \ 
  \> physics plotting and tables in Python \
  \> arduino : 
  morse code: text to led blinks \
  magnetic card door lock
