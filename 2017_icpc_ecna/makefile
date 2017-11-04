b:
	g++ -std=c++11 -Wfatal-errors -o b.exe b.cpp
	b.exe < b.in.txt 2>&1 | tee b.out.txt

f:
	g++ -std=c++11 -Wfatal-errors -o f.exe f.cpp
	f.exe < f.in.txt 2>&1 | tee f.out.txt

clean:
	rm *.exe *.out.txt