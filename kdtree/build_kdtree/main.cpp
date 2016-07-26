#include <iostream>
#include <vector>
#include "KDTreeApplication.h"
using namespace std;

int main(int argc, char * argv[]) {
    string ipfile(argv[1]);
    string treefile(argv[2]);
    KDTreeApplication kdtapp(ipfile);
    //Write to disk
    kdtapp.saveToDisk(treefile);
    cout << "KDTree is created and saved to: " << treefile << endl;
    return 0;
 }


