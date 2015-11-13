# The Uniform Distribution
#
#     These functions provide information about the uniform distribution
#     on the interval from ‘min’ to ‘max’. 
#     ‘runif’ generates random deviates.
#
# Usage:
#
#     runif(n, min = 0, max = 1)
#
# The Normal Distribution
#
#    Density, distribution function, quantile function and random
#    generation for the normal distribution with mean equal to ‘mean’
#    and standard deviation equal to ‘sd’.
#
# Usage:
#
#   rnorm(n, mean = 0, sd = 1)
#

genereaza_observatii <- function(m, a, b, xmin, xmax, sigma) {
    # Generează m observații pentru un model de regresie simpla folosind
    # formula: yi = a + bxi + Ei, i = 1, · · · , m.
    # Mai multe informații aici: http://goo.gl/5DbT9Z pagina 12

    # Valorile lui X sunt distribuite uniform in intervalul [xmin, xmax]
    x <- runif(m, min=xmin, max=xmax)

    # Distribuția erorilor este Ei ∼ N(0, sigma * sigma)
    e <- rnorm(m, sd=sigma)

    # yi = a + b * xi + Ei, i = 1, ...m
    y <- a + b * x + e
    
    return(list(x=x,y=y))
}

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

coeficienti_regresie <- function(observatii) {
    model_liniar <- lm(formula=observatii$y~observatii$x)
    coeficienti <- coef(model_liniar)
    intervalele_incredere <- confint(object=model_liniar,
                                     level=0.95)
    return(list(coeficienti=coeficienti, intervale=intervalele_incredere))
}

plot_regresie <- function(image_name, m, a, b, xmin, xmax, sigma) {
    pdf(image_name, width=10, height=10)
    observatii <- genereaza_observatii(m, a, b, xmin, xmax, sigma)
    data <- coeficienti_regresie(observatii)
    coeficienti <- data$coeficienti

    plot(observatii$x, observatii$y)
    abline(a, b, col="red")
    abline(coeficienti[1], coeficienti[2], col="blue")

    title(main=image_name)
    dev.off()
}

plot_regresie("1.pdf", 100, 3, 5, -200, 200, 1.5)
plot_regresie("2.pdf", 10, 3, 5, -5, 5, 1)
plot_regresie("3.pdf", 10000, 3, 5, -5, 5, 1)
plot_regresie("4.pdf", 10, 3, 5, 5, 5.2, 1)
plot_regresie("5.pdf", 10000, 3, 5, 5, 5.2, 1)
plot_regresie("6.pdf", 10, 3, 5, 5, 5.2, 0.01)
