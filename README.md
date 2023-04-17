# Proiecte-PC2
Repository for Computer Programming 2 @ FMI Unibuc

Contents: One project per Programming Method ( D&I, Greedy, Backtracking, Dynamic Programming )

1. Divide et Impera - Check whether or not a given tree ( in Preorder, with NULL values at dead ends ) is a Binary Search Tree or not in linear time. \
  -> The tree is read from the file and parsed by a recursive function that dynamically allocates memory for each node. \
  -> The tree is recursively checked for BST requirements using lower and upper bound values. The desired behavior is that each path from the root to a leaf should narrow down (converge) to a value, so the lower bound is updated on each right step, while the upper bound is updated on each left step. \
  -> Memory is freed by TreeDestructor recursive function. \
