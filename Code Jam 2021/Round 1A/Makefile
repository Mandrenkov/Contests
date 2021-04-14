A B C:
	@g++ -std=c++17 -O0 -g -D D3BUG=true -g -o "$@.exe" "$@.cpp"
	@"$@.exe" < "$@.in" 2>&1 | tee "$@.out"

clean:
	@rm -f *.exe *.out
