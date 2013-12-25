
deffn mul_vector_by_vector (args v1 v2)
    if (empty v1)
        0
        +
            * (head v1) (head v2)
            mul_vector_by_vector (tail v1) (tail v2)

deffn mul_vector_by_matrix (args mat vec)
    map 
        \ (args vec1)
            mul_vector_by_vector vec vec1
        mat

deffn mul_matrix_by_matrix (args mat1 mat2)
    transpose
        map
            \ (args vct1)
                mul_vector_by_matrix mat1 vct1
            transpose mat2

deffn transpose (args mtrx)
    if (empty ([] 0 mtrx))
        list
        ++ 
            list 
                map (' head) mtrx
            transpose
                map (' tail) mtrx

deffn map2 
    args func li1 li2 
    if (empty li1)
        list
        cons (func (head li1) (head li2)) (map2 (' func) (tail li1) (tail li2))

deffn replace_row 
    args mtrx n row
    if (empty mtrx)
        list
        cons 
            if (== 0 n)
                row
                head mtrx
            replace_row (tail mtrx) (- n 1) row

deffn activate
    args mtrx sample
    mul_matrix_by_matrix sample mtrx

deffn train
    args mtrx sample label
    define tmp_mtrx
        [] label
            transpose mtrx
    transpose
        reverse 
            replace_row  
                transpose mtrx 
                label
                reverse 
                    map2 
                        \ (args w x)
                            + w (* 0.5 (- x w))
                        tmp_mtrx
                        sample

deffn train_on_samples
    args mat samples
    foldl
        \ (args mt smpl)
            train mt (last smpl) (head smpl)
        mat
        samples

define mat
    map 
        \ (args x) (list  0.0 0.0 0.0)
        range 0 66

print mat

; define mat 
;    list 
        list 0 0 0
        list 0 0 0
        list 0 0 0

print "sam"

define samples
    list
        list 0 (loadbmp "pics/16a1.bmp")
        list 0 (loadbmp "pics/16a2.bmp")
        list 0 (loadbmp "pics/16a3.bmp")
        list 1 (loadbmp "pics/16b1.bmp")
        list 1 (loadbmp "pics/16b2.bmp")
        list 1 (loadbmp "pics/16b3.bmp")
        list 1 (loadbmp "pics/16b4.bmp")
        list 2 (loadbmp "pics/16c1.bmp")
        list 2 (loadbmp "pics/16c2.bmp")
        list 2 (loadbmp "pics/16c3.bmp")
        list 2 (loadbmp "pics/16c4.bmp")


define mat
    train_on_samples mat
        samples

print "fil"
print  
    length
        filter
            \ (args x) (not (== 0 x))
            loadbmp "pics/16a-res.bmp"

print "Fin Mat"
print mat
print "Test 16a-res.bmp"
print (activate mat (list (loadbmp "pics/16a-res.bmp")))
print "Test 16b-res.bmp"
print (activate mat (list (loadbmp "pics/16b-res.bmp")))
print "Test 16c-res.bmp"
print (activate mat (list (loadbmp "pics/16c-res.bmp")))

; print (activate mat (list (list 3 1 0)))
; print (activate mat (list (list 0 1 5)))
; print (activate mat (list (list 0 1 0)))

