#pragma once
#include <vector>
#include <iterator>
#include <cmath>
#include "point.h"

using std::vector;
using std::string;
using std::ostream;
using std::cout;
using std::endl;


template <int Dim>

class KDTree
{
  private:
    /**
     * Internal structure for a node of KDTree.
     * Contains left, right children pointers and a K-dimensional point
     */
    struct KDTreeNode
    {
      Point<Dim> point;
      KDTreeNode *left, *right;

      KDTreeNode() : point(), left(NULL), right(NULL) {}
      KDTreeNode(const Point<Dim> &point) : point(point), left(NULL), right(NULL) {}
    };

  public:
    KDTree(const vector<Point<Dim>>& newPoints);

    /**
     * Copy constructor for KDTree.
     *
     * @param other The KDTree to copy.
     */
    KDTree(const KDTree<Dim>& other);

    /**
     * Assignment operator for KDTree.
     *
     * @param rhs The right hand side of the assignment statement.
     * @return A reference for performing chained assignments.
     */
    KDTree const &operator=(const KDTree<Dim>& rhs);

    /**
     * Destructor for KDTree.
     */
    ~KDTree();

    // function for 1-NN
    Point<Dim> findNearestNeighbor(const Point<Dim>& query) const;
    Point<Dim> findNearestNeighbor_h(const Point<Dim>& query, int dim, KDTree<Dim>::KDTreeNode* curRoot) const;
  

    // helper functions
    typename KDTree<Dim>::KDTreeNode* buildSubT(vector<Point<Dim>>& points, int left, int right, int cur_dim);
    void copy(const KDTree<Dim>& rhs);
    void clear(KDTreeNode*& root);
};




/**
  * Determines if a Point is closer to the target Point than another
  * reference Point. Takes three points: target, currentBest, and
  * potential, and returns whether or not potential is closer to
  * target than currentBest.
  *
  * We are using Euclidean distance to compare k-dimensional
  * points: \f$\sqrt{(p_1-q_1)^2+(p_2-q_2)^2+\ldots+(p_n-q_n)^2} =
  * \sqrt{\sum_{i=1}^n (p_i-q_i)^2}\f$. Note, however, that
  * **minimizing distance is the same as minimizing squared
  * distance**, so you can **avoid invoking the square root** and
  * just compare squared distances in your code.
  *
  * For example:
  *
  *     Point<3> target(1, 3, 5);
  *     Point<3> currentBest1(1, 3, 2);
  *     Point<3> possibleBest1(2, 4, 4);
  *     Point<3> currentBest2(1, 3, 6);
  *     Point<3> possibleBest2(2, 4, 4);
  *     Point<3> currentBest3(0, 2, 4);
  *     Point<3> possibleBest3(2, 4, 6);
  *     cout << shouldReplace(target, currentBest1, possibleBest1) << endl; // true
  *     cout << shouldReplace(target, currentBest2, possibleBest2) << endl; // false
  *     cout << shouldReplace(target, currentBest3, possibleBest3) << endl;
  *      // based on operator<, this should be false
  *
  * @todo This function is required for Part 1.
  * @param target The Point we want to be close to.
  * @param currentBest The Point that is currently our closest Point
  *    to target.
  * @param potential Our Point that is a candidate to replace
  *    currentBest (that is, it may be closer to target than
  *    currentBest).
  * @return A boolean value indicating whether potential is closer
  *  to target than currentBest. Ties should be broken with
  *  Point::operator<().
  */
template <int Dim>
bool shouldReplace(const Point<Dim>& target, const Point<Dim>& currentBest, const Point<Dim>& potential);


/**
  * Determines if Point a is smaller than Point b in a given dimension d.
  * If there is a tie, break it with Point::operator<().
  *
  * For example:
  *
  *     Point<3> a(1, 2, 3);
  *     Point<3> b(3, 2, 1);
  *     cout << smallerDimVal(a, b, 0) << endl; // should print true
  *     cout << smallerDimVal(a, b, 2) << endl; // should print false
  *     cout << smallerDimVal(a, b, 1) << endl; // based on operator<, this should be true
  *
  * @todo This function is required for Part 1.
  * @param first Point to compare.
  * @param second Second point to compare.
  * @param curDim Dimension these points are being compared in.
  * @return A boolean value indicating whether the first Point is smaller
  *  than the second Point in the curDim dimension.
  */
  
template <int Dim>
bool smallerDimVal(const Point<Dim>& first, const Point<Dim>& second,int curDim);


/**
  * This function uses the quickselect algorithm to partition the given
  * range such that the k-th element is in the k-th position and all
  * elements that compare as less by the provided function are to the
  * left and all larger elements are to the right. Note this does not
  * sort the range and runs in expected O(n) time.
  *
  * Reference (https://en.wikipedia.org/wiki/Quickselect)
  *
  * @param begin iterator to the start of the range inclusive
  * @param end  iterator to one past the end of the range
  * @param k iterator to the location in the range to find
  * @param cmp compare function true if arg1 is less than arg2
  */
  // template <typename RandIter, typename Comparator>
  // void select(RandIter begin, RandIter end, RandIter k, Comparator cmp);

template <typename RandIter, typename Comparator>
RandIter partition(RandIter left, RandIter right, RandIter pivotIndex, Comparator cmp);

template <typename RandIter, typename Comparator>
void select(RandIter begin, RandIter end, RandIter k, Comparator cmp);


#include "kdtree.hpp"