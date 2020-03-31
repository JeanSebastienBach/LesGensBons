/* EXERCICE A */

/* HOMME */
homme(albert).
homme(jean).
homme(paul).
homme(bertrand).
homme(louis).
homme(benoit).
/* Vous l'aviez oublié !!!!! */
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




