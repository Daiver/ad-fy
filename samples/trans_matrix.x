

deffn trans (args mtrx)
    if (empty ([] 0 mtrx))
        list
        ++ 
            list 
                map (' head) mtrx
            trans
                map (' tail) mtrx

define m 
    list
        list 1 2 3
        list 4 5 6

print m
trans m
