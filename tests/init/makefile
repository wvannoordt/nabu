nabu   := $(realpath ../../src)
target := out.x

main:
	g++ -std=c++20 -I${nabu} main.cc -o ${target} && ./out.x && ${MAKE} -C ./graphic-debug -f makefile

clean:
	rm -f ${target}
