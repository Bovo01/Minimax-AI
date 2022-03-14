#ifndef TREE_H
#define TREE_H

#include <vector>

template <typename T>
class Tree {
private:
   T _value;
   std::vector<Tree<T> *> _children;
   int _num_children;

public:
   Tree(T root);
   ~Tree();

   Tree<T> *add(T value);
   Tree<T> *is_child(T value);
   void leaves(std::vector<T> &v) const;
   T max_leaf() const;
   T min_leaf() const;
};


#include "Tree.hpp"

#endif