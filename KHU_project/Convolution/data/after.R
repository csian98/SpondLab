setwd("/Users/csian/Desktop/CP/Conovlution/data")
before<-read.table("data.txt")

setwd("/Users/csian/Desktop/CP/Conovlution/Source")
after<-read.table("after.txt")
after<-cbind(before[, 1], after)

colnames(before)<-c("Kinetic Enery[eV]", "CPS")
colnames(after)<-c("Kinetic Enery[eV]", "CPS")

par(mfrow=c(1, 2))
plot(before, type='l', main="BEFORE")
plot(after, type='l', main="AFTER")
