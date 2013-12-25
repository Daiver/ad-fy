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

assert (== (mod 6 4) 2) mod
assert (== (mod 7 2) 1) mod2
assert (== (mod 11 3) 2) mod3

assert
    lists-compare
        list 0 2 4 6 8
        filter (' odd) (range 0 10)
    filter_assert

deffn f10
    args x
    deffn g
        args y
        * x y
    g 100

assert 
    == (f10 11) 1100
    inner_func

deffn f20
    args x
    deffn g
        args y
        deffn k
            args z
            + z y
        * x (k 7)
    g 100

assert (== 1177 (f20 11)) inner_func2

assert 
    == 3.3
        + 0.1 2.0 1.2
    double_one

assert (== 3.2 (+ 1 1.0 1.2)) double_two
assert (== 0.1 (- 1.1 1)) double_three
assert 
    == 0.44
        /
            *
                2.2
                + 1 2
                    - 1 2
            10
    double_four

assert (== 1 (* 0.1 10)) double_five
assert (== 10.0 (* 0.1 100)) double_six
assert 
    == 1024
        /
            + 1 2 3 (- 3 1)
            * .01
                / 
                    + 
                        + 8 (* 10 7)
                        * .5 .5 .5
                    100
    double_seven

assert (== 0.125 (product (list .5 .5 .5))) double_product
assert (== 0.1 (sum (list .05 -.05 .09 .01))) double_sum
assert
    lists-compare
        list .1 .2 .3
        map (\ (args x) (* x 0.1)) (list 1 2 3)
    double_map

define s_some "some"
define s_string "string"

assert 
    lists-compare
        ++
            ++ s_some " "
            s_string
        "some string"
    string_test1

assert (== 3 (length "321")) string_length

deffn map2 
    args func li1 li2 
    if (empty li1)
        list
        cons (func (head li1) (head li2)) (map2 (' func) (tail li1) (tail li2))

deffn str_list_compare 
    args s1 s2
    foldl1
        \ (args l r)
            if l
                if r
                    1
                0
        map2 (' lists-compare) s1 s2

assert
    str_list_compare
        list "Hi!" "Hello!" "Bonjour!"
        map (\ (args x) (++ x "!")) (list "Hi" "Hello" "Bonjour")
    string_map

print (+ 10 "0" 100)
