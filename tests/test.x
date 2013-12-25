define x 10
define y 2
define res (+ x y)
assert "simple_assertion" (== 12 res) 

define x 100
assert "redefine_value_assertion" (== x 100) 

define res
    /
        102
        +
            4
            * 2 5 3
            -3
            2
            1

assert "many_ariphmetics" (== res 3) 

define l (range 0 5)
assert "list_compare_test" (lists-compare l (list 0 1 2 3 4)) 
assert "reverse_assert" (lists-compare (reverse l) (list 4 3 2 1 0)) 
assert "reverse_assert2" (lists-compare (reverse l) (reverse (range 0 5))) 

deffn apply (args f x) (f x)
define res
    apply (\ (args x) (* x 10)) 5

assert "call_and_lambda" (== 50 res) 

define res 
    if 1
        50
        20
assert "if_assert" (== res 50) 

deffn f
    if (== 50 res)
        define res 1
        define res 2
f
assert "inner_define" (== res 50) 

; some comment
comment more comments

assert
    "map_assert"
    lists-compare 
        map (' inc) l
        list 1 2 3 4 5

define x 10
assert
    "functor_test"
    lists-compare (functor 10 (list (' dec) (' inc) (' ')))
        list 9 11 10

define res 
    foldl (' ++) list 
        list (range 0 8) (range 8 13) 
assert "foldl_test" (lists-compare res (range 0 13)) 

assert "sum_assert" (== 15 (sum (range 0 6))) 
assert "product_assert" (== 0 (product (range 0 6))) 
assert "product_assert2" (== 120 (product (range 1 6))) 
assert "foldl1_assert" (== 15 (foldl1 (' +) (range 0 6))) 

assert "mod" (== (mod 6 4) 2) 
assert "mod2" (== (mod 7 2) 1) 
assert "mod3" (== (mod 11 3) 2) 

assert
    "filter_assert"
    lists-compare
        list 0 2 4 6 8
        filter (' odd) (range 0 10)

deffn f10
    args x
    deffn g
        args y
        * x y
    g 100

assert 
    "inner_func"
    == (f10 11) 1100

deffn f20
    args x
    deffn g
        args y
        deffn k
            args z
            + z y
        * x (k 7)
    g 100

assert "inner_func2" (== 1177 (f20 11)) 

assert 
    "double_one"
    == 3.3
        + 0.1 2.0 1.2

assert "double_two" (== 3.2 (+ 1 1.0 1.2)) 
assert "double_three" (== 0.1 (- 1.1 1)) 
assert 
    "double_four"
    == 0.44
        /
            *
                2.2
                + 1 2
                    - 1 2
            10

assert "double_five" (== 1 (* 0.1 10)) 
assert "double_six" (== 10.0 (* 0.1 100)) 
assert 
    "double_seven"
    == 1024
        /
            + 1 2 3 (- 3 1)
            * .01
                / 
                    + 
                        + 8 (* 10 7)
                        * .5 .5 .5
                    100

assert "double_product" (== 0.125 (product (list .5 .5 .5))) 
assert "double_sum" (== 0.1 (sum (list .05 -.05 .09 .01))) 
assert
    "double_map"
    lists-compare
        list .1 .2 .3
        map (\ (args x) (* x 0.1)) (list 1 2 3)

define s_some "some"
define s_string "string"

assert 
    "string_test1"
    lists-compare
        ++
            ++ s_some " "
            s_string
        "some string"

assert "string_length" (== 3 (length "321")) 

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
    "string_map"
    str_list_compare
        list "Hi!" "Hello!" "Bonjour!"
        map (\ (args x) (++ x "!")) (list "Hi" "Hello" "Bonjour")

assertEq "is_exception test" NTYPE_EXCEPTION (type (+ 10 "0" 100))

; assert "PASSED BAD" 0
assertEq
    "Wrong num assert"
    NTYPE_EXCEPTION
    type (assert 1)

print 
    + 
        10
        - 
            1
            * ""
rgegergergeg
