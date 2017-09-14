(* Stack Machine Interpreter implemented by Akilesh B, CS13B1042 *)

class List {            -- Class for an empty list. 

   isNil() : Bool { true };                      -- returns true if list is empty. 

   head_String() : String {{abort(); "0";}};

   tail()  : List { { abort(); self; } };       -- returns the tail of the list, everything except head. This is the pop function of stack
   
   cons_String(s : String) : List {             -- adds s to the start of the list and returns new list 
      (new Cons).init_String(s, self)
   };

};

(* Cons class inherits properties of class list *)

class Cons inherits List {          

   car_String : String;

   cdr : List;	

   isNil() : Bool { false };

   head_String() : String { car_String };

   tail()  : List { cdr };

   init_String(s : String, rest: List) : List {       -- adds s to the start of the list and returns new list.
      {
         car_String <- s;
         cdr <- rest;
         self;
      }
   };
};

(* Main class which inherits IO *)
class Main inherits IO {

   mylist : List;
   m : String;

(* method to print the list *)
   print_list(l : List) : Object {
      if l.head_String() = "" 
        then out_string("")
      else {
          out_string(l.head_String());  
			   out_string("\n");
			   print_list(l.tail());
		        }
      fi
   };

(* If a + is encountered, pop the last two elements of the stack, add them and push it back to the stack *)
   add(l : List) : List {  
      {
      let a: Int, b : Int, c: String 
         in
         { 
         a <- (new A2I).a2i(l.head_String());       -- convert string to integer
         l <- l.tail();
         b <- (new A2I).a2i(l.head_String());
         l <- l.tail();
         c <- (new A2I).i2a(a + b);                 -- convert it back to string.
         l <- (new Cons).init_String(c, l);
         };
      l;
      }  
   };

(* If a s is encountered, swap the top two elements of the stack *)
   swap(l : List) : List {
      {
      let a: String, b: String
         in
         {
         a <- l.head_String();
         l <- l.tail();
         b <- l.head_String();
         l <- l.tail();
         l <- (new Cons).init_String(a, l);
         l <- (new Cons).init_String(b, l);   
         };
      l;
      }
   };

   main() : Object {
      {	
      mylist <- new List.cons_String("");
	 
       out_string(">");
       m <- in_string();    -- m is the input string obtained from the user.

       while(not m = "x") loop        -- as long as m is not equal to x
       {
         if(m = "e") then
         {
            if(mylist.head_String() = "+") then
               {
                  let d: List <- add(mylist.tail()) in 
                  {  
                     mylist <- d;   
                  };
               }
             else
               {
                  if(mylist.head_String() = "s") then
                     {
                        let d: List <- swap(mylist.tail()) in 
                        {
                           mylist <- d;   
                        };
                     } 
                  else
                  out_string("")
                  fi;
               }
               fi;
         }
         else
            {
               if(m = "d") then
               {
                  print_list(mylist);
               }
               else
                mylist <- (new Cons).init_String(m, mylist)
               fi;
            }
            fi; 
          
        out_string(">");
        m <- in_string();
       }
       pool;
    
    }  
   };

};



