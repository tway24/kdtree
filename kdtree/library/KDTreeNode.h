//
// Created by tshah on 7/18/2016.
//

#ifndef BUILD_KDTREE_KDTREENODE_H
#define BUILD_KDTREE_KDTREENODE_H

#include <limits>
#include <unordered_set>
#include <sstream>
#include "KDUtils.h"
#include "Point.h"
#include "DimensionBondary.h"

template<typename T = float> class KDTreeNode{
private:
    //In datapoint which point to which point belongs to this node
    std::pair<int, int> pointIndexRange = {-1, -1};
    int numDimensions;
    //Set containing pointer to adjacent nodes
    //Adjacent nodes is the one who is sharing a boundary
    //If this node is non-leaf node, this set will be erased
    //after tree building is completed (saves space)
    std::unordered_set<KDTreeNode *> adjNodes;

public:
    //Vector with upper and lower bound for each dimension
    //If this is non-leaf node, this vector will be erased
    //after tree building is completed.
    DimensionBondaryVec<T> dimBnd;
    KDTreeNode *leftTree = NULL;
    KDTreeNode *rightTree = NULL;
    //Which dimension is used for dividing dataset
    int cutDimension;
    // median used for division
    T medianValue;
    KDTreeNode(){};
    KDTreeNode(int n): numDimensions(n){
        dimBnd = DimensionBondaryVec<T> (n);
    };
    void setPoints(std::vector<Point<T>> & pts, int start, int end){
        pointIndexRange.first = start;
        pointIndexRange.second = end;
    }
    bool isLeafNode(){
        return leftTree==NULL && rightTree==NULL;
    }
    //remove from memory
    void destroy();

    //Given a set of parent's adjacent node, check which nodes are also
    //adjacent to this node. If so, insert to current set
    void updateAdjNode(std::unordered_set<KDTreeNode<T> *> & parentAdjNodes);
    //Nearest neighbor method
    T getNearestPoint(Point<T> pt, Point<T> & closestPt, std::vector<Point<T>> & dataPoints);
    //save tree node to string
    void serialize(std::string &s);
    //generate tree node from string
    static KDTreeNode<T> * deserialize(std::istringstream &is, int numDim);

    std::unordered_set<KDTreeNode<T> *> & getAdjNodes(){
        return adjNodes;
    }
    void insertAdjNode(KDTreeNode * node){
        adjNodes.insert(node);
    }
    void removeAdjNode(KDTreeNode * node){
        adjNodes.erase(node);
    }
    void clearAdjNodes(){
        adjNodes.clear();
    }
};

template<typename T>
T KDTreeNode<T>::getNearestPoint(Point<T> pt, Point<T> & closestPt, std::vector<Point<T>> & dataPoints){
    T minDistance = std::numeric_limits<T>::max();
    //iterate over all points in current node and find closest point
    for(int i = pointIndexRange.first ; i < pointIndexRange.second ; ++i){
        T dist = pt.distance(dataPoints[i]);
        if(dist < minDistance){
            minDistance = dist;
            closestPt = dataPoints[i];
        }
    }
    return minDistance;
}

template<typename T>
void KDTreeNode<T>::serialize(std::string &s){
    serializeData(s, numDimensions);
    serializeData(s, pointIndexRange.first);
    serializeData(s, pointIndexRange.second);
    serializeData(s, cutDimension);
    serializeData(s, medianValue);
    if(isLeafNode()) {
        dimBnd.serialize(s);
    }else{
        // for non-leaf node DimensionBoundary will not be present
        serializeData(s, "NL");
    }
}

template<typename T>
KDTreeNode<T> *KDTreeNode<T>::deserialize(std::istringstream &is, int numDim) {
    KDTreeNode<T> *node = NULL;
    if(!is.eof()){
        std::string s = deserialString(is);
        if(s.compare("NULL")) {
            //non-leaf node
            node = new KDTreeNode<T>;
            node->numDimensions = numDim;
            node->pointIndexRange.first = deserialData<T>(is);
            node->pointIndexRange.second = deserialData<T>(is);
            node->cutDimension = deserialData<T>(is);
            node->medianValue = deserialData<T>(is);
            node->dimBnd = DimensionBondaryVec<T>::deserialize(is, numDim);
        }
    }
    return node;
}

//Given a set of parent's adjacent node, check which nodes are also
//adjacent to this node. If so, insert to current set
template<typename T>
void KDTreeNode<T>::updateAdjNode(std::unordered_set<KDTreeNode<T> *> & parentAdjNodes){
    for(auto & node : parentAdjNodes){
        if(dimBnd.isAdjacent(node->dimBnd))
            insertAdjNode(node);
    }
}

template<typename T>
void KDTreeNode<T>::destroy(){
    if(leftTree != NULL) leftTree->destroy();
    if(rightTree != NULL) rightTree->destroy();
    dimBnd.destroy();
}

#endif //BUILD_KDTREE_KDTREENODE_H
