setwd('/Users/csian/Desktop/CP/Conovlution/data')
save_data<-"/Users/csian/Desktop/CP/Conovlution/Source/data.txt"
save_filter<-"/Users/csian/Desktop/CP/Conovlution/Source/filter.txt"

data<-read.table("data.txt")
filter<-read.table("filter.txt")

# need to check x gap!
filter<-filter[filter[, 2]>0.01, ]
# re-normalization
filter[, 2]<-scale(filter[, 2], min(filter[, 2]), max(filter[, 2])-min(filter[, 2]))

data<-data[, 2]
filter<-filter[, 2]

write.table(data, save_data, row.names=F, col.names=F)
write.table(filter, save_filter, row.names=F, col.names=F)
