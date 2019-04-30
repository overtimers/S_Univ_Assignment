datatype pattern = Wildcard | Variable of string | UnitP
                 | ConstP of int | TupleP of pattern list
                 | ConstructorP of string * pattern

datatype valu = Const of int | Unit | Tuple of valu list
              | Constructor of string * valu

(* foldl f init list = f(xn f(... f(x1 init))) *)
(* List.exists f l = (f x = true) when x in l *)
fun check_pat p =
    let fun make_list x =
          case x of Variable a => [a]
          | TupleP l => List.foldl (fn (p, acc)=> acc @ make_list(p) ) [] l
          | _ => []
        fun check_repeat xs =
          case xs of [] => false
         | x::xs' => List.exists (fn a => x = a) xs' orelse check_repeat xs'
    in not (check_repeat (make_list p))
    end

fun match (v, p) =
  case (v, p) of
    (_, Wildcard) => SOME []
  | (_, Variable s) => SOME [(s, v)]
  | (Unit, UnitP) => SOME []
  | (Const c, ConstP cp) => if c = cp then SOME [] else NONE
  | (Tuple vt, TupleP pt)=> if length vt <> length pt
                            then NONE
                            else
                              let
                                  fun loop (acc, xs) =
                                    case xs of [] => SOME acc
                                    | SOME(x)::xs' => loop(acc@x, xs')
                                    | NONE::xs' => NONE
                              in loop ([], map match (ListPair.zip(vt,pt)))
                              end
  | _ => NONE

type name = string
datatype RSP = ROCK | SCISSORS | PAPER
datatype 'a strategy = Cons of 'a * (unit -> 'a strategy)
datatype tournament =
    PLAYER of name * (RSP strategy ref)
  | MATCH of tournament * tournament

fun onlyOne(one:RSP) = Cons(one, fn() => onlyOne(one))
fun alterTwo(one: RSP, two:RSP) = Cons(one, fn() => alterTwo(two, one))
fun alterThree(one:RSP, two:RSP, three:RSP) =
  Cons(one, fn()=> alterThree(two, three, one))

val r = onlyOne(ROCK)
val s = onlyOne(SCISSORS)
val p = onlyOne(PAPER)
val rp = alterTwo(ROCK, PAPER)
val sr = alterTwo(SCISSORS, ROCK)
val ps = alterTwo(PAPER, SCISSORS)
val srp = alterThree(SCISSORS, ROCK, PAPER)

fun next(strategyRef) =
  let val Cons(rsp, func) = !strategyRef
  in
    strategyRef := func();
    rsp
  end

fun wins (s1, s2) =
  let val rsp1 = next(s1)
      val rsp2 = next(s2)
  in
    if (rsp1 = ROCK andalso rsp2 = SCISSORS)
      orelse (rsp1 = SCISSORS andalso rsp2 = PAPER)
      orelse (rsp1 = PAPER andalso rsp2 = ROCK)
    then true
    else if (rsp2 = ROCK andalso rsp1 = SCISSORS)
      orelse (rsp2 = SCISSORS andalso rsp1 = PAPER)
      orelse (rsp2 = PAPER andalso rsp1 = ROCK)
    then false
    else wins(s1, s2)
  end

fun whosWinner(t) =
  case t of
    MATCH (PLAYER(p1,s1), PLAYER(p2,s2)) =>
          if wins(s1, s2)
          then PLAYER(p1,s1)
          else PLAYER(p2,s2)
  | MATCH (MATCH m1, PLAYER p1) =>
      whosWinner( MATCH (whosWinner(MATCH m1), PLAYER p1))
  | MATCH (PLAYER p1, MATCH m1) =>
      whosWinner( MATCH (PLAYER p1, whosWinner(MATCH m1)))
  | MATCH (MATCH m1, MATCH m2) =>
      whosWinner( MATCH (whosWinner(MATCH m1), whosWinner(MATCH m2)))
  | PLAYER p => PLAYER p

val winner = whosWinner(MATCH(PLAYER("s", ref s),
  MATCH(PLAYER("rp", ref rp), PLAYER("r", ref r))));
