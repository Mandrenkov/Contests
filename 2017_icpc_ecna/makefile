b f :
	g++ -std=c++11 -Wfatal-errors -o "$@.exe" "$@.cpp"
	"$@.exe" < "$@.in.txt" 2>&1 | tee "$@.out.txt"

clean:
	rm -f *.exe *.out.txt