file_name='CPS_raw.txt'
setwd('/Users/csian/Desktop/CP/Conovlution/20220407')
df<-read.csv(file_name, sep='\t', header=F)
df<-df[, 2]
write.table(df, file="CSV_df.txt", row.names=F, col.names=F)

filter_name='250nm.txt'
setwd('/Users/csian/Desktop/CP/Conovlution/20220407')
filter<-read.csv(filter_name, sep='\t')
filter[, 1]<-1240/filter[, 1]

gap=0.05
pos=7.9
end=4.15
x<-c()
y<-c()
while(pos>end){
  ind=which.min(abs(filter[, 1]-pos))
  x<-append(x, pos)
  y<-append(y, filter[ind, 2])
  pos<-pos-gap
}
x<-rev(x); y<-rev(y)
filter_r<-data.frame(x, y)

ma<-max(filter_r[, 2])
mi<-min(filter_r[, 2])
filter_r[, 2]<-(filter_r[, 2]-mi)/(ma-mi)

write.table(filter_r[, 2], file="filter.txt", row.names=F, col.names=F)
