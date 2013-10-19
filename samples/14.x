deffn f (args n)
    if (== n 0)
        0
        + 1 
            f (- n 1)
        
f 1500
