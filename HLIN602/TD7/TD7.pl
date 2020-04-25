/* AUTEURS : HAYAAT HEBIRET ET JEREMIE ROUX */

/* EXERCICE A */

/* HOMME */
homme(albert).
homme(jean).
homme(paul).
homme(bertrand).
homme(louis).
homme(benoit).
homme(edgar).

/* FEMME */
femme(germaine).
femme(christiane).
femme(simone).
femme(marie).
femme(sophie).
femme(madeleine).

/* PARENT(X,Y) : X est parent de Y */
parent(albert,jean).
parent(jean,paul).
parent(paul,bertrand).
parent(paul,sophie).
parent(jean,simone).
parent(louis,benoit).
parent(paul,edgar).
parent(germaine,jean).
parent(christiane,simone).
parent(christiane,paul).
parent(simone,benoit).
parent(marie,bertrand).
parent(marie,sophie).
parent(madeleine,edgar).

/* MERE(X,Y) : Une femme X qui est parent de Y */
mere(X,Y) :- parent(X,Y), femme(X).

/* PERE(X,Y) : Un homme X qui est parent de Y */
pere(X,Y) :- parent(X,Y), homme(X).

/* PERE(X) : X est père */
pere(X) :- pere(X,_).

/* FILS(X,Y) : X est le fils de Y */
fils(X,Y) :- parent(Y,X), homme(X).

/* FILLE(X,Y) : X est la fille de Y */
fille(X,Y) :- parent(Y,X), femme(X).

/* GRAND_PERE(X,Y) : X est grand père de Y */
grand_pere(X,Y) :- pere(X,Z), parent(Z,Y).

/* GRAND_MERE(X,Y) : X est grand mère de Y */
grand_mere(X,Y) :- mere(X,Z), parent(Z,Y).

/* GRAND_PARENT(X,Y) : X est grand parent de Y */
grand_parent(X,Y) :- parent(X,Z), parent(Z,Y).

/* FRERE(X,Y) : X est frère Y */
frere(X,Y) :- pere(Z,X), mere(T,X), pere(Z,Y), mere(T,Y), homme(X).

/* SOEUR(X,Y) : X est soeur Y */
soeur(X,Y) :- pere(Z,X), mere(T,X), pere(Z,Y), mere(T,Y), femme(X).

/* OU EXCLUSIF */
X xor Y :- (X;Y),not((X,Y)).

/* DEMI_FRERE(X,Y) : X est demifrère de Y */
demi_frere(X,Y) :- ((pere(Z,X), pere(Z,Y)) xor (mere(T,X), mere(T,Y))), homme(X).

/* DEMI_SOEUR(X,Y) : X est demisoeur de Y */
demi_soeur(X,Y) :- ((pere(Z,X) ,pere(Z,Y)) xor (mere(T,X), mere(T,Y))), femme(X).

/* COUSIN : X est cousin germain ou cousine germaine de Y */
cousin(X,Y) :- grand_parent(Z,X), grand_parent(Z,Y), not(frere(X,Y)), not(soeur(X,Y)), not(demi_frere(X,Y)), not(demi_soeur(X,Y)).

/* 1)

a) homme(paul) -> true.

b) femme(benoit) -> false.

c) femme(X) -> X = germaine ; X = christiane ; X = simone ; X = marie ;
X = sophie ; X = madelaine.

d) homme(X) -> X = albert ; X = jean ; X = bertrand ; X = louis ; X =
benoit.

e) mere(marie,sophie) -> true.

f) mere(X,jean) -> X = germaine.

g) pere(paul,X) -> X = bertrand ; X = sophie.

h) pere(X) -> X = albert ; X = jean ; X = paul ; X = louis.

*/

/* 2) Voir plus haut */

/* 3) */

/* ANCETRE(X,Y) : X est ancêtre de Y */
ancetre(X,Y) :- parent(X,Y).
ancetre(X,Y) :- parent(X,Z), ancetre(Z,Y).

/* DESCENDANT(X,Y) : X est descendant de Y */
descendant(X,Y) :- parent(Y,X).
descendant(X,Y) :- parent(Z,X), descendant(Z,Y).

/* MEMEFAMILLE(X,Y) : X et Y ont un ancêtre commun */
memefamille(X,Y) :- ancetre(Z,X), ancetre(Z,Y).



/* EXERCICE B */

q(X,Z) :- p(X,Y), p(Y,Z).
p([1|Z],Z).

/*
q(U,[]) -> U = [1,1].
q(U,[1,2,3]) -> U = [1,1,1,2,3].
*/

lg([],0).
lg([_|L1],N1) :- lg(L1,N), N1 is N+1.

/*
lg([a,b,c],2) -> false.
lg([a,b,c],P) -> P = 3.
*/


/* EXERCICE C */

/* APPARTIENT(X,L) : L'élément X appartient-il à la liste L ? */
appartient(X,[X|_]).
appartient(Y,[X|L]) :- X \== Y, appartient(Y,L).

/* NON_APPARTIENT(X,L) : L'élement X n'est-il pas dans la liste L ? */
non_appartient(X,L) :- not(appartient(X,L)).

/* SANS_REPETITION(L) : La liste L ne contient pas deux fois le meme élément */
sans_repetition([]).
sans_repetition([X|L]) :- non_appartient(X,L), sans_repetition(L).

/* AJOUT_TETE(X,L1,L2) : L2 vaut-elle X concaténé à L1 ? */
ajout_tete(X,L,[X|L]).

/* AJOUT_QUEUE(X,L1,L2) : L2 vaut-elle L1 concaténée à X ? */
ajout_queue(X,[],[X]).
ajout_queue(X,[Y|L1],L2) :- ajout_tete(Y,L3,L2), ajout_queue(X,L1,L3).

/* SUPPRIMER(X,L1,L2) : L2 vaut L1 sans la première occurence de X ? */
supprimer(_,[],[]).
supprimer(X,[X|L1],L1).
supprimer(X,[Y|L1],[Z|L2]) :- X \== Y,  Y==Z, supprimer(X,L1,L2).
supprimer(X,[X],[]).

/* SUPPRIMER_FIN(L1,L2) : L2 vaut L1 sans son dernier élément ? */
supprimer_fin2([_|L],[]) :- L==[].
supprimer_fin2([X|L1],[X|L2]) :- supprimer_fin2(L1,L2).
supprimer_fin(L1,L2) :- (L1==[], L2==[]) ; supprimer_fin2(L1,L2).

/* FUSION(L1,L2,L3) : L3 est une fusion des termes en alternance de L1 et L2 ? */
fusionD(L1,[X|L2],[Y|L3]) :- X==Y, fusion(L1,L2,L3).
fusion([],L,L).
fusion(L,[],L).
fusion([X|L1],L2,[Y|L3]) :- X==Y, ((L1==[], L2==L3);(L2==[], L1==L3);(fusionD(L1,L2,L3))).

/* CONCATENER(L1,L2,L3) : L3 est une concaténation des éléments de L1 puis de L2 ? */
concatener([],[],[]).
concatener(L,[],L).
concatener([],L,L).
concatener([X|L1],L2,[Y|L3]) :- X==Y, concatener(L1,L2,L3).

/* INVERSER(L1,L2) : L1 est inversée par rapport à L2 ? */
inverser([],[]).
inverser([X|L1],L2) :- inverser(L1,L3), ajout_queue(X,L3,L2).

/* COMMUN(L1,L2,L3) : L3 est la liste qui concatene les ordre de première appartion de L1 puis de L2 ? */
commun([],[_|_],[]).
commun([X|L1],L2,L3) :- non_appartient(X,L2),commun(L1,L2,L3).
commun([X|L1],L2,L3) :- appartient(X,L2),commun(L1,L2,L4),ajout_tete(X,L4,L3).
commun([_|_],[],[]).
commun(L1,[Y|L2],L3) :-appartient(Y,L1),commun(L1,L2,L4),ajout_tete(Y,L4,L3).
commun(L1,[Y|L2],L3) :- non_appartient(Y,L1),commun(L1,L2,L3).

/* ENS(L1,L2) : L2 est obtenue à partir de L1 par suppression de toutes les occurences de sess élément sauf les dernières ? */
ens([],[]).
ens([X|L1],L2) :- appartient(X,L1),ens(L1,L2).
ens([X|L1],L2) :- non_appartient(X,L1),ens(L1,L3),ajout_tete(X,L3,L2).

/* REUNION(L1,L2,L3) : L3 est l'union des élements de L1 et L2 (L1 et L2 sont sans répétition) dans L1 suivie de L2 ? */
reunion([],L,L).
reunion(L,[],L).
reunion(L1,[Y|L2],L3) :- appartient(Y,L1), reunion(L1,L2,L3).
reunion(L1,[Y|L2],L3) :- non_appartient(Y,L1), ajout_queue(Y,L4,L3), reunion(L1,L2,L4).

/* REUNIONBIS(L1,L2,L3) : L3 est l'union des élements de L1 et L2 (L1 et L2 peuvent être avec répétition) dans L1 suivie de L2 ? */
reunionbis(L1,L2,L3) :- ens(L1,L1B), ens(L2,L2B), reunion(L1B,L2B,L3).
