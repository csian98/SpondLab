library(shiny)

shinyUI(pageWithSidebar(
  headerPanel("GRAPH DRAWER"),
  # SIDEBAR PANEL
  sidebarPanel(
    shinyjs::useShinyjs(),
    id="side-panel",
    # INPUT1
    fileInput("data1", "(.txt / .csv)_with dimension nx1; y)", accept=c("text/csv", "text/comma-seperated-values", ".txt")),
    textInput("min_x", "START X", 0),
    textInput("max_x", "END X", 1),
    tags$hr(),
    
    # INPUT2
    fileInput("data2", "(.txt / .csv)_with dimension nx2; x-y)", accept=c("text/csv", "text/comma-seperated-values", ".txt")),
    checkboxInput("header", "HEADER", FALSE),
    
    radioButtons("line_type", "GRAPH TYPE", list("LINE"="l", "POINT"="p", "MIXED"="b")),
    selectInput("col", "COLOUR", list("BLACK"="black", "BLUE"="blue", "RED"="red", "GREEN"="green")),
    
    textInput("xlabel", "X-AXIS LABEL", "x lab"),
    textInput("ylabel", "Y-AXIS LABEL", "y lab"),
    
    tags$hr(),
    sliderInput("range", "RANGE", min=0, max=1, value=c(0, 1), step=0.005),
    verbatimTextOutput("range"),
    
    tags$hr(),
    submitButton("COMPLETE"),
    tags$br(),
    actionButton("reset_btn", "CLEAR")
  ),
  # MAIN PANEL
  mainPanel(
    tabsetPanel(
      # TAB1:
      tabPanel("SINGLE", plotOutput("graph")),
      # TAB2:
      tabPanel("RAW", tableOutput("table"))
    )
  )
))
