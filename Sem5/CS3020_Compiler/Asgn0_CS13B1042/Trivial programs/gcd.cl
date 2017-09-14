(* recursive method to find gcd of a function  Implemented by Akilesh B CS13B1042  *)
class Main inherits A2I {
	
	main() : Object {
		(new IO).out_string(i2a(gcd(a2i((new IO).in_string()),a2i((new IO).in_string()))).concat("\n"))
	};

	gcd(a: Int, b: Int): Int {		(* gcd function *)
		if(a = b) 			 
		then a 
		else if(b < a)
		then gcd(a-b, b)
		else 
		gcd(a, b-a)
		fi
		fi 
	};
};
