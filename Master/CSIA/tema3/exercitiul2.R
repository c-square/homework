# Fitting Linear Models
# ---------------------
# ‘lm’ is used to fit linear models.  It can be used to carry out
# regression, single stratum analysis of variance and analysis of
# covariance (although ‘aov’ may provide a more convenient interface
# for these).
#
# Usage
# -----
# lm(formula, data, subset, weights, na.action,
# method = "qr", model = TRUE, x = FALSE, y = FALSE, qr = TRUE,
# singular.ok = TRUE, contrasts = NULL, offset, ...)
#
# Confidence Intervals for Model Parameters
# -----------------------------------------
# Computes confidence intervals for one or more parameters in a
# fitted model.  There is a default and a method for objects
# inheriting from class ‘"lm"’.
#
# Usage
# -----
# confint(object, parm, level = 0.95, ...)
#

genereaza_observatii <- function(m, a, b, xmin, xmax, sigma) {
    x <- runif(m, xmin, xmax)
    e <- rnorm(m, sigma)
    y <- a + b * x + e
    return(list(x=x,y=y))
}

coeficienti_regresie <- function(observatii) {
    model_liniar <- lm(formula=observatii$y~observatii$x)
    coeficienti <- coef(model_liniar)
    intervalele_incredere <- confint(object=model_liniar,
                                     level=0.95)
    return(list(coeficienti=coeficienti, intervale=intervalele_incredere))
}

coeficienti_regresie(genereaza_observatii(100, 3, 5, -200, 200, 1.5))
