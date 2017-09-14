/* Assignment 3 part 1
Implemented by Akilesh B CS13B1042
*/
:- use_module(library(clpfd)).

/* To find the length of a given list. */

list_length([],0).
list_length([_|T],R):- 
	list_length(T,R1),R is R1+1.

/* Predicates to find possible combinations of binary trees given number of nodes */

bintree(0,[]).
bintree(1,[[],[]]).
bintree(N, [Left, Right]) :-
    N #> 1,
    N #= N1 + N2 + 1,
    N1 #>= 0,N2 #>= 0,
    bintree(N1, Left),bintree(N2, Right).

/* to print n spaces */
prt_space(N):-
	N #> 0,
	write(' '),
	prt_space(N-1).

/* to print list when empty */
prt_list(S,[]):-
	(prt_space(S);true),
	write('-'),nl.

/* to print list when not empty */
prt_list(S,Lst):-
	list_length(Lst,N),
	N #> 0,
	[Ll,Rl]=Lst,
	(prt_space(S);true),
	write('x'),nl,
	prt_list(S+3,Ll),prt_list(S+3,Rl).

/* which takes in number of nodes as input and prints all possible binary trees */
bintrees(N):-
	bintree(N,L),
	S = 3,
	prt_list(S,L).




