/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>
#include <iostream>

#include "kdtree.h"

using namespace std;

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)  //check x
{
    /**
     * @todo Implement this function!
     */
    if (first[curDim] == second[curDim]) return first < second; 
    return first[curDim] < second[curDim];
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) //check x
{
    /**
     * @todo Implement this function!
     */
    int tc_dis = 0, tp_dis = 0;
    for (int i = 0; i < Dim; ++i) {
      tc_dis += std::pow((target[i] - currentBest[i]), 2);
      tp_dis += std::pow((target[i] - potential[i]), 2);
    }
    if (tc_dis == tp_dis) return potential < currentBest;
    return tp_dis < tc_dis;
}



//constructor
///////////////////////////////////////////////////////////////////////////////////////////
template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints) {
    /**
     * @todo Implement this function!
     */
    if (newPoints.empty()) { root = nullptr; return;}
    vector<Point<Dim>> vec;
    for (auto it = newPoints.begin(); it != newPoints.end(); it++) vec.push_back(*it);
    root = buildSubT(vec, 0, (int) vec.size() - 1, 0);
}


template<int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildSubT(vector<Point<Dim>>& points, int left, int right, int cur_dim) { //helper
  if (left > right) return nullptr;
  int mid = (left + right) / 2;
  select(points.begin() + left, points.begin() + right + 1, points.begin() + mid, [cur_dim](auto lhs, auto rhs) {return smallerDimVal(lhs, rhs, cur_dim); });
  KDTreeNode* node = new KDTreeNode(points[mid]);
  node->left = buildSubT(points, left,  mid - 1, (cur_dim+1) % Dim);
  node->right = buildSubT(points, mid + 1, right, (cur_dim+1) % Dim);
  return node;
}




template <typename RandIter, typename Comparator>
RandIter partition(RandIter left, RandIter right, RandIter pivotIndex, Comparator cmp) {
    swap(*pivotIndex, *(right - 1)); // swap pivot element to the end
    RandIter storedIndex = left;
    for (auto it = left; it != right - 1; ++it) { // loop up to right - 1
        if (cmp(*it, *(right - 1))) {
            swap(*it, *storedIndex);
            storedIndex++;
        }
    }
    swap(*(right - 1), *storedIndex); // swap pivot element back
    return storedIndex;
}

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp) { //helper may have some problem
    if (start >= end) return;
    auto pivotIndex = partition(start, end, k, cmp);
    if (k == pivotIndex) return;
    else if (k < pivotIndex) select(start, pivotIndex, k, cmp);
    else if (k > pivotIndex) select(pivotIndex + 1, end, k, cmp);
}

////////////////////////////////////////////////////////


template <int Dim>
void KDTree<Dim>::copy(const KDTree<Dim>& rhs) {
  if (rhs == NULL) return;
  auto node = new KDTreeNode(rhs->point);
  node->left = copy(rhs->left);
  node->right = copy(rhs->right);
}
template <int Dim>
void KDTree<Dim>::clear(KDTreeNode*& root) {
  if (root == NULL) return;
  clear(root->left);
  clear(root->right);
  delete root;
  root = nullptr;
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
  copy(other);
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  if (&rhs == this) return *this;
  clear(rhs);
  copy(rhs);
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  clear(root);
}





template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    
    return findNearestNeighbor_h(query, 0, root);
}



template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor_h(const Point<Dim>& query, int dim, KDTree<Dim>::KDTreeNode* curRoot) const {
  if (curRoot->left == NULL && curRoot->right == NULL) return curRoot->point;
  Point<Dim> currentBest = curRoot->point;
  Point<Dim> potential = curRoot->point;
  bool is_left = smallerDimVal(query, curRoot->point, dim);
  if (smallerDimVal(query, curRoot->point, dim)) {
    if (curRoot->left != NULL) currentBest = findNearestNeighbor_h(query, (dim+1)%Dim, curRoot->left);
    else currentBest = findNearestNeighbor_h(query, (dim+1)%Dim, curRoot->right);
  } else {
    if (curRoot->right != NULL) currentBest = findNearestNeighbor_h(query, (dim+1)%Dim, curRoot->right);
    else currentBest = findNearestNeighbor_h(query, (dim+1)%Dim, curRoot->left);
  }
  if (shouldReplace(query, currentBest, curRoot->point)) {
    currentBest = curRoot->point;
  }
  double splitDist = std::pow((curRoot->point[dim] - query[dim]), 2);
  double radius = 0.0;
  for (int i = 0; i < Dim; ++i) radius += std::pow((query[i] - currentBest[i]), 2);
  if (radius >= splitDist) {
    if (is_left && curRoot->right != NULL) potential = findNearestNeighbor_h(query, (dim+1)%Dim, curRoot->right);
    else if (curRoot->left != NULL) potential = findNearestNeighbor_h(query, (dim+1)%Dim, curRoot->left);
    if (shouldReplace(query, currentBest, potential)) currentBest = potential;
  } 
  return currentBest;
}
