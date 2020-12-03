# The part of the puzzle to be solved.
PART=1

A B C D E F G H I J K L M N O P Q R S T U V W X Y:
	@g++ -std=c++17 -Wfatal-errors -O2 -o "$@$(PART).exe" "$@$(PART).cpp"
	@"$@$(PART).exe" < "$@.in" 2>&1 | tee "$@.out"

clean:
	@rm -f *.exe *.out
