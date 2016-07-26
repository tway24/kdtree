========================================================================================================
'kdtree' directory structure:

    - library: contains different header files for KDTree library
    		+ CSVIterator.h : To read CSV file
    		+ KDUtils.h : contains utility functions
    		+ Point.h : implements Point class to represent K-dimensional point
    		+ DimensionBondary.h : implements DimensionBondary class to represent K-dimensional min-max ranges
    		+ KDTreeNode.h : implements KDTreeNode class for tree node
    		+ KDTree.h : implements KDTree class used as library interface

    - input: contains training data point csv file and query point csv files
    		+ sample_data.csv
    		+ query_data.csv

    - output: generated output index file is placed in this directory

    - treeDB: generated tree database file is placed in this directory

    - build_kdtree: application to read sample_data.csv file, generate kdtree and save to treeDB directory
    		+ KDTreeApplication.h : Instantiates KDTree class
    		+ main.cpp
    		+ makefile

    - query_kdtree: application to read query_data.csv file, load kdtree from treeDB directory and 
		    create output to output directory
    		+ KDTreeApplication.h : Instantiates KDTree class
    		+ main.cpp
    		+ makefile

    - test: contains python script used to generate expected output. This expected output is used to compare
	    actual output. python script uses numpy, pandas and sklearn.neighbors for this.

========================================================================================================
How to run:
    - build_kdtree
	> make all  : this will compile and generate binary 'build_kdtree' and also runs binary
	> make run  : this will run 'build_kdtree' binary.Following command line arguments are needed:
		      1. path to sample_data.csv 
		      2. path to output tree file 
	> make clean : removes build_kdtree binary

    - query_kdtree
	> make all  : this will compile and generate binary 'query_kdtree' and also runs binary
	> make run  : this will run 'query_kdtree' binary.Following command line arguments are needed:
		      1. path to stored tree file 
		      2. path to query csv file  
		      2. path to output file  
	> make clean : removes query_kdtree binary


NOTE: In the submitted directory, binaries (build_kdtree and query_kdtree) are already present


