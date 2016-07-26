//
// Created by tshah on 7/18/2016.
//

#ifndef BUILD_KDTREE_KDTREE_H
#define BUILD_KDTREE_KDTREE_H

#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include "KDUtils.h"
#include "KDTreeNode.h"
#include "KDTreeExtended.h"
#include "Point.h"

template<typename T = float > class KDTree{
private:
    //root node
    KDTreeNode<T> *root = NULL;
    //number of dimensions
    int numDimensions;
    //number of points in input data
    int numPoints;
    //max. number of points per leaf node
    int nodeSize;
    //level of adjacency to explore
    // 0 : only current node containing query point is explored
    // 1 : current node + immidiate neighbors will be explored
    // 2 : neigbors of immidiate neighbors will also be explored
    // and so on.
    int adjacentLevels = 0;
    // vector of input data points
    std::vector<Point<T>> dataPoints;
    //------------------------------------------------------------------------------
    //private methods:
    //------------------------------------------------------------------------------

    // Generate child node and set the median value to limit min or max
    KDTreeNode<T> * generateChildNode(DimensionBondaryVec<T> &parentDim, T medianVal, bool isLeft, int cutDim);
    // Traverse all leaf nodes and check its adjacent nodes
    // If adjacent node is non-leaf node traverse that node to find leaf nodes which are adjacent
    void traverseUpdateAdjNodes(KDTreeNode<T> * &curNode);
    // If adjacent node is non-leaf node traverse that node to find leaf nodes which are adjacent
    void traverseGetAdjNodes(KDTreeNode<T> * const &curNode, KDTreeNode<T> * &origNode);

    //BST traversal to find node enclsing query point
    KDTreeNode<T> * getEnclosingNode(Point<T> pt, KDTreeNode<T> * curNode);
    //searialize KDTree and return string to be stored to disk
    std::string serialize();
    void serializeRec(KDTreeNode<T> * curNode, std::string &s);
    //deserialize string to build KDTree
    static KDTree<T> * deserialize( std::istringstream &is);
    static KDTreeNode<T> * deserializeRec(std::istringstream &is, int numDim);
    //get list of leaf nodes
    void getAllLeafNodes(KDTreeNode<T> *node, std::unordered_set<KDTreeNode<T> *> & nodeSet);
    // update leaf nodes with their adjacent nodes
    void updateAllLeafNodes(std::unordered_set<KDTreeNode<T> *> & nodeSet);
    // destroy the object
    void destroy();
public:
    KDTree(){};
    KDTree(std::vector<std::vector<T>> & kdMatrix, int i_nodeSize, int i_adjLevel, int (*dimensionSelectorFn)(std::vector<Point<T>> &, int, int, int)=getCutDimension);
    void build(std::vector<std::vector<T>> & kdMatrix, int i_nodeSize, int i_adjLevels, int (*dimensionSelectorFn)(std::vector<Point<T>> &, int, int, int)=getCutDimension);
    void buildRecursive(int start, int end, KDTreeNode<T> * & curNode);
    ~KDTree(){
        destroy();
    }

    std::pair<int, T> getNearestNeighbor(Point<T> pt);
    //Save KDTree to disk
    void saveToDisk(std::ofstream & ofs);
    //Load KDTree from disk
    static KDTree<T> * loadFromDisk(std::ifstream & ifs);
private:
    //bonus point
    int (*dimensionSelFn)(std::vector<Point<T>> &, int, int, int);
};

template<typename T>
KDTree<T>::KDTree(std::vector<std::vector<T>> & kdMatrix, int i_nodeSize, int i_adjLevels, int (*dimensionSelectorFn)(std::vector<Point<T>> &, int, int, int)) {
    build(kdMatrix, i_nodeSize, i_adjLevels, dimensionSelectorFn);
}
template<typename T>
void KDTree<T>::build(std::vector<std::vector<T>> & kdMatrix, int i_nodeSize, int i_adjLevels, int (*dimensionSelectorFn)(std::vector<Point<T>> &, int, int, int)){
    nodeSize = i_nodeSize;
    adjacentLevels = i_adjLevels;
    dimensionSelFn = dimensionSelectorFn;
    //check if matrix is empty
    if(kdMatrix.empty()){
        // TODO: raise exception
    }else {
        numPoints = kdMatrix.size();
        numDimensions = kdMatrix[0].size();
        //Check if all data points has equal dimensionality
        for(auto & data : kdMatrix){
            if(data.size() != numDimensions){
                //TODO: raise exception
            }
        }
        //create all data points
        for(int i = 0 ; i < numPoints ; ++i){
            Point<T> pt(numDimensions, kdMatrix[i], i);
            dataPoints.push_back(pt);
        }
        //recursively build tree
        root = new KDTreeNode<T>(numDimensions);
        buildRecursive(0, numPoints, root);
        //Once tree is built traverse the tree for each leaf-node
        //Check adjacent-nodes and if adj-node is non-leaf node find
        //exact adj-node
        traverseUpdateAdjNodes(root);
    }
}

//traverse the tree for each leaf-node
//Check adjacent-nodes and if adj-node is non-leaf node find
//exact adj-node
template<typename T>
void KDTree<T>::traverseUpdateAdjNodes(KDTreeNode<T> * &curNode){
    if(curNode->isLeafNode()){
        std::unordered_set<KDTreeNode<T> *> nonLeafToRemove;
        for(auto node : curNode->getAdjNodes()){
            if(!(node->isLeafNode())) {
                nonLeafToRemove.insert(node);
                traverseGetAdjNodes(node, curNode);
            }
        }
        for(auto node : nonLeafToRemove)
            curNode->removeAdjNode(node);
    }else{
        traverseUpdateAdjNodes(curNode->leftTree);
        traverseUpdateAdjNodes(curNode->rightTree);
        //destroy non-leaf node's adjacent nodes set
        curNode->clearAdjNodes();
        //destroy non-leaf node's boundary-dimension vector
        curNode->dimBnd.destroy();
    }
}

//Find all leaf-nodes in current node hierarch
//check if this leaf-node is adjacenct to original node
//original node is the one for which traversal was started
template<typename T>
void KDTree<T>::traverseGetAdjNodes(KDTreeNode<T> * const &curNode, KDTreeNode<T> * &origNode) {
    if(curNode->isLeafNode()){
        if(curNode->dimBnd.isAdjacent(origNode->dimBnd)){
            origNode->insertAdjNode(curNode);
        }
    }else{
        traverseGetAdjNodes(curNode->leftTree, origNode);
        traverseGetAdjNodes(curNode->rightTree, origNode);
    }
}

template<typename T>
KDTreeNode<T> * KDTree<T>::generateChildNode(DimensionBondaryVec<T> &parentDim, T medianVal, bool isLeft, int cutDim){
    KDTreeNode<T> * node = new KDTreeNode<T>(numDimensions);
    node->dimBnd = parentDim;
    if(isLeft){
        node->dimBnd[cutDim].dimMax = medianVal;
    }else{
        node->dimBnd[cutDim].dimMin = medianVal;
    }
    return node;
}

template<typename T>
void KDTree<T>::buildRecursive(int start, int end, KDTreeNode<T> * &curNode){
    if(end-start <= nodeSize){
        //No further division
        curNode->setPoints(dataPoints, start, end);
        curNode->leftTree = NULL;
        curNode->rightTree = NULL;
        //Print dimensions
        //curNode->printBoundaries();
        return;
    }
    //divide dataset
    //int cutDim = getCutDimension(dataPoints, start, end, numDimensions);
    int cutDim = dimensionSelFn(dataPoints, start, end, numDimensions);
    curNode->cutDimension = cutDim;
    //sort slice of data-points between start and end
    //based on selected dimension
    sort(dataPoints.begin()+start, dataPoints.begin()+end, [&cutDim](Point<T> & a, Point<T> & b){ return a[cutDim] < b[cutDim]; });
    int medianIdx = start + (end-start)/2;
    curNode->medianValue = dataPoints[medianIdx][cutDim];
    //Build left tree
    curNode->leftTree = generateChildNode(curNode->dimBnd, curNode->medianValue, true, cutDim);
    curNode->leftTree->updateAdjNode(curNode->getAdjNodes());
    //Build right tree
    curNode->rightTree = generateChildNode(curNode->dimBnd, curNode->medianValue, false, cutDim);
    curNode->rightTree->updateAdjNode(curNode->getAdjNodes());
    //Add left and right to each other
    curNode->leftTree->insertAdjNode(curNode->rightTree);
    curNode->rightTree->insertAdjNode(curNode->leftTree);
    //recursively build trees
    buildRecursive(start, medianIdx+1, curNode->leftTree);
    buildRecursive(medianIdx+1, end, curNode->rightTree);
    return;
}

template<typename T>
void KDTree<T>::destroy(){
    //recursively destroy all nodes starting with root
    if(root != NULL) {
        root->destroy();
        //destroy all points
        for (int i = 0; i < numPoints; ++i) {
            //delete individual kdimensional-points
            dataPoints[i].destroy();
        }
        // destroy point array
        std::vector<Point<T>>().swap(dataPoints);
    }
}

template<typename T>
std::pair<int, T> KDTree<T>::getNearestNeighbor(Point<T> pt) {
    //Find node which contains this point
    KDTreeNode<T> * enclNode = getEnclosingNode(pt, root);
    std::unordered_set<KDTreeNode<T> *> nodeSet;
    // add enclosing node to current set as a center node
    nodeSet.insert(enclNode);
    // dependening upon lavels of neighbors we need,
    // keep on adding those neighbours to set
    for(int i = 1 ; i <= adjacentLevels ; ++i){
        std::unordered_set<KDTreeNode<T> *> adjOfadj;
        for(auto node : nodeSet){
            std::unordered_set<KDTreeNode<T> *> adjOfneigh = node->getAdjNodes();
            copy(adjOfneigh.begin(), adjOfneigh.end(), inserter(adjOfadj, adjOfadj.end()));
        }
        copy(adjOfadj.begin(), adjOfadj.end(), inserter(nodeSet, nodeSet.end()));
    }

    T minDistance = std::numeric_limits<T>::max();
    Point<T> nearPoint, nearestPoint;
    // find closest point among all nodes (enclosing node + its adjacent nodes)
    for(auto & node : nodeSet){
        T dist = node->getNearestPoint(pt, nearPoint, dataPoints);
        if(dist < minDistance){
            minDistance = dist;
            nearestPoint = nearPoint;
        }
    }

    float distance = pt.distance(nearestPoint);
    std::pair<int, T> result = {nearestPoint.getIndex(), distance};
    return result;
}



/*
 //Brute-force approach for debugging
 // Check every leaft-node
template<typename T>
pair<int, T> KDTree<T>::getNearestNeighbor(Point<T> pt) {
    //Find node which contains this point
    KDTreeNode<T> * enclNode = getEnclosingNode(pt, root);
    KDTreeNode<T> * goldenNode = NULL;
    unordered_set<KDTreeNode<T> *> nodeSet;
    getAllLeafNodes(root, nodeSet);
    // add enclosing node to current set
    //nodeSet.insert(enclNode);
    T minDistance = numeric_limits<T>::max();
    Point<T> nearPoint, nearestPoint;
    // find closest point among all nodes (enclosing node + its adjacent nodes)
    for(auto & node : nodeSet){
        T dist = node->getNearestPoint(pt, nearPoint, dataPoints);
        if(dist < minDistance){
            minDistance = dist;
            nearestPoint = nearPoint;
            goldenNode = node;
        }
    }

    float distance = pt.distance(nearestPoint);
    pair<int, T> result = {nearestPoint.getIndex(), distance};
    return result;
}
*/

template<typename T>
KDTreeNode<T> * KDTree<T>::getEnclosingNode(Point<T> pt, KDTreeNode<T> * curNode){
    if(curNode->isLeafNode()){
        return curNode;
    } else{
        if(pt[curNode->cutDimension] <= curNode->medianValue)
            return getEnclosingNode(pt, curNode->leftTree);
        else
            return getEnclosingNode(pt, curNode->rightTree);
    }
}

//Serialize tree
template<typename T>
std::string KDTree<T>::serialize(){
    std::string serialStr;
    //serialize metadata
    serializeData(serialStr, numDimensions);
    serializeData(serialStr, numPoints);
    serializeData(serialStr, nodeSize);
    serializeData(serialStr, adjacentLevels);
    //serialStr.append("\n");
    for(int i = 0 ; i < numPoints; ++i){
        dataPoints[i].serialize(serialStr);
    }
    //serialize current datapoints
    //recursively serialize tree
    serializeRec(root, serialStr);
    return serialStr;
}

template<typename T>
void KDTree<T>::serializeRec(KDTreeNode<T> * curNode, std::string &s){
    if(curNode == NULL) {
        serializeData(s, "NULL");
        //s.append("\n");
    }
    else{
        curNode->serialize(s);
        serializeRec(curNode->leftTree, s);
        serializeRec(curNode->rightTree, s);
    }
}

//Save KDTree to disk
template<typename T>
void KDTree<T>::saveToDisk(std::ofstream & ofs){
    std::string serializedTree = serialize();
    ofs << serializedTree ;
}

template<typename T>
KDTree<T> * KDTree<T>::loadFromDisk(std::ifstream &ifs) {
    std::string s;
    getline(ifs, s);
    std::istringstream is(s);
    return deserialize(is);
}

template<typename T>
KDTree<T> * KDTree<T>::deserialize(std::istringstream & is) {
    KDTree<T> *kdtptr = new KDTree<T> ;
    //TODO: check for validity of is object
    kdtptr->numDimensions = deserialData<int>(is);
    kdtptr->numPoints = deserialData<int>(is);
    kdtptr->nodeSize = deserialData<int>(is);
    kdtptr->adjacentLevels = deserialData<int>(is);
    for(int i = 0 ; i < kdtptr->numPoints ; ++i){
        kdtptr->dataPoints.push_back(Point<T>::deserialize(is, kdtptr->numDimensions));
    }
    kdtptr->root = deserializeRec(is, kdtptr->numDimensions);
    //get all leaf nodes
    std::unordered_set<KDTreeNode<T> *> nodeSet;
    kdtptr->getAllLeafNodes(kdtptr->root, nodeSet);
    //update each leaf nodes' adjacent nodes
    kdtptr->updateAllLeafNodes(nodeSet);
    return kdtptr;
}

template<typename T>
KDTreeNode<T> *KDTree<T>::deserializeRec(std::istringstream &is, int numDim) {
    KDTreeNode<T> * node = KDTreeNode<T>::deserialize(is, numDim);
    if(node != NULL) {
        node->leftTree = deserializeRec(is, numDim);
        node->rightTree = deserializeRec(is, numDim);
    }
    return node;
}

//Following method used when KDTree is loade from disk
template<typename T>
void KDTree<T>::getAllLeafNodes(KDTreeNode<T> *node, std::unordered_set<KDTreeNode<T> *> &nodeSet) {
    if(node == NULL) return;
    if(node->isLeafNode())
        nodeSet.insert(node);
    else{
        getAllLeafNodes(node->leftTree, nodeSet);
        getAllLeafNodes(node->rightTree, nodeSet);
    }
}

//Following method used when KDTree is loade from disk
template<typename T>
void KDTree<T>::updateAllLeafNodes(std::unordered_set<KDTreeNode<T> *> &nodeSet) {
    for(auto node : nodeSet){
        node->clearAdjNodes();
        for(auto neighNode : nodeSet){
            if(node != neighNode){
                if(node->dimBnd.isAdjacent(neighNode->dimBnd)){
                    node->insertAdjNode(neighNode);
                }
            }
        }
    }
}

#endif //BUILD_KDTREE_KDTREE_H
