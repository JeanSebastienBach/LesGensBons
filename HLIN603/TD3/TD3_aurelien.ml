class min(init : int) =
    object (self)
        val x = init
        method get=x
        method min y = if self#get<y then self#get else y
    end;;

class min_max (init : int) =
    object (self)
        inherit min init
        method max y = if self#min y =y then self#get else y
    end;;


class other =
    object
        method get = 1
        method min n = n - 1
        method max n = n+1
    end ;;

class ['a] cell( init : 'a ) =
    object
        val mutable cont = init
        method get = cont
        method set n = cont<-n
    end ;;

let m = new min 1;;
let mm= new min_max 2;;
let o = new other;;
let c = new cell 1;;


let natural (o: min) = o#min(0) = 0;;
let negative (o: #min) =o#min(0) = o#get;;
let positive o = o#get > 0;;

(*Exercice 1*)

(* 1)
 * natural : val natural : min -> bool = <fun>
 * negative: val negative : #min -> bool = <fun>
 * positive: val positive : < get : int; .. > -> bool = <fun>
 *)

(*2*)
print_endline (string_of_bool (natural m ));;            (*true*)
(*natural mm;;*)    (*Impossible car natural attends un objet de type min, or mm
 est un min_max qui a des méthodes differentes donc != types*)
(*natural o ;;*)    (*Idem*)
print_endline (string_of_bool (natural ( o:>min )));;    (*false*)
(*natural c ;;*)    (*Idem*)
print_endline (string_of_bool (negative m ));;           (*false*)
print_endline (string_of_bool (negative mm));;           (*false*)
print_endline (string_of_bool (negative o ));;           (*false*)
(*negative c ;;*)   (*Impossible car negative attends un sous-type de min or
c est de type cell, qui n'est pas un sous-type de min*)
print_endline (string_of_bool (positive m ));;           (*true*)
print_endline (string_of_bool (positive mm));;           (*true*)
print_endline (string_of_bool (positive o ));;           (*true*)
print_endline (string_of_bool (positive c ));;           (*true*)

(*Exercice 2*)
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

class virtual ['a] add_group =
    object (self)
        inherit ['a] add_monoid
        method virtual inv : 'a->'a
    end;;

class virtual ['a] ring=
    object (self)
        inherit ['a] add_group
        inherit ['a] mul_monoid
    end;;

class int_ring  =
    object (self)
        inherit [int] ring
        method add_id = 0
        method mul_id = 1
        method add (e : int) (r : int)= e+r
        method mul (e : int) (r : int)= e*r
        method inv (r : int) = (-r)
    end;;

print_endline("Exercice 2");;
print_endline("Test du int_ring");;
print_endline("Creation d'un int_ring");;
let ir=new int_ring;;print_newline();;
print_string("Test de la methode add_id: ");;
print_int(ir#add_id);;print_newline();;
print_string("Test de la methode mul_id: ");;
print_int(ir#mul_id);;print_newline();;
print_string("Test de la methode add 1 22: ");;
print_int(ir#add 1 22);;print_newline();;
print_string("Test de la methode inv 5: ");;
print_int(ir#inv 5);;print_newline();;

class virtual ['a, 'b] polynomial (r: 'b) (p: ('a * int) list)=
    object (self)
        method virtual eval : 'a -> 'a
        method virtual print: unit
    end;;

class int_polynomial (p : (int * int) list) =
    object (self)
        inherit int_ring
        inherit [int,int] polynomial (0) p
        method eval (v : int) =
            List.fold_right
            (fun (a,b)->
               (self#add (self#mul a (int_of_float(float_of_int(v) ** float_of_int(b)))))

            )
            p self#add_id
        method print =
          List.iter(fun (a,b) ->
            if b=0 then
              print_endline("" ^ (if a != 1 then string_of_int(a)
                                  else "") ^ (if b=0 then
                                                ""
                                              else
                                                if b=1 then
                                                  "x"
                                                else
                                                  "x^") ^ (if b = 0 || b=1 then ""
                                                         else string_of_int(b) ^ ""))
            else
              print_string("(" ^ (if a != 1 then string_of_int(a)
                                  else "") ^ (if b=0 then
                                                ""
                                              else
                                                if b=1 then
                                                  "x"
                                                else
                                                  "x^") ^ (if b = 0 || b=1 then ")+"
                                                         else string_of_int(b) ^ ")+"))
              ) p
   end;;

print_endline("Test du int_polynomial");;
print_endline("Creation du polynome x^2-5x+6");;
let p = [(1,2); (-5,1); (6,0)];;
let ip=new int_polynomial(p);;
print_string("Evaluation avec la valeur -3: ");;
print_int(ip#eval(-3));;
print_newline();;
print_string("Utilisation de la méthode print pour afficher le polynome: ");;
ip#print;;
