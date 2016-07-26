#include <iostream>
#include <vector>
#include "KDTreeApplication.h"
using namespace std;

int main(int argc, char * argv[]) {
    string treefile(argv[1]);
    string qfile(argv[2]);
    string opfile(argv[3]);
    KDTreeApplication *kdtapp_ld = KDTreeApplication::loadFromDisk(treefile);
    kdtapp_ld->queryFromFile(qfile, opfile);
    cout << "query_kdtree: output is saved to " << opfile << endl;
    return 0;
 }


