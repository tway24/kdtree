//
// Created by tshah on 7/18/2016.
//

#ifndef BUILD_KDTREE_KDTREEEXTENDED_H
#define BUILD_KDTREE_KDTREEEXTENDED_H

#include "Point.h"

//For given dimension, return minimum value from start to end poins
template<typename T>
T getMinDimension(std::vector<Point<T>> & dataPoints, int start, int end, int dimension){
    T minDim = dataPoints[0][dimension];
    for(int i = start; i < end ; ++i){
        if(minDim > dataPoints[i][dimension])
            minDim = dataPoints[i][dimension];
    }
    return minDim;
}

//For given dimension, return maximum value from start to end poins
template<typename T>
T getMaxDimension(std::vector<Point<T>> & dataPoints, int start, int end, int dimension){
    T maxDim = dataPoints[0][dimension];
    for(int i = start; i < end ; ++i){
        if(maxDim < dataPoints[i][dimension])
            maxDim = dataPoints[i][dimension];
    }
    return maxDim;
}

//By default following function is used for spliting axis
template<typename T>
int getCutDimension(std::vector<Point<T>> & dataPoints, int start, int end, int numDimensions){
    //T maxDiff = abs(dataPoints[start][0] - dataPoints[end-1][0]);
    T maxDiff = 0;
    int maxDiffDim = 0;
    for(int i = 0 ; i < numDimensions ; ++i){
        T curMin = getMinDimension(dataPoints, start, end, i);
        T curMax = getMaxDimension(dataPoints, start, end, i);
        T diff = abs(curMin - curMax);
        if(diff > maxDiff){
            maxDiff = diff;
            maxDiffDim = i;
        }
    }
    return maxDiffDim;
}

//For extending to support different way of spliting axis
//As an example: following function can be passed as the argument while KDTree instance is created
template<typename T>
int getCutDimensionBad(std::vector<Point<T>> & dataPoints, int start, int end, int numDimensions){
    return 0;
}

#endif //BUILD_KDTREE_KDTREEEXTENDED_H
