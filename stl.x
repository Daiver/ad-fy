comment aliases
alias ; comment
alias \ lambda
alias ' id

; numeric work
deffn inc (args x) (+ x 1)
deffn dec (args x) (- x 1)

; lists
deffn head (args li) ([] 0 li)
deffn tail (args li) (slice 1 (length li) li)
deffn last (args li) ([] (dec (length li)) li)
deffn init (args li) (slice 0 (dec (length li)) li)
