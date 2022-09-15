prepare:
	@echo ">>> Building palpatine ...";
	@if [ -d ./build ]; then rm -rf ./build; fi
	mkdir build; 
	cd build; \
	cmake -S .. -B .; \
	cd build/app; make; \
	
	@echo ">>> Done";


