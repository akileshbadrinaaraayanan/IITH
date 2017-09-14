/* Assignment 3 part 2
	Implemented by Akilesh B CS13B1042
*/
:- use_module(library(clpfd)).
/* finds all possible trees with N nodes */
distincttree(1,[]).
distincttree(N, [F|L]):-
	N #> 1,
	P is N-1,
	between(1,P,M),
	distincttree(M,F),R is P-M+1,
	distincttree(R,L).

/* to print n spaces */
prt_space(N):-
	N #> 0,
	write(' '),
	prt_space(N-1).

/* if list is empty */
prt_list(S,[]).

/* to print the generated list */
prt_list(S,Lst):-
	[L|R]=Lst,
	(prt_space(S);true),
	write('x'),nl,
	prt_list(S+3,L),
	prt_list(S,R).

/* given n, gives all possible distinct general trees with n nodes */
trees(N):-
	distincttree(N,A),
	write('x'),nl,
	S=3,
	prt_list(S,A).





