#!/usr/bin/Rscript

library(moments)
library(ggplot2)


mode <- function(dataset, na.rm=FALSE) {
    # The mode is the most frequent score in our data set.
    if(na.rm){
        dataset <- dataset[!is.na(dataset)]
    }

    unique_values <- unique(dataset)
    counts <- vector()
    for (index in 1:length(unique_values)) {
        counts[index] <- length(which(dataset == unique_values[index]))
    }

    if(mean(counts) == max(counts)) {
        return(NA)
    }

    position <- c(which(counts == max(counts)))
    return(unique_values[position])
}


central_tendency <- function(dataset) {
    # A measure of central tendency is a single value that attempts
    # to describe a set of data by identifying the central position
    # within that set of data. As such, measures of central tendency
    # are sometimes called measures of central location.
    #
    # They are also classed as summary statistics. The mean (often
    # called the average) is most likely the measure of central
    # tendency that you are most familiar with, but there are others,
    # such as the median and the mode.
    #
    # Resources:
    #   - http://goo.gl/FaSOam
    #   - https://goo.gl/C2iG4n
    fields <- list(age=dataset$Age, class=dataset$Pclass,
                   survived=dataset$Survived)

    print("Central tendency")
    for (name in names(fields)) {
        print(paste("|---", name))
        print(paste("|   |---mean:   ", mean(fields[[name]], na.rm=TRUE)))
        print(paste("|   |---median: ", median(fields[[name]], na.rm=TRUE)))
        print(paste("|   |---mode:   ", mode(fields[[name]], na.rm=TRUE)))
    }
}


distribution <- function(dataset) {
    fields <- list(age=dataset$Age, class=dataset$Pclass,
                   survived=dataset$Survived)

    print("Distribution")
    for (name in names(fields)) {
        print(paste("|---", name))
        print(paste("|    |---skewness", skewness(fields[[name]], na.rm=TRUE)))
        print(paste("|    |---kurtosis", kurtosis(fields[[name]], na.rm=TRUE)))
    }
}


variability <- function(dataset) {
    # Variability refers to how "spread out" a group of scores is.
    fields <- list(age=dataset$Age, class=dataset$Pclass,
                   survived=dataset$Survived)

    print("Variability")
    for (name in names(fields)) {
        print(paste("|---", name))
        
        print(paste("|    |---variance          ", var(fields[[name]], na.rm=TRUE)))
        print(paste("|    |---standard deviation", sd(fields[[name]], na.rm=TRUE)))
        print(paste("|    |---range             ", diff(range(fields[[name]], na.rm=TRUE))))
    }
}


frequencies <- function(dataset) {
    # Resources:
    #   - http://www.statmethods.net/stats/frequencies.html
    fields <- list(age=dataset$Age, class=dataset$Pclass,
                   survived=dataset$Survived)

    print("Frequencies")
    for (name in names(fields)) {
        print(paste("|---", name))

        data <- table(fields[[name]])
        print(data)
    }
}


plot_data <- function(dataset) {
    fields <- list(age=dataset$Age, class=dataset$Pclass,
                   survived=dataset$Survived)

    for (name in names(fields)) {
        hist(fields[[name]], main=name)
        boxplot(fields[[name]], data=dataset, horizontal=TRUE)
    }
}


independence <- function(dataset) {
    dataset$bool_sex[dataset$Sex=="male"] <- 1
    dataset$bool_sex[dataset$Sex=="female"] <- 0 

    print ("PClass -- Survivability")
    print (paste("|    |---correlation: ", cor(dataset$Survived, dataset$Pclass)))
    print (paste("|    |---independence: ", fisher.test(table(dataset$Survived, dataset$Pclass))))
    
    print ("Sex -- Survivability: ")
    print (paste("|    |---correlation: ", cor(dataset$Survived, dataset$bool_sex)))
    print (paste("|    |---independence: ", fisher.test(table(dataset$Survived, dataset$Sex))))
      

    # The association between survivability and class
    association <- table(dataset$Pclass, dataset$Survived)
    plot(association, main="Class and survivability association")
    
    barplot_association <- rep(association)
    barplot(barplot_association,
            col = c("lightgreen", "mistyrose", "red",
                     "lavender", "cornsilk", "lightblue"),
            names.arg = c("Died-C1", "Died-C2", "Died-C3",
                          "Survived-C1", "Survived-C2", "Survived-C3"))
    title("Class and survival association")    

    # The association between survivability and sex
    association <- table(dataset$Sex, dataset$Survived)
    plot(association, main="Sex and survivability association")
        
    barplot_association <- rep(table(dataset$bool_sex, dataset$Survived))
    barplot(barplot_association,
            col = c("lightgreen", "mistyrose",
                    "lavender", "cornsilk"),
            names.arg = c("Dead Male", "Survived Male",
                          "Dead Female", "Survived Female"))
    title("Sex and survival association")
}


main <- function() {
    dataset <- read.csv(file="train.csv", header=TRUE)
    central_tendency(dataset)
    distribution(dataset)
    variability(dataset)
    frequencies(dataset)
    independence(dataset)
    plot_data(dataset)
}

main()