deffn f_main
	args xxx
	deffn f_i (args y) (+ xxx y)
	' f_i

deffn apply
	args f i
	f i

apply (f_main 13) 100	 
