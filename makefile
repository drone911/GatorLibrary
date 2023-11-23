all:
	g++ main.cpp gatorLibrary.cpp reservations.cpp -o gatorLibrary

test: all
	for testfile in tests/test?.txt; do \
		./gatorLibrary $${testfile}; \
	done;
	for testfile in tests/test?.txt; do \
		base=$$(basename $${testfile} .txt); \
		echo "\033[1;32m[Tests] Comparing files $${base}_output_file.txt and $${base}.output.txt\033[0m"; \
		diff -iZ tests/$${base}_output_file.txt tests/$${base}.output.txt; \
		echo "\033[1;32m[Tests] Finished comparing files $${base}_output_file.txt and $${base}.output.txt\033[0m\033[0m"; \
	done;

clean:
	rm -f gatorLibrary
	rm -f tests/test?_output_file.txt