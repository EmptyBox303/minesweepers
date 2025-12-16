naive-gen: naive_gen.cpp head.hpp
	g++ naive_gen.cpp -o naive-gen

solver: solve.cpp head.hpp
	g++ solve.cpp -o solve
