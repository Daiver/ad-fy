define m_x 10
deffn inc
    args m_x
    + 1 m_x
deffn dec (args x) (- x 1)
deffn . (args f g x) 
    f (g x)


. (' inc) (' dec) m_x

