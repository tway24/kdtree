Finding a nearest neighbor in high dimensional space is very computation intesive task. Brute-force algorithm will require to find distance from each point in training dataset and then select closest point. So, time complexity of brute-force search will be O(n*k), where n is total number of points in dataset and k is the dimensionality.

k-d tree is a binary tree, which divides data at each level based on some criteria. And, leaf node contains set of K-dimensional datapoints. Becaues of binary tree datastructure, searching for nearest neighbor is efficient. But, sometime depending upon division of dataset, k-d tree might not give accurate nearest neighbor.  
For more information please visit this wiki page.
https://en.wikipedia.org/wiki/K-d_tree

To imporve k-d tree accuracy and keeping efficiency, I have developed this library which achieve following time complexity:
Time complexity = O(log n) + O(a * p *k) = O(a * p * k) // where ‘a’ is total adjacent nodes and ‘p’ is number of points in each node

Time complexity comparision with brute-formce:
If there are 10000 5-dimensional datapoints, brute-force will consume O(50000). 
If k-d tree is configured to have 10 points per leaf node and to search 10 adjacent nodes, it will consume O(10*10*5) = O(500)

For more details on implementation and library usage please refer to Implementation_details.pdf . 
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





