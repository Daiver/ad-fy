deffn vector_by_vector (args v1 v2)
    if (empty v1)
        0
        +
            * (head v1) (head v2)
            vector_by_vector (tail v1) (tail v2)

deffn vector_by_matrix (args mat vec)
    map 
        \ (args vec1)
            vector_by_vector vec vec1
        mat

deffn matrix_by_matrix (args mat1 mat2)
    trans
        map
            \ (args vct1)
                vector_by_matrix mat1 vct1
            trans mat2

deffn trans (args mtrx)
    if (empty ([] 0 mtrx))
        list
        ++ 
            list 
                map (' head) mtrx
            trans
                map (' tail) mtrx

define m 
    list
        list 1 2 3
        list 4 5 6

define m2
    list
        list 10 11
        list 12 13
        list 14 15

define vctr
    list 10 20 30

print m
trans m
vector_by_vector ([] 0 m) ([] 1 m)
vector_by_matrix m vctr
matrix_by_matrix m m2
