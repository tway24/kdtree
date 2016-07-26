//
// Created by tshah on 7/18/2016.
//

#ifndef BUILD_KDTREE_POINT_H
#define BUILD_KDTREE_POINT_H
#include <iostream>
#include <cmath>
#include <sstream>
#include "KDUtils.h"
using namespace std;

template<typename T = float> class Point{
    int numDimensions = 0; // number of dimensions
    std::vector<T> data;
    int index = 0;  // Index used for tracking sample index
public:
    Point(){}
    Point(int n, std::vector<T> dt, int idx);
    // Used to set or get
    T & operator[](int idx){
        return data[idx];
    }
    T distance(Point<T> pt);
    //destruct object
    void destroy();
    ~Point();
    int getIndex(){ return index;}
    int getDataSize(){ return data.size();}
    //serailize data to wrtite to the disk
    void serialize(std::string &s);
    //dserailize string to generate Point object
    static Point<T> deserialize(std::istringstream & is, int numDim);
};

template<typename T>
Point<T>::Point(int n, std::vector<T> dt, int idx) : numDimensions(n), index(idx){
    data = dt;
    data = dt;
}

template<typename T>
void Point<T>::destroy(){
    data.erase(data.begin(), data.end());
}

template<typename T>
Point<T>::~Point(){
    destroy();
}

template<typename T>
T Point<T>::distance(Point<T> pt){
    T sq_dist = 0;
    for(int i = 0 ; i < numDimensions ; ++i){
        T diff = pt[i] - data[i];
        sq_dist += diff * diff;
    }
    return sqrt(sq_dist);
}

template<typename T>
void Point<T>::serialize(std::string &s){
    serializeData(s, index);
    serializeVec(s, data);
    //s.append("\n");
}

template<typename T>
Point<T> Point<T>::deserialize(std::istringstream & is, int numDim){
    Point<T> pt;
    pt.index = deserialData<int>(is);
    pt.numDimensions = numDim;
    for(int i = 0 ; i < numDim; ++i){
        pt.data.push_back(deserialData<T>(is));
    }
    return pt;
}
#endif //BUILD_KDTREE_POINT_H
