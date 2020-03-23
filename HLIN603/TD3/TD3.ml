let print_num_exo i j = print_string "\n"; print_int i; print_string "."; print_int j; print_string ")\n";;

let rec print_bool = function
| true -> print_string "vrai\n"
| false -> print_string "faux\n";;

let eval x y = print_string("eval("); print_string(x); print_string(") = "); print_bool(y);;

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

(* 1.1 *)
print_num_exo 1 1;;
print_string "natural : min -> bool\n";;
print_string "negative : #min -> bool\n";;
print_string "positive : other -> bool\n";;

(* 1.2 *)
print_num_exo 1 2;;
eval "natural m" (natural m);;
print_string("eval(natural mm) = Impossible car natural attend un type min, or mm est de type min_max et a la méthode max en plus\n");;
print_string("eval(natural o) = Impossible car natural attend un type min, or o est de type other et a la méthode max en plus\n");;
eval "natural (o:>min)" (natural (o:>min));;
print_string("eval(natural c) = Impossible car natural attend un type min, or c est de type cell et a la méthode set en plus\n");;
eval "negative m" (negative m);;
eval "negative mm" (negative mm);;
eval "negative o" (negative o);;
print_string("eval(negative c) = Impossible car negative attend un type min, or c est de type cell et n'a pas de méthode min\n");;
eval "positive m" (positive m);;
eval "positive mm" (positive mm);;
eval "positive o" (positive o);;
eval "positive c" (positive c);;

(* 2.1 *)
print_num_exo 2 1;;

class virtual ['a] add_magma =
object
	method virtual add : 'a -> 'a -> 'a
end;;

class virtual ['a] mul_magma =
object
	method virtual mul : 'a -> 'a -> 'a
end;;


(* 2.2 *)
print_num_exo 2 2;;

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


(* 2.3 *)
print_num_exo 2 3;;

class virtual ['a] add_group =
object
	inherit ['a] add_monoid
	method virtual inverse_id : 'a
end;; 


(* 2.4 *)
print_num_exo 2 4;;

class virtual ['a] ring =
object
	inherit ['a] add_group
	inherit ['a] mul_monoid
end;;


(* 2.5 *)
print_num_exo 2 5;;

class virtual int_ring =
object
	inherit [int] add_group
	inherit [int] mul_monoid
end;;


(* 2.6 *)
print_num_exo 2 6;;

class virtual ['a, 'b] polynomial (r : 'b) (p : ('a * int) list) =
object
	
end;;




