A B C E:
	@g++ -std=c++17 -O2 -D D3BUG=true -g -o "$@.exe" "$@.cpp"
	@"$@.exe" < "$@.in" 2>&1 | tee "$@.out"

D:
	@g++ -std=c++17 -O2 -D D3BUG=true -g -o "$@.exe" "$@.cpp"

clean:
	@rm -f *.exe *.out
