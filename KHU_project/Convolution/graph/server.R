library(shiny)

read_1<-function(path, header=T, x_min=0, x_max=1, x_lab='x', y_lab='y'){
  tmp<-read.table(file=path, header=header)
  if(length(tmp)!=1){
    print("DATA1 NEEDS TO BE nx1 DIM")
    return(NULL)
  }
  len<-length(tmp[, 1])
  x<-seq(x_min, x_max, length.out=len)
  tmp<-cbind(x, tmp)
  colnames(tmp)<-c(x_lab, y_lab)
  return(tmp)
}

read_2<-function(path, header=T, x_lab='x', y_lab='y'){
  tmp<-read.table(file=path, header=header)
  if(length(tmp)!=2){
    print("DATA2 NEEDS TO BE nx2 DIM")
    return(NULL)
  }
  colnames(tmp)<-c(x_lab, y_lab)
  return(tmp)
}

shinyServer(function(input, output){
  # observer
  observeEvent(input$reset_btn, {
    shinyjs::reset("side-panel")
  })
  data<-reactive({
    # DATA READ
    state1=TRUE
    state2=TRUE
    raw1<-input$data1
    if(is.null(raw1)){
      state1=FALSE
    } else {
      raw<-read_1(raw1$datapath, header=input$header, x_min=input$min_x, x_max=input$max_x, x_lab=input$xlabel, y_lab=input$ylabel)
    }
    raw2<-input$data2
    if(is.null(raw2)){
      state2=FALSE
    } else {
      raw<-read_2(raw2$datapath, header=input$header, x_lab=input$xlabel, y_lab=input$ylabel)
    }
    
    if(!xor(state1, state2)) return(NULL)
    
    len<-length(raw[, 1])
    if(state1){
      xind_l<-round(as.numeric(input$range[1])*len)
      xind_h<-round(as.numeric(input$range[2])*len)
      raw<-raw[xind_l:xind_h, ]
    }
    return(raw)
  })
  
  # TEXT OUTPUT
  output$range<-renderPrint({
    range<-as.numeric(input$max_x)-as.numeric(input$min_x)
    x_min<-as.numeric(input$min_x)+range*as.numeric(input$range[1])
    x_max<-as.numeric(input$min_x)+range*as.numeric(input$range[2])
    cat("FROM ", x_min, " TO ", x_max)
  })
  
  # PANEL 1: GRAPH
  output$graph<-renderPlot({
    data<-data()
    if(is.null(data)){
      cat("NO DATA UPLOADED")
      return(NULL)
    }
    plot(data[, 1], data[, 2], col=input$col, type=input$line_type, xlab=colnames(data)[1], ylab=colnames(data)[2])
  })
  
  # PANEL 2: TABLE
  output$table<-renderTable({
    data<-data()
    if(is.null(data)) return(NULL)
    data
  })
})
