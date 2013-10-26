deffn fac (args n)
    if (== n 0)
        1
        *
            n
            fac (- n 1 ) 

fac 12
