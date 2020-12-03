A B C D E:
	# @#g++ -std=c++17 -Wall -Wextra -fsanitize=address -fsanitize=undefined -Wno-sign-compare -O2 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D D3BUG=true -g "$@.cpp" -o "$@.exe"
	@g++ -std=c++17 -Wall -Wextra -Wno-sign-compare -O0 -D D3BUG=true -g "$@.cpp" -o "$@.exe"
	@"$@.exe" < "$@.in" 2>&1 | tee "$@.out"

clean:
	@rm -f *.exe *.out
