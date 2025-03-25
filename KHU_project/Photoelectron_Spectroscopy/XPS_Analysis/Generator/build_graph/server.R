library(shiny)

# functions
read_XPS<-function(path, header=T){
  tmp<-read.csv(file=path, sep=' ', header=header)
  tmp<-tmp[, c(1, 2)]
  return(tmp)
}

max_normalize<-function(df){
  max<-max(df[, 2])
  min<-min(df[, 2])
  df[, 2]<-(df[, 2]-min)/(max-min)
  return(df)
}
  
carbon_normalize<-function(df){
  C_position<-1202.3
  max<-df[which(df[, 1]==C_position), 2]
  min<-min(df[, 2])
  df[, 2]<-(df[, 2]-min)/(max-min)
  return(df)
}

total_normalize<-function(df){
  tot<-sum(df[, 2])
  df[, 2]<-df[, 2]/tot
  return(df)
}
#

shinyServer(function(input, output){
  data<-reactive({
    # read_data
    one_data=TRUE
    XPS1<-input$data1
    if(is.null(XPS1)) return(NULL)
    XPS1<-read_XPS(XPS1$datapath, header=input$header)
    colnames(XPS1)<-c("POS", "XPS1")
    XPS<-XPS1
    XPS2<-input$data2
    if(!is.null(XPS2)){
      XPS2<-read_XPS(XPS2$datapath, header=input$header)
      XPS<-cbind(XPS1, XPS2[, 2])
      colnames(XPS)<-c("POS", "XPS1", "XPS2")
      one_data=FALSE
    }
    # initialize
    if(input$nor){
      nor_fun<-switch(input$nor_opt, "max"=max_normalize, "car"=carbon_normalize, "tot"=total_normalize)
      XPS[, c(1, 2)]<-nor_fun(XPS[, c(1, 2)])
      if(!one_data){
        XPS[, c(1, 3)]<-nor_fun(XPS[, c(1, 3)])
      }
    }
    # range
    xlim_l<-input$range[1]
    xlim_h<-input$range[2]
    XPS<-XPS[XPS$POS>xlim_l & XPS$POS<xlim_h, ]
    XPS[, -1]<-format(XPS[, -1], scientific=TRUE)
    XPS
  })
  # RAW
  #output$summary<-renderText()
  output$table<-renderTable({
    data()
  })
  output$summary<-renderPrint({
    XPS<-data()
    if(!is.null(XPS)){
      XPS[, 2]<-as.numeric(XPS[, 2])
      sum<-format(sum(XPS[, 2]), scientific=T)
      avg<-format(mean(XPS[, 2]), scientific=T)
      min<-min(XPS[, 2])
      max<-max(XPS[, 2])
      min_pos<-XPS[which(XPS[, 2]==min), 1]
      max_pos<-XPS[which(XPS[, 2]==max), 1]
      cat("XPS 1", "\n", sep="")
      cat("min: ", min, "(", min_pos, ")", "\n", "max: ", max, "(", max_pos, ")", "\n", sep="")
      cat("sum: ", sum, ", ", "avg: ", avg, "\n", sep="")
      if(ncol(XPS)==3){
        XPS[, 3]<-as.numeric(XPS[, 3])
        sum<-format(sum(XPS[, 3]), scientific=T)
        avg<-format(mean(XPS[, 3]), scientific=T)
        min<-min(XPS[, 3])
        max<-max(XPS[, 3])
        min_pos<-XPS[which(XPS[, 3]==min), 1]
        max_pos<-XPS[which(XPS[, 3]==max), 1]
        cat("\n", "XPS 2", "\n", sep="")
        cat("min: ", min, "(", min_pos, ")", "\n", "max: ", max, "(", max_pos, ")", "\n", sep="")
        cat("sum: ", sum, ", ", "avg: ", avg, "\n", sep="")
      }
    }
  })
  # GRAPH
  output$r_graph<-renderPlot({
    XPS<-data()
    one_data=TRUE
    low<-0
    high<-0
    if(!is.null(XPS)){
      low<-min(as.numeric(XPS[, 2]))
      high<-max(as.numeric(XPS[, 2]))
      
      if(ncol(XPS)==3){
        low2<-min(as.numeric(XPS[, 3]))
        high2<-max(as.numeric(XPS[, 3]))
        if(low>low2) low<-low2
        if(high<high2) high<-high2
        one_data=FALSE
      }
      par(mar=c(5, 5, 3, 2)+0.1)
      plot(XPS$POS, XPS$XPS1, axes=T, ylim=c(low, high), col=input$col1, main="XPS", xlab="Kinetic Energy(eV)", ylab="Intensity", type=input$p_type)
      if(!one_data){
        par(new=T)
        plot(XPS$POS, XPS$XPS2, axes=F, ylim=c(low, high), col=input$col2, xlab="", ylab="", type=input$p_type)
      }
      if(input$add1){
        abline(h=0, col="red")
      }
      if(!is.na(suppressWarnings(as.numeric(input$add2)))){
        abline(v=as.numeric(input$add2), col="red")
      }
    }
  })
  # SEPARATE
  output$s_graph<-renderPlot({
    tmp<-data()
    if(!is.null(tmp)){
      if(ncol(tmp)==3){
        XPS<-tmp[, 1]
        XPS<-data.frame(XPS, as.numeric(tmp[, 2])-as.numeric(tmp[, 3]))
        colnames(XPS)<-c("POS", "XPS_diff")
        min<-min(as.numeric(XPS[, 2]))
        max<-max(as.numeric(XPS[, 2]))
        plot(XPS$POS, XPS$XPS_diff, axes=T, ylim=c(min, max), col=input$col1, main="XPS1 - XPS2", xlab="Kinetic Energy(eV)", ylab="Intensity", type=input$p_type)
        if(input$add1){
          abline(h=0, col="red")
        }
        if(!is.na(suppressWarnings(as.numeric(input$add2)))){
          abline(v=as.numeric(input$add2), col="red")
        }
      }
    }
  })
})
