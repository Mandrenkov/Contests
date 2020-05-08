A B C D E F G H I J K L:
	@g++ -std=c++17 -Wfatal-errors -O2 -o "$@.exe" "$@.cpp"
	@"$@.exe" < "$@.in" 2>&1 | tee -a "$@.out" ; \

clean:
	@rm -f *.exe *.out
