A B C D:
	@g++ -std=c++17 -Wall -Wextra -fsanitize=address -fsanitize=undefined -Wno-sign-compare -O0 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D D3BUG=true -g -o "$@.exe" "$@.cpp"
	@"$@.exe" < "$@.in" 2>&1 | tee "$@.out"

clean:
	@rm -f *.exe *.out
