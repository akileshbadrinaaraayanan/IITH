(*question 1 miniassignment 2*)
(*implemented by Akilesh B, CS13B1042*)

(*Count 
the number of negative integers in a list l*)
fun countnegative(list1:int list):int=
(case list1
	of nil=>0
	| x::rem=>if x<0 then 1+countnegative(rem) else countnegative(rem));

(*Count 
how often a given string s occurs in a list l*)
fun numberofs(s:string,list1:string list):int=
(case list1
	of nil=>0
	|x::rem=>if x=s then 1+numberofs(s,rem) else numberofs(s,rem));   

(*Construct
a list of all the integers in a list l 
which are greater than a value v.*)
fun listgreaterv(v:int,list1:int list):int list=
(case list1
	of nil=>[]
	|x::rem=>if x>v then x::listgreaterv(v,rem) else listgreaterv(v,rem));

(*Merge two sorted string lists s1 and s2*)
fun smerge(list1:string list,list2:string list):string list=
(case (list1,list2)
	of (nil,ys)=>ys
	|(rem,nil)=>rem
	|(x::rem,y::ys)=>if x<y then x::smerge(rem,y::ys) else y::smerge(x::rem,ys));


(*Construct a single sorted string list 
from a list of sorted string lists.*)
fun slmerge(list1:string list list):string list=
(case list1
	of x::rem => if rem=nil then x else smerge(x,slmerge(rem)));


(*stock consists of item name, no. in stock, reorder level*)
(*Construct a list of those stock records
 with the number in stock less than the reorder level.*)
fun check(list1:(string*int*int) list):(string*int*int)list=
(case list1
	of nil=>[]
	| x::rem => if #2 x < #3 x then x::check(rem) else check(rem));

(*Update a list of stock records from a list of update records represented as tuples of :
item name - string
update no. - integer
by constructing a new stock list of records with the number in stock increased by the update number.*)
fun update1(list3:(string * int * int) list, s:string * int):(string * int * int) list =
	(case list3 
		of nil => []
		| y::rem2 => if #1 y = #1 s then (#1 y, #2 y + #2 s, #3 y)::(update1(rem2,s)) else y::update1(rem2,s));

fun update(list1:(string * int * int) list,list2:(string * int) list):(string * int * int) list =
	(case list2
		of nil => list1
		| x::rem => update(update1(list1,x),rem));

(*End of question 1*)