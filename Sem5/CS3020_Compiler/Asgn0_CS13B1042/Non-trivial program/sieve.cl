(* Non trivial program #2
	Implemented by Akilesh B CS13B1042
	given n, generated all primes less than n 
	Sieve of Eratosthenes
*)

(* 
* List class which has some operations defined on list objects, this specifies operations on empty list 
* addend function adds an element to the end of the list
*)

Class List inherits IO { 
        
	isNil() : Bool 
	{{ abort(); true; }};

	cons(head : Int) : Cons {
	  (let node : Cons <- new Cons in
		node.init(head,self)
	  )
	};

	car() : Int 
	{ { abort(); new Int; } };

	cdr() : List 
	{ { abort(); new List; } };

	insert(i : Int) : List 
	{ cdr() };

	addend(i : Int) : List 
	{ cdr() };

	modify(i : Int) : Int { car() };

	check(i : Int) : Int { car() };

	print_list() : Object { abort() };
};

(* 
* Cons inherits all operations from List. It has two attributes to hold: current node value and the rest of the list 
*)

Class Cons inherits List {
	xcar : Int;  
	xcdr : List; 

	isNil() : Bool { false };

	init(head : Int, tail : List) : Cons {
	  {
	    xcar <- head;
	    xcdr <- tail;
	    self;
	  }
	};
	  
	car() : Int { xcar };

	cdr() : List { xcdr };

	insert(i : Int) : List {
		if i < xcar then
			(new Cons).init(i,self)
		else
			(new Cons).init(xcar,xcdr.insert(i))
		fi
	};


	addend(i : Int) : List { (new Cons).init(xcar, xcdr.addend(i)) };

	print_list() : Object {
		{
		     out_int(xcar);
		     out_string("\n");
		     xcdr.print_list();
		}
	};

	modify(i : Int) : Int {			(* makes ith element of list as 1, ie assign arr[i] = 1 *)
			if(0 < i)
			then xcdr.modify(i-1)
			else
			xcar <- 1
			fi	
	};

	check(i : Int) : Int {			(* returns ith element of the list, ie return arr[i] *)
			if(0 < i)
			then xcdr.check(i-1)
			else
			xcar
			fi	
};	

};

Class Empty inherits List {
	isNil() : Bool
	 { true };

	insert(i : Int) : List 
	{ addend(i) };

	addend(i : Int) : List 
	{ (new Cons).init(i,self) };

	print_list() : Object 
	{ true };

};


Class Main inherits A2I {

	myList : List;

	perf_action(i : Int) : List {
	    {
		myList <- new Empty;
		(let j : Int <- 0
		 in
		   while j < i 
		   loop 
		     {
		       myList <- (new Cons).init(0,myList);
		       j <- j + 1;
		     } 
		   pool
		);
		myList;
	    }
	};

	markMultiples(c: List, a: Int, b: Int): Object { (* function to mark multiples *)
		{
		(let f: Int <- 2,
		g: Int <- 0
		in
		 while ((g <- f*a) <= b)
		 loop
		 {
		 	c.modify(g-1); 
		 	f <- f+1;  
		 }
		 pool
		);
	}
	};

	sieve(q : Int) : Object {   (* main sieve function *)
		if(2 <= q)
	     then {
	    	let modList: List <- perf_action(q),
	     	i: Int <- 1 in {
	     	(* modList.print_list(); *)
	     	while i < q
	     	loop
	     	{
	     		if(modList.check(i) = 0)
	     		then {
	     			(* (new IO).out_string("inside zero"); *) 
	     			(new IO).out_int(i+1);
	     			(new IO).out_string("\n");
	     			markMultiples(modList, i+1, q);
	     		}
	     		else 
	     			(new IO).out_string("")
	     		fi;	
	     		i <- i+1;
	     	}
	     	pool;
	     };
	  }
	  else
	  	(new IO).out_string("")
	  fi	 
	};		

	main() : Object {
	   {
	     (new IO).out_string("Enter value of n");
	     let n: Int <- ((new IO).in_int())
	      in
	       sieve(n);
}
};			    
};




