#
# Proof of concept makefile
# 

.PHONY: default all clean distclean buildcmake

default:  all

all: buildcmake

buildcmake: build
	cd build && cmake .. && make 

build: 
	mkdir -p build	

clean:
	@rm -rf build 2>/dev/null

distclean: 	
	clean

