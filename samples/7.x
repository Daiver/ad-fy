deffn square
    args m_x
    * m_x m_x

deffn example 
    args f arg
    f arg

square 100
example (' square) 2
example (id square) 2
