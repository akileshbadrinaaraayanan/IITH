(* recursive method to find gcd of a function  Implemented by Akilesh B CS13B1042 *)
class Main inherits A2I {
	
	main() : Object { 
		let n: Bool in
		n = gcd(10,7)
	};

	gcd(a: Int, b: Int): Bool {		(* gcd function *)
		if(a = b) 			 
		then True           	-- First letter of constants true and false should be capital. Violation of 10.4 
		Else if(b < a)
		then false
		else 
		true
		fi
		fi 
	};
};
