define x 10
define y 2
define res (+ x y)
assert (== 12 res) simple_assertion

define x 100
assert (== x 100) redefine_value_assertion

define res
    /
        102
        +
            4
            * 2 5 3
            -3
            2
            1

assert (== res 3) many_ariphmetics

define l (range 0 5)
assert (lists-compare l (list 0 1 2 3 4)) list_compare_test
assert (lists-compare (reverse l) (list 4 3 2 1 0)) reverse_assert
assert (lists-compare (reverse l) (reverse (range 0 5))) reverse_assert2

deffn apply (args f x) (f x)
define res
    apply (\ (args x) (* x 10)) 5

assert (== 50 res) call_and_lambda

define res 
    if 1
        50
        20
assert (== res 50) if_assert

deffn f
    if (== 50 res)
        define res 1
        define res 2
f
assert (== res 50) inner_define

; some comment
comment more comments

assert
    lists-compare 
        map (' inc) l
        list 1 2 3 4 5
    map_assert

define x 10
assert
    lists-compare (functor 10 (list (' dec) (' inc) (' ')))
        list 9 11 10
    functor_test

define res 
    foldl (' ++) list 
        list (range 0 8) (range 8 13) 
assert (lists-compare res (range 0 13)) foldl_test

assert (== 15 (sum (range 0 6))) sum_assert
assert (== 0 (product (range 0 6))) product_assert
assert (== 120 (product (range 1 6))) product_assert2
assert (== 15 (foldl1 (' +) (range 0 6))) foldl1_assert
