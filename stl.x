comment aliases
alias ; comment
alias \ lambda
alias ' id

; types defs
define NTYPE_NONE 0
define NTYPE_BUILTIN_FUNC 1
define NTYPE_NODE 2
define NTYPE_FUNC 3
define NTYPE_EXCEPTION 66
define NTYPE_INT 101
define NTYPE_DOUBLE 102
define NTYPE_CHAR 104
define NTYPE_BOOL 105
define NTYPE_LIST 110
define NTYPE_STRING 111


; numeric work
deffn inc (args x) (+ x 1)
deffn dec (args x) (- x 1)
deffn not (args x) (if (== 0 x) 1 0)
deffn mod (args x y) (- x (* y (/ x y)))
deffn odd (args x) (not (mod x 2))

; lists
deffn head (args li) ([] 0 li)
deffn tail (args li) (slice 1 (length li) li)
deffn last (args li) ([] (dec (length li)) li)
deffn init (args li) (slice 0 (dec (length li)) li)

deffn empty (args li) (== 0 (length li))

deffn foldl 
    args foldl_function initial li
    if (== 0 (length li))
        initial
        foldl (' foldl_function )
            foldl_function initial (head li)
            tail li

deffn foldl1
    args foldl1_func li
    foldl (' foldl1_func) (head li) (tail li)

deffn map
    args f li
    deffn map_tmp
        args f li res
        if (== 0 (length li))
            res
            map_tmp 
                ' f
                tail li
                cons (f (head li)) res
    map_tmp (' f) li list

deffn filter 
    args foo li
    deffn inner_foo
        args l x
        if (foo x)
            cons x l
            l
    foldl (' inner_foo) list li

deffn range_tmp (args start_range end_range arr)
    if (== 0 (- start_range end_range))
        arr
        range_tmp
            inc start_range
            end_range
            cons start_range arr
deffn range (args st_range e_range) (range_tmp st_range e_range list)

deffn sum (args li) (foldl (' +) 0 li)
deffn product (args li) (foldl (' *) 1 li)

deffn reverse (args some_list)
    deffn reverse_tmp (args sl ls)
        if (== 0 (length ls))
            sl
            reverse_tmp (cons (last ls) sl) (init ls)
    reverse_tmp list some_list

deffn average (args li) (/ (sum li) (length li))

deffn ++ (args l1 l2)
    if (empty l2)
        l1
        ++
            cons (head l2) l1
            tail l2

deffn lists-compare (args l1 l2)
    deffn lists-values-compare (args l1 l2)
        if (empty l1)
            1
            if (== (head l1) (head l2))
                lists-values-compare (tail l1) (tail l2)
                0

    if (not (== (length l1) (length l2)))
        0
        lists-values-compare l1 l2
        
define functor
    lambda (args value seq)
        map 
            \ (args f)
                f value
            seq

deffn call (args some_lambda) some_lambda

comment
    unworks, because we cannot change upper context 
    deffn while (args while_predicate while_body)
    if (while_predicate)
        0
        call (\ while_body (while (' while_predicate) (' while_body)))

;   unworks
    deffn isException (args x) (== (type x) NTYPE_EXCEPTION )

deffn assert (args assertion_name assertion_value)
    if 
        assertion_value
        0
        print "FAILED            " assertion_name 

deffn assertEq (args assert_name value1 value2) (assert assert_name (== value2 value1) )

deffn tryOr 
    args x value
    ; print x
    ; print "Type" (type x)
    0

