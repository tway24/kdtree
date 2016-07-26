#------------------------------------------------------------------------------

SOURCE=main.cpp KDTreeApplication.h
MYPROGRAM=build_kdtree
MYINCLUDES=../library/
CC=g++
COMP_STD=-std=c++11
INPUT_FILE=../input/sample_data.csv
OUTPUT_FILE=../treeDB/kdtree_uber.txt
#------------------------------------------------------------------------------



all: $(MYPROGRAM) run



$(MYPROGRAM): $(SOURCE)

	$(CC) $(COMP_STD) -I$(MYINCLUDES) $(SOURCE) -o$(MYPROGRAM)

run:
	./$(MYPROGRAM) $(INPUT_FILE) $(OUTPUT_FILE)

clean:

	rm -f $(MYPROGRAM)



