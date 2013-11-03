define x 10
define y 2
define res (+ x y)
assert (== 12 res) simple_assertion

define x 100
print x
assert (== x 100) redefine_value_assertion
