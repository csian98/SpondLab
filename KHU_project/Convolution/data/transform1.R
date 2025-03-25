setwd('/Users/csian/Desktop/CP/Conovlution/data')
data<-read.table("CPS_high_2021-03-25 오후 3_33_44.txt")
data<-data[, c(1, 7)]
colnames(data)<-c("Kinetic Energy[eV]", "CPS")
write.table(data, "data.txt", row.names=F, col.names=F)

filter<-read.table("250nm.txt")
# filter x-axis scale transform & normalization(filter[0, ]이상치 제거)
filter<-filter[-1, ]
filter[, 1]<-1240/filter[, 1]
filter[, 2]<-scale(filter[, 2], min(filter[, 2]), max(filter[, 2])-min(filter[, 2]))

# build gap as 0.025
gap<-0.025
start=7.95; end=4.125
x<-c(); y<-c()

while(start>end){
  ind=which.min(abs(filter[, 1]-start))
  x<-append(x, start)
  y<-append(y, filter[ind, 2])
  start<-start-gap
}
x<-rev(x); y<-rev(y)
filter<-data.frame(x, y)
colnames(filter)<-c("eV", "filter")
write.table(filter, "filter.txt", row.names=F, col.names=F)

# plot
filter[, 1]<-filter[, 1]-range(filter[, 1])[1]
plot(data, type='l', col="black")
par(new=T)
plot(filter, type='l', col="blue", xlim=range(data[, 1]), axes=F, xaxt='n', yaxt='n', ann=F)
