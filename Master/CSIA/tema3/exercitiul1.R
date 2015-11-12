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
    x <- runif(m, mean=0, min=xmin, max=xmax)

    # Distribuția erorilor este Ei ∼ N(0, sigma * sigma)
    e <- rnorm(m, mean=0, sd=sigma * sigma)

    # yi = a + b * xi + Ei, i = 1, ...m
    y <- a + b * x + e
    
    return(y)
}


print(genereaza_observatii(100, 3, 5, -200, 200, 1.5))
