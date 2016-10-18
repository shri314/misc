all: run

run: a.out b.out
	LD_LIBRARY_PATH=/data1/local/v2/lib ./a.out
	@echo
	LD_LIBRARY_PATH=/data1/local/v2/lib ./b.out

html: tagged_tuple.hpp Usage.cpp FunctionalLogic.cpp
	@mkdir html-t; cd ./html-t; for i in $^; do /usr/bin/ex +:TOhtml +wqa ../$$i; mv ../$$i.html .; done; echo
	@rm -rf html; mv html-t html

a.out: Usage.cpp tagged_tuple.hpp
	LD_LIBRARY_PATH=/data1/local/v2/lib /data1/local/v2/bin/g++ -std=c++11 -I /data1/local/v2/include $< -o $@

b.out: FunctionalLogic.cpp
	LD_LIBRARY_PATH=/data1/local/v2/lib /data1/local/v2/bin/g++ -std=c++11 -I /data1/local/v2/include $< -o $@

clean:
	rm -f a.out b.out *.o *.html
	rm -rf html
