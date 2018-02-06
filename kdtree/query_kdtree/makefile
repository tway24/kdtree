#------------------------------------------------------------------------------

SOURCE=main.cpp KDTreeApplication.h
MYPROGRAM=query_kdtree
MYINCLUDES=../library/
CC=g++
COMP_STD=-std=c++11
TREE_FILE=../treeDB/kdtree.txt
QUERY_FILE=../input/query_data.csv
OUTPUT_FILE=../output/query_output.txt
#------------------------------------------------------------------------------



all: $(MYPROGRAM) run



$(MYPROGRAM): $(SOURCE)

	$(CC) $(COMP_STD) -I$(MYINCLUDES) $(SOURCE) -o$(MYPROGRAM)

run:
	./$(MYPROGRAM) $(TREE_FILE) $(QUERY_FILE) $(OUTPUT_FILE)

clean:
	rm -f $(MYPROGRAM)



