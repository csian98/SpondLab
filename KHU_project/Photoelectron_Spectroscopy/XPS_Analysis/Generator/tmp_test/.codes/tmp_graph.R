# for plot XPS_data from SESSA
# made by Csian

file_name<-commandArgs()
file_name<-file_name[length(file_name)]
eq<-file_name

find_name<-paste(file_name, "reg1.spc", sep='')
file_name<-paste(file_name, ".png", sep='')

path<-"/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/tmp_test/spc"
path_down="/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/tmp_test/graph"
setwd(path_down)

file_path<-paste(path, "/", find_name, sep='')

data<-read.csv(file=file_path, sep=' ')
data<-data[, c(1, 2)]

path<-"/Users/csian/Desktop/CP/Photoelectron_Spectroscopy/XPS_Analysis/Generator/tmp_test/tmp/tmp_peak.txt"
peak<-read.csv(file=path, sep='', header=F)
pm<-max(data[, 2])

png(filename=file_name, width=1400, height=600, unit="px")
plot(data, type='l', xlab="Kinetic Energy[eV]", ylab="Intensity", main=eq)
text(peak[, 2], rep(c(0, 0.1*pm, 0.2*pm, 0.3*pm, 0.4*pm, 0.5*pm, 0.6*pm, 0.7*pm, 0.8*pm, 0.9*pm, pm), length.out=nrow(peak)), peak[, 1], cex=1, adj=c(0, 1))
abline(v=peak[, 2], col='blue', lty=3)
dev.off()
