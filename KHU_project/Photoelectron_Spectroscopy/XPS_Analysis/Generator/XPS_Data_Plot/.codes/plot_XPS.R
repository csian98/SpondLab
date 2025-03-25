# for plot XPS_data from SESSA
# made by Csian

file_name<-commandArgs()
file_name<-file_name[length(file_name)]
path<-getwd()
file_path<-paste(path, "/", file_name, sep='')

data<-read.csv(file=file_path, sep=' ')
data<-data[, c(1, 2)]

png(filename="tmp_XPS.png", width=1400, height=600, unit="px")
plot(data, type='l', xlab="Kinetic Energy[eV]", ylab="Intensity")
dev.off()
