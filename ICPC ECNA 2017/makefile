a b c d e f g h i j:
	@g++ -std=c++11 -Wfatal-errors -o "$@.exe" "$@.cpp"
	@rm -f "$@.out.txt"
	@for infile in `ls | grep "$@[0-9].in.txt"`; do \
		echo "Input \"$$infile\"\n-----------------" | tee -a "$@.out.txt" ; \
		"$@.exe" < "$$infile" 2>&1 | tee -a "$@.out.txt" ; \
		echo "" | tee -a "$@.out.txt" ; \
	done

clean:
	@rm -f *.exe *.out.txt