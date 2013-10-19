comment aliases
alias ; comment
alias \ lambda
alias ' id

; numeric work
deffn inc (args x) (+ x 1)
deffn dec (args x) (- x 1)

; lists
deffn head (args li) ([] 0 li)
deffn tail (args li) (slice 1 (length li) li)
deffn last (args li) ([] (dec (length li)) li)
deffn init (args li) (slice 0 (dec (length li)) li)

deffn foldl 
    args f initial li
    if (== 0 (length li))
        initial
        foldl (' f )
            f initial (head li)
            tail li

deffn map_tmp
    args f li res
    if (== 0 (length li))
        res
        map_tmp 
            ' f
            tail li
            cons (f (head li)) res

deffn map
    args f li
    map_tmp (' f) li list

deffn sum (args li) (foldl (' +) 0 li)
deffn product (args li) (foldl (' *) 1 li)
