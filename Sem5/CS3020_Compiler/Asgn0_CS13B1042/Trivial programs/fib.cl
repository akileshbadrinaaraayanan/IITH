(* finding nth fibonacci number Implemented by Akilesh B CS13B1042 *)

class Main inherits A2I {
	
	main() : Object {
		(new IO).out_string(i2a(fib(a2i((new IO).in_string()))).concat("\n"))
	};

	fib(a: Int): Int {
		if(a <= 1) 
		then a 
		else 
		fib(a-1) + fib(a-2)
		fi
	};
};
