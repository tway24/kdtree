//
// Created by tshah on 7/16/2016.
//

#ifndef CLTEST_KDTREEAPPLICATION_H
#define CLTEST_KDTREEAPPLICATION_H

#include <fstream>
#include "KDTree.h"
#include "KDTreeExtended.h"
#include "Point.h"
#include <iostream>
#include "CSVIterator.h"


class KDTreeApplication{
    KDTree<float> * kdt;
    CSVParser cvp;
    int nodeSize = 10;
    int adjLevels = 2;
public:
    KDTreeApplication(){};
    KDTreeApplication(string csvpath) {
        vector<vector<float>> dataMatrix = cvp.parse(csvpath);
        kdt = new KDTree<float>(dataMatrix, nodeSize, adjLevels);
        //kdt = new KDTree<float>(dataMatrix, nodeSize, adjLevels, getCutDimensionBad);
    }

    void queryFromFile(string queryFile, string outFile){
        vector<vector<float>> queryMatrix = cvp.parse(queryFile);
        if(queryMatrix.empty())
            //TODO: exception
            return;
        int numPoints = queryMatrix.size();
        int numDim = queryMatrix[0].size();
        ostringstream resultBuf;
        for(int i = 0 ; i < numPoints ; ++i){
            Point<float> queryPoint(numDim,queryMatrix[i],0);
            //pair<int, float> nrstPt = getNearestPointIndex(queryPoint);
            //resultBuf << nrstPt.first << "," << nrstPt.second << "\n";
            int index = getNearestPointIndex(queryPoint);
            resultBuf << index << "\n";
        }
        string resultStr = resultBuf.str();
        ofstream ofs(outFile);
        if(!ofs){
            //TODO : exception
        }else{
            ofs << resultStr;
            ofs.close();
        }
    }
    int getNearestPointIndex(Point<float> queryPoint){
    //pair<int, float> getNearestPointIndex(Point<float> queryPoint){
        pair<int, float> nearestPt = kdt->getNearestNeighbor(queryPoint);
        return nearestPt.first;
        //return nearestPt;
    }
    void saveToDisk(string filepath){
        ofstream ofs(filepath);
        if(!ofs) {
            //TODO: raise exception
        }
        kdt->saveToDisk(ofs);
        //kdt.traversePrint();
    }
    static  KDTreeApplication * loadFromDisk(string filepath){
        KDTreeApplication *kdtapp = NULL;
        ifstream ifs(filepath);
        if(!ifs){
            //TODO: exception
        }else{
            kdtapp = new KDTreeApplication;
            kdtapp->kdt = KDTree<float>::loadFromDisk(ifs);
        }
        return kdtapp;
    }
};

#endif //CLTEST_KDTREEAPPLICATION_H
