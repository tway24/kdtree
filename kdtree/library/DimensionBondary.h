//
// Created by tshah on 7/18/2016.
//

#ifndef BUILD_KDTREE_DIMENSIONBONDARY_H
#define BUILD_KDTREE_DIMENSIONBONDARY_H

#include <limits>
#include <sstream>
#include "KDUtils.h"

template<typename T = float> class DimensionBondary{
public:
    T dimMin;
    T dimMax;
    DimensionBondary(){
        dimMin = std::numeric_limits<T>::min();
        dimMax = std::numeric_limits<T>::max();
    }
    DimensionBondary(T mn, T mx) : dimMin(mn), dimMax(mx) {};
    // check if boundaries of two intersect each other
    bool isIntersect(DimensionBondary &db){
        return (dimMin <= db.dimMin && db.dimMin <= dimMax) || (dimMin <= db.dimMax && db.dimMax <= dimMax);
    }
    // check if boundaries of two are touching to each other
    bool isTouching(DimensionBondary &db){
        return (db.dimMin == dimMax || db.dimMax== dimMin);
    }
    //serailize data to wrtite to the disk
    void serialize(std::string &s){
        serializeData(s, dimMin);
        serializeData(s, dimMax);
    }
    //dserailize string to generate object
    static DimensionBondary<T> deserialize(std::istringstream & is){
        DimensionBondary<T> db;
        db.dimMin = deserialData<T>(is);
        db.dimMax = deserialData<T>(is);
        return db;
    }
};

template<typename T = float> class DimensionBondaryVec{
public:
    int numDimensions = 0;
    std::vector<DimensionBondary<T>> dimVec;
    DimensionBondaryVec(){}
    DimensionBondaryVec(int n) : numDimensions(n){
        dimVec  = std::vector<DimensionBondary<T>>(n);
    }
    // Used to set or get
    DimensionBondary<T> & operator[](int idx){
        return dimVec[idx];
    }
    //Check if one hyperPlane is adjacent to another one
    bool isAdjacent(DimensionBondaryVec dVec);
    void destroy(){
        dimVec.erase(dimVec.begin(), dimVec.end());
    }
    ~DimensionBondaryVec(){
        destroy();
    }
    void serialize(std::string &s);
    static DimensionBondaryVec<T> deserialize(std::istringstream & is, int numDim);
};

//Check if one hyperPlane is adjacent to another one
//Logic: if one dimension touches same dimesion and if rest of the dimensions intersect
// 		 then those two are adjacent to each other
//Space complexity: O(d)  // d is number of dimensions
//Time complexity: O(d) // d is number of dimensions
                        // even if there is nested loop, touches[i] will be true for
                        // at max only one dimension
template<typename T>
bool DimensionBondaryVec<T>::isAdjacent(DimensionBondaryVec dVec){
    std::vector<bool> intersects(numDimensions, false);
    std::vector<bool> touches(numDimensions, false);
    //check each dimension for 'touch' and 'intersect'
    for(int i = 0 ; i < numDimensions; ++i){
        intersects[i] = dimVec[i].isIntersect(dVec[i]);
        touches[i] = dimVec[i].isTouching(dVec[i]);
    }
    for(int i = 0 ; i < numDimensions; ++i){
        if(touches[i]){
            bool adjacent = true;
            for(int j = 0 ; j < numDimensions; ++j){
                if(!intersects[i]){
                    adjacent = false;
                    break;
                }
            }
            if(adjacent) return true;
        }
    }
    return false;
}

template<typename T>
void DimensionBondaryVec<T>::serialize(std::string &s){
    serializeData(s, numDimensions);
    for(int i = 0 ; i < numDimensions; ++i){
        dimVec[i].serialize(s);
    }
    //s.append("\n");
}

template<typename T>
DimensionBondaryVec<T> DimensionBondaryVec<T>::deserialize(std::istringstream & is, int numDim){
    DimensionBondaryVec<T> dimv;
    dimv.numDimensions = numDim;
    std::string s = deserialString(is);
    if(s.compare("NL")) { // leaf-nome
        for(int i = 0 ; i < numDim; ++i){
            DimensionBondary<T> db = DimensionBondary<T>::deserialize(is);
            dimv.dimVec.push_back(db);
        }
    }
    return dimv;
}



#endif //BUILD_KDTREE_DIMENSIONBONDARY_H
