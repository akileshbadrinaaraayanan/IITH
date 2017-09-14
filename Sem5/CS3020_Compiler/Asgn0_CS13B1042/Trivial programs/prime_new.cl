(* recursive implementation to see if a number is prime or not *)

class Main inherits A2I {
	
	main() : Object {
			
		let l: Int <- a2i((new IO).in_string())
		in
		{
		if(is_prime(l, l/2) = 1)
		then {
		(new IO).out_int(l);	
		(new IO).out_string(" is a prime number");
		}
		else { 
		(new IO).out_int(l);
		(new IO).out_string(" is not a prime number");
		}
		fi;
		}
	};

	is_prime(a: Int, b: Int): Int {
		
		if(b = 1)
		 	then 1
		else  if((a - (a/b)*b) = 0)     (* it is equivalent to a mod b *)
			then 0
		else
			is_prime(a, b-1)
		fi
		fi
};
};