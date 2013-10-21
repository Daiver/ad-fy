not (== 10 1)
foldl (' +) 0 (list 1 2 3)
sum (list 1 2 3 4 5)
product (list 1 2 3 4 5)
map (' inc) (list 1 2 3)
filter (' inc) (list -1 -1 2)
define x (range 1 10)


deffn applyer (args f) (f 10)
deffn ident (args x) x
define func_list (list (' inc) (' dec) (' ident)) 
map (' applyer) func_list
reverse x
average (list 5 2 3 4)
