datatype expr = NUM of int
              | PLUS of expr * expr
              | MINUS of expr * expr

datatype formula = TRUE
                 | FALSE
                 | NOT of formula
                 | ANDALSO of formula * formula
                 | ORELSE of formula * formula
                 | IMPLY of formula * formula
                 | LESS of expr * expr

type name = string

datatype metro = STATION of name
               | AREA of name * metro
               | CONNECT of metro * metro

datatype 'a lazyList = nullList
                     | cons of 'a * (unit -> 'a lazyList)

fun eval f =
    case f of
      TRUE => true
    | FALSE => false
    | NOT e => not (eval e)
    | ANDALSO (e1, e2) => (eval e1) andalso (eval e2)
    | ORELSE (e1, e2) => (eval e1) orelse (eval e2)
    | IMPLY (e1, e2) => ((eval e1) andalso (eval e2)) orelse (not (eval e1))
    | LESS (e1, e2) =>   let fun expreval ex =
                          case ex of
                            NUM e => e
                          | PLUS (e1, e2) => expreval(e1) + expreval(e2)
                          | MINUS (e1, e2) => expreval(e1) - expreval(e2)
                          in expreval(e1) < expreval(e2) end

fun checkMetro met =
    case met of
      STATION n => false
    | AREA (n, m) => let fun checkName (nn, mm) =
                    if nn = []
                    then false
                    else
                    case mm of
                      STATION n1 => (n1 = hd(nn)) orelse (checkName(tl(nn), mm))
                    | AREA (n1,m1) => checkName([n1] @ nn, m1)
                    | CONNECT (m1, m2) => (checkName(nn, m1)) andalso (checkName(nn, m2))
                    in checkName ([n], m) end
    | CONNECT (n, m) => (checkMetro n) andalso (checkMetro m)

fun seq(first : int, last : int) =
    if first > last
    then nullList
    else cons (first, fn() => seq(first+1, last))

fun infSeq(first : int) = cons (first, fn () => infSeq(first+1))

fun firstN(lazyListVal, n) =
  if n = 0
  then []
  else
    case lazyListVal of
      nullList => []
    | cons(a, f) => [a] @ firstN(f(), n-1)

fun Nth(lazyListVal, n) =
  case lazyListVal of
    nullList => NONE
  | cons(a, f) => if n = 1 then SOME a
                  else Nth(f(), n-1)

fun filterMultiples (lazyListVal, n) =
  case lazyListVal of
    nullList => nullList
  | cons(a, f) => if a mod n = 0
                  then filterMultiples(f(), n)
                  else cons(a, fn() => filterMultiples(f(), n))

fun primes() =
  let fun seive(lazyListVal) =
    case lazyListVal of
      nullList => nullList
    | cons(a, f) => cons(a, fn() => seive(filterMultiples(f(), a) ) )
  in seive(infSeq(2)) end
