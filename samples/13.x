print (list 10)
define x (list 1 2 3)
print x
print 
    [] 1 x

define a (list (+ ([] 2 x) 10) 15 -1 100 0 50 9 8)
define s 
    slice 1 4 a

print
    list 1 2 3 (list 13 13 13) 5 6 7 (list -1 -2 -3) 9

define s1 (cons 999 s)
define s2 (cons 999 s1)
define s3 (cons 333 s2)
define s4 (slice 3 5 s3)
print 
    length s4
    length s3

head s3
head s4
tail s3
tail s4
init s3
init s4
last s3
last s4
tail (list)
