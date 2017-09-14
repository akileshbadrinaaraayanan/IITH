(*question no 2 mini assignment *)
(*implemented by Akilesh B cs13b1042*)

(*Extract 
the leftmost n letters from string s :*)
fun temp1(n:int,s:char list):char list =
(case (n,s)
	of (0,xs) => []
	|(n,nil)=>[]
	| (n,x::xs) => x::temp1(n-1,xs));

(*convert a string to character list => use implode*)
fun temp2(n:int,s:char list):string = implode(temp1(n,s));

(*convert a character list back to string => use explode*)
fun left (n:int,s:string):string = temp2(n,explode(s));


(*Extract
 the rightmost n letters from string s :*)
fun temp3(n:int,s1:char list):char list =
(case (n,s1)
	of (0,xs) => xs
	| (n,x::xs) => if n<0 then x::xs else temp3(n-1,xs));
fun temp4(n:int,s1:char list):string = implode(temp3(n,s1));

(*consider string length - n(from left)*)
fun right (n:int,s1:string):string = 
	temp4((size s1) - n,explode(s1));



(*Extract n letters 
starting with the l’ letter from string s ie from position n1 extract n letters *)
fun temp5(n:int,n1:int,s1:char list):char list = 
(case (n,n1,s1)
	of (n,0,x::xs) => temp1(n,x::xs)
	| (n,n1,x::xs) => if xs=nil then [] else temp5(n,n1-1,xs));

fun temp6(n:int,n1:int,s1:char list):string = implode(temp5(n,n1,s1));

fun middle (n:int,n1:int,s1:string):string = temp6(n,n1-1,explode(s1));


(*Find the position
 of the first occurence of string s1 
in string s2 :*)
fun remove(list1:char list):char list =
(case list1
	of (x::rem) => if rem=nil then [] else rem);

fun find2(list1:char list,list2:char list):int =
	if length list1 > length list2 then length list1 
	else if list1= explode(left(length list1,implode(list2))) then 1 else 1+find2(list1,remove(list2));

fun find1(list1:char list,list2:char list):int = 
	if find2(list1,list2) < length list2 then find2(list1,list2) else ~1;(*return -1 if not found*)

fun find (list1:string, list2:string):int = find1(explode(list1),explode(list2));


(*end of question 2*)






