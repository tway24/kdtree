//
// Created by tshah on 7/18/2016.
//

#ifndef BUILD_KDTREE_KDUTILS_H
#define BUILD_KDTREE_KDUTILS_H
#include <string>
#include <sstream>
#include <vector>

//Searialize given data to string
template<typename D>
void serializeData(std::string &s, D dataElem){
    std::ostringstream dtBuf;
    dtBuf << dataElem << ",";
    s.append(dtBuf.str());
}

//Searialize given vector to string
template<typename T>
void serializeVec(std::string &s, std::vector<T> vec){
    for(int i = 0 ; i < vec.size(); ++i){
        serializeData(s, vec[i]);
    }
}

//return string until ',' is found
std::string deserialString(std::istringstream & is){
    std::string s;
    getline(is, s, ',' );
    return s;
}

//desrialize and returns data of type T
template<typename T>
T deserialData(std::istringstream & is){
    std::string s = deserialString(is);
    std::istringstream ibuf(s);
    T data;
    ibuf >> data;
    return data;
}
#endif //BUILD_KDTREE_KDUTILS_H
