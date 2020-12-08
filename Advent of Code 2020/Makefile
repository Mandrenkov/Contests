%1 %2:
	@$(eval DAY=$(subst 1,,$(subst 2,,$@)))
	@g++ -std=c++17 -Wfatal-errors -O2 -o "$@.exe" "$@.cpp"
	@"$@.exe" < "$(DAY).in" 2>&1 | tee "$(DAY).out"

clean:
	@rm -f *.exe *.out
