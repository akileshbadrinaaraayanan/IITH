(* tower of hanoi problem *)

class Main inherits A2I {
	
	main() : Object {
		let l: Int <- a2i((new IO).in_string()),
		m: Int <- a2i((new IO).in_string()),
		n: Int <- a2i((new IO).in_string()),
		o: Int <- a2i((new IO).in_string())
		in 
			toh(l,m,n,o)
	};

	toh(a: Int, b: Int, c: Int, d: Int):Object {
		if(a = 1)
		then {
		(new IO).out_string("\n Move disk 1 from rod ");
		(new IO).out_int(b);
		(new IO).out_string(" to rod ");
		(new IO).out_int(c);
	    }
	    else{
		toh(a-1, b, d, c);
		(new IO).out_string("\n Move disk ");
		(new IO).out_int(a);
		(new IO).out_string(" from rod ");
		(new IO).out_int(b);
		(new IO).out_string(" to rod ");
		(new IO).out_int(c);
		toh(a-1, d, c, b);
	    }
	    fi
	};
};
