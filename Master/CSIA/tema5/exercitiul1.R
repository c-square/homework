# 1. Să se determine cel mai bun (sub)model de regresie
# folosind metodele *forward*, *backward* și *stepwise*
# selection.

housedata <- read.table(file="house.dat", header=TRUE)
null <- lm(PRICE~1, data=housedata)
full <- lm(PRICE~., data=housedata)

# forward selection
forward_selection <- step(full,
						  scope=list(lower=null, upper=full),
						  data=housedata,
						  direction="forward",
						  trace="F")

# backward selection
backward_selection <- step(full,
						   scope=list(lower=null, upper=full),
						   data=housedata,
						   direction="backward",
						   trace="F")


# stepwise selection
stepwise_selection <- step(full,
						   scope=list(lower=null, upper=full),
						   data=housedata,
						   direction="both",
						   trace="F")

# Computes the (generalized) Akaike *A*n *I*nformation 
# *C*riterion for a fitted parametric model.
fs_aic <- extractAIC(forward_selection)
bs_aic <- extractAIC(backward_selection)
ss_aic <- extractAIC(stepwise_selection)

print("Model de regresie: Forward")
print(forward_selection)
print("Model de regresie: Backward")
print(backward_selection)
print("Model de regresie: Stepwise")
print(stepwise_selection)

print("Model de regresie: Forward")
print(fs_aic)
print("Model de regresie: Backward")
print(bs_aic)
print("Model de regresie: Stepwise")
print(ss_aic)
