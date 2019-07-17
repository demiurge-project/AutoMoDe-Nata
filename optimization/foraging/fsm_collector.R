args = commandArgs(trailingOnly=TRUE)

readRdata <- function(exp_name) {
        output_file <- file(paste(exp_name, ".txt", sep=""))
        content <- c()
        for (run in 1:10) {
                run_file <- paste(exp_name, sprintf("-%02d", run), "/irace.Rdata", sep="")
                if (file.exists(run_file)) {
                        load(run_file)
                        pnames <- iraceResults$parameters$names
                        switches <- iraceResults$parameters$switches[pnames]
                        #Selection of the candidate (best of the last iteration)
                        candidate <- iraceResults$allConfigurations[iraceResults$iterationElites[length(iraceResults$iterationElites)] ,pnames]
                        fsm <- irace:::buildCommandLine(values=candidate , switches=switches)
                        content <- c(content, fsm)
                } else {
                        stop("File does not exist!")
                }
        }
	writeLines(content, output_file)
        close(output_file)
}


if (length(args)!=2) {
        stop("The working directory (arg[1]) and the experiment name (arg[2]) have to be specified")
} else {
        library("irace")
        setwd(args[1])
        readRdata(args[2])
}
