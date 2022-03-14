#ifndef TREE_HPP
#define TREE_HPP

template <typename T>
Tree<T>::Tree(T root) : _value(root), _children(std::vector<Tree<T> *>()), _num_children(0) {}

template <typename T>
Tree<T>::~Tree()
{
   for (Tree<T> *child : _children)
   {
      if (child != nullptr)
         delete child;
   }
}

template <typename T>
Tree<T> *Tree<T>::add(T value)
{
   Tree *t = is_child(value);
   if (t)
      return t;
   t = new Tree<T>(value);
   _children.push_back(t);
   _num_children++;
   return t;
}

template <typename T>
Tree<T> *Tree<T>::is_child(T value)
{
   for (Tree<T> *child : _children)
   {
      if (child != nullptr && child->_value == value)
         return child;
   }
   return nullptr;
}

template <typename T>
void Tree<T>::leaves(std::vector<T> &v) const
{
   for (Tree<T> *child : _children)
   {
      if (child == nullptr)
         continue;
      if (child->_num_children == 0)
         v.push_back(child->_value);
      else
         child->leaves(v);
   }
}

template <typename T>
T Tree<T>::max_leaf() const
{
   std::vector<T> v;
   leaves(v);
   if (v.size() == 0)
      throw std::runtime_error("Non esistono foglie");
   T max = v[0];
   for (T leaf : v)
   {
      if (max < leaf)
         max = leaf;
   }
   return max;
}

template <typename T>
T Tree<T>::min_leaf() const
{
   std::vector<T> v;
   leaves(v);
   if (v.size() == 0)
      throw std::runtime_error("Non esistono foglie");
   T min = v[0];
   for (T leaf : v)
   {
      if (min > leaf)
         min = leaf;
   }
   return min;
}

#endif