
deffn my_map
    args f seq
    foldl 
        \ (args l x)
            cons (f x) l
        list
        seq

my_map (' inc) (list 1 2 3)
