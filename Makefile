dependency:
	cd build && cmake .. --graphviz=dependency.dot && dot -Tpng dependency.dot -o dependency.png

prepare:
	@echo ">>> Building palpatine ...";
	@if [ -d ./build ]; then rm -rf ./build; fi
	@mkdir build; 
	@cd build; \
	cmake -S .. -B .; \
	make; \
	
	@echo ">>> Done";
	@echo ">>> Run ./palpatine [options] to start the application";


test:
	@echo ">>> Running tests ...";
	@cd build; \
	cmake --build . --target unit_tests ;
	@cd build/tests; \
	./unit_tests;

codecov:
	@echo ">>> Running tests ...";
	@cd build; \
	cmake --build . --target unit_tests ;
	gcovr --html -o build/index.html ;
