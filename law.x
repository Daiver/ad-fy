deffn transpose (args m)
    print m
    print ([] 0 m)
    ; if (== 0 (length ([] 0 m)))
        list
        cons 
            map (' last) m
            transpose 
                map (' init) m

transpose
    list
        list 1 2 3
        list 4 5 6

