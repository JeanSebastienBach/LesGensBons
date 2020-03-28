

print_endline("Exercice 1");;
print_newline();;

class min (init : int) = 
object (self)
	val x = init
	method get = x
	method min y = if (self#get<y) then self#get else y
end;;

class min_max (init : int) =
object (self)
	inherit min init
	method max y = if (self#min y=y)then self#get else y
end;;

class other =
object
	method get = 1
	method min n = n-1
	method max n = n+1
end;;

class ['a] cell (init : 'a) =
object
	val mutable cont = init
	method get = cont
	method set n = cont <- n
end;;

let m = new min 1;;
let mm = new min_max 2;;
let o = new other;;
let c = new cell 1;;

let natural (o: min) = (o#min(0)=0);;
let negative (o: #min) = (o#min(0)=o#get);;
let positive o = (o#get>0);;


(* Question 1 *)
print_string "natural : min type : bool\n";;
print_string "negative : #min type : bool\n";;
print_string "positive : other type : bool\n";;


(* Question 1 *)

print_endline("
natural m;; (* Marche *)
(*natural mm;;*) (*Pas de méthode max dans min_max + problème de type  *)
(*natural o;;*) (*Pas de méthode max dans min_max + problème de type  *)
natural (o:>min);; (* Marche *)
(*natural c;;*) (* Pas de méthode set et c est de type int cell pas min*)
negative m;; (* Marche *)
negative mm;; (* Marche *)
negative o ;; (* Marche *)
(*negative c ;;*) (* Mauvais type int cell et pas de méthode min *)
positive m;; (* Marche *)
positive mm;; (* Marche *)
positive o ;; (* Marche *)
positive c ;; (* Marche *)");;
print_newline();;


(* Exercice 2 *)

(* Question 1 *)

 
class virtual ['a] add_magma =
object
  method virtual add : 'a -> 'a -> 'a
end;;

class virtual ['a] mul_magma =
object
  method virtual mul : 'a -> 'a -> 'a
end;;

class virtual ['a] add_monoid =
object 
	inherit ['a] add_magma 
	method virtual add_id : 'a
end;;

class virtual ['a] mul_monoid =
object 
	inherit ['a] mul_magma 
	method virtual mul_id : 'a
end;;

(* Question 2 *)
class virtual ['a] add_group =
object 
	inherit ['a] add_monoid 
	method virtual add_inv : 'a -> 'a
end;;


(* Question 3 *)

class virtual ['a] ring = 
object 
	inherit ['a] add_group 
	inherit ['a] mul_monoid
end;;

 

(*pensez à montrer aux autres pas sur virtual ou non*)
class int_ring = 
object	
	inherit [int] ring 
	method add_inv x = -(x)
	method add_id = 0
	method add x y = x + y 
	method mul_id = 1 
	method mul x y = x * y 
end;;


class virtual ['a,'b] polynomial (r:'b) (p: ('a * int ) list ) =
object 
	inherit ['a] ring 
	method virtual eval : 'a -> 'a 
end;;

let rec puis x n = 
	if n = 0 
	then 1 
	else (if n = 1 
				then x 
				else x * ( puis x (n-1) )) ;;


(*Test instance d'anneaux de type int*)

let s = new int_ring ;;

class int_polynomial (p: (int * int ) list ) = 
object (self) 
	inherit [int,int_ring] polynomial s p
	inherit int_ring 
	method eval x = List.fold_right (fun (a,b) c -> s#add ( s#mul a (puis x b) ) c ) p s#add_id
end;; 

(*Test du poly *)

print_endline("Création du poly : 
let poly = new int_polynomial [(0,1);(1,2);(2,3)] ;;");;
let poly = new int_polynomial [(0,1);(1,2);(2,3)] ;;

(*Affichage du résultat*)

print_endline("Résultat de poly(8) :");;
print_int(poly#eval (8)) ;;