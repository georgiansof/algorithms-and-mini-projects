# Proiecte-PC2
Repository for Computer Programming 2 @ FMI Unibuc

Contents: One project per Programming Method ( D&I, Greedy, Backtracking, Dynamic Programming )

1. Divide et Impera - Check whether or not a given tree ( in Preorder, with NULL values at dead ends ) is a Binary Search Tree or not in linear time. \
  -> The tree is read from the file and parsed by a recursive function that dynamically allocates memory for each node. \
  -> The tree is recursively checked for BST requirements using lower and upper bound values. The desired behavior is that each path from the root to a leaf should narrow down (converge) to a value, so the lower bound is updated on each right step, while the upper bound is updated on each left step. \
  -> Memory is freed by TreeDestructor recursive function. \
2. Greedy - Given a list of numbers, sort them bwy simulating a Solitaire game. \
  -> Create a stack: 1) when no stack exists 2) When the current number is greater than all the stack tops existing. \
  -> If there is a stack where the number fits, pick the one with the smallest top. ( done using a minheap ) \
  -> After the heap was created, start removing the top value of the top-most stack in the heap. Remove the stack and reinsert it to assure correct heap ordering. \
