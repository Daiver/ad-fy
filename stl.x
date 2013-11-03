comment aliases
alias ; comment
alias \ lambda
alias ' id

; numeric work
deffn inc (args x) (+ x 1)
deffn dec (args x) (- x 1)
deffn not (args x) (if (== 0 x) 1 0)

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
        

deffn call (args some_lambda) some_lambda

deffn while (args while_predicate while_body)
    if (while_predicate)
        0
        call (\ while_body (while (' while_predicate) (' while_body)))
