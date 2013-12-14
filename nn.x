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
    list 
        list 0 0
        list 0 0
        list 0 0

print (reverse (replace_row (reverse mat) 0 (list 1 2)))

;
    define mat (train mat (list 1 0 0) 0)
    define mat (train mat (list 2 1 0) 0)
    define mat (train mat (list 0 1 1) 1)
    define mat (train mat (list 0 1 0) 1)

define mat
    train_on_samples mat
        list
            list 0 (list 1 0 0)
            list 0 (list 2 1 0)
            list 1 (list 0 1 1)
            list 1 (list 0 1 0)

print mat
print (activate mat (list (list 3 1 0)))
print (activate mat (list (list 0 1 5)))
print (activate mat (list (list 0 1 0)))

