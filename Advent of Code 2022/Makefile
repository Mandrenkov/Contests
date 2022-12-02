% : day_%_solution.js
	@echo "Running sample..."
	@node $< day_$*_sample.txt
	@echo "\nRunning unique..."
	@node $< day_$*_unique.txt

%.js : %.ts
	@npx tsc $<
