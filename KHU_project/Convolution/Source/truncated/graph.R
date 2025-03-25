setwd("/Users/csian/Desktop/CP/Conovlution/data")
before<-read.table("data.txt")

data_pos<-"/Users/csian/Desktop/CP/Conovlution/Source/truncated/k/"
graph_pos<-"/Users/csian/Desktop/CP/Conovlution/Source/truncated/graph/"
setwd(data_pos)

file_name="k_"
for(i in 1:16){
  tmp=paste(data_pos, file_name, i, ".txt", sep="")
  png_name=paste(graph_pos, file_name, i, ".png", sep="")
  after<-read.table(tmp)
  png(png_name)
  plot(before[, 1], after[, 1], type='l', xlab="Kinetic Energy[eV]", ylab="CPS")
  dev.off()
}
