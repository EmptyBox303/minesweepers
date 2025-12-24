naive-gen: naive_gen.cpp head.hpp
	g++  -Wall naive_gen.cpp -o naive-gen

solver: solve.cpp head.hpp solve.hpp
	g++ -Wall solve.cpp -o solve
