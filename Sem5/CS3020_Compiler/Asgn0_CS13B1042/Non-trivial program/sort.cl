(*	Non trivial program #1
	Implemented by Akilesh B CS13B1042
	Program to get n numbers from the user, store in a linked list and sort it. 
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

	car() : Int 			(* car stores the first element of the list *)
	{ { abort(); new Int; } };

	cdr() : List 			(* cdr stores the last element of the list *)	
	{ { abort(); new List; } };

	insert(i : Int) : List 
	{ cdr() };

	sort() : List 
	{ cdr() };

	addend(i : Int) : List 
	{ cdr() };
	
	print_list() : Object { abort() };
};

(* 
* Cons inherits all operations from List. It has two attributes to hold: current node value and the rest of the list 
*)

Class Cons inherits List {
	xcar : Int;  
	xcdr : List; 

	isNil() : Bool { false };

	init(head : Int, tail : List) : Cons {		(* function to initialize values *)
	  {
	    xcar <- head;
	    xcdr <- tail;
	    self;
	  }
	};
	  
	car() : Int { xcar };

	cdr() : List { xcdr };

	sort() : List { (xcdr.sort()).insert(xcar) };

	insert(i : Int) : List {
		if i < xcar then
			(new Cons).init(i,self)
		else
			(new Cons).init(xcar,xcdr.insert(i))
		fi
	};


	addend(i : Int) : List { (new Cons).init(xcar, xcdr.addend(i)) }; (* this adds to the end of the list *)

	print_list() : Object {			(* to print the list *)
		{
		     out_int(xcar);
		     out_string("\n");
		     xcdr.print_list(); 
		}
	};
};

Class Empty inherits List {
	isNil() : Bool
	 { true };

	sort() : List 
	{ self };

	insert(i : Int) : List 
	{ addend(i) };

	addend(i : Int) : List 
	{ (new Cons).init(i,self) };

	print_list() : Object 
	{ true };

};


Class Main inherits A2I {

	myList : List;

	perf_action(i : Int) : List {		(* gets input from the user and store in a linked list *)
	    {
		myList <- new Empty;	(* initialize as empty *)
		(let j : Int <- 0,
		m: Int <- 0	
		 in
		   while j < i 
		   loop 
		     {

		       (new IO).out_string("enter number ");
		       (new IO).out_int(j+1);	
		       (new IO).out_string(":");
		       m <- a2i((new IO).in_string());
		       myList <- (new Cons).init(m,myList);
		       j <- j + 1;
		     } 
		   pool
		);
		myList;
	    }
	};		

	main() : Object {
	   {

	     (new IO).out_string("Count of numbers to sort?");	(* get input from user *)
	     perf_action((new IO).in_int()).sort().print_list();
	   }
	};
};			    





