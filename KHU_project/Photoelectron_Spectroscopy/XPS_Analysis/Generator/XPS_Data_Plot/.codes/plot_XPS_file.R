# for plot XPS_data from SESSA
# made by Csian

args<-commandArgs()
start<-length(args)-2; end<-length(args)
args<-args[start:end]
path<-args[1]
file_name<-args[2]
mkdir_name<-args[3]

path_file<-paste(path, '/', file_name, sep='')
data<-read.csv(file=path_file, sep=' ')
data<-data[, c(1, 2)]

new_path<-paste("./Plot_Data/", mkdir_name, sep='')
print(new_path)
setwd(new_path)

saved_name<-paste(file_name, ".png", sep='')
png(filename=saved_name, width=1400, height=600, unit="px")
plot(data, type='l', xlab="Kinetic Energy[eV]", ylab="Intensity")
dev.off
