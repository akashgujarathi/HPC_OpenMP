all:
	g++ -O4 -fopenmp -o assgn_2_1 final_version.cpp
	@echo "./assgn_2_1 dimensions points threads"

clean:
	rm assgn_2_1
