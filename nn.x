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

deffn activate
    args mtrx sample
    mul_matrix_by_matrix sample mtrx

deffn train
    args mtrx sample label
    define tmp_mtrx
        [] label
            transpose mtrx
    print 
        map2 
            \ (args w x)
                + w (* 0.5 (- x w))
            tmp_mtrx
            sample

define mat
    list 
        list 0 0
        list 0 0
        list 0 0

train mat (list 1 0 1) 1

