library(shiny)

shinyUI(pageWithSidebar(
  headerPanel("XPS_Analysis"),
  sidebarPanel(
    fileInput("data1", "choose XPS 1", accept=c("text/csv", "text/comma-separated-values", ".spc", ".txt")),
    fileInput("data2", "choose XPS 2", accept=c("text/csv", "text/comma-separated-values", ".spc", ".txt")),
    checkboxInput("header", "Header", TRUE),
    checkboxInput("nor", "Normalize", FALSE),
    selectInput("nor_opt", "Option", list("Maximum"="max", "Carbon"="car", "Total"="tot")),
    sliderInput("range", "Range", min=400, max=1486, value=c(400, 1486), step=0.5),
    selectInput("col1", "XPS1 Color", list("BLACK"="black", "BLUE"="blue", "RED"="red", "GREEN"="green")),
    selectInput("col2", "XPS2 Color", list("BLACK"="black", "BLUE"="blue", "RED"="red", "GREEN"="green")),
    radioButtons("p_type", "GRAPH TYPE", list("LINE"="l", "POINT"="p", "BOTH"="b")),
    checkboxInput("add1", "h=0", FALSE),
    textInput("add2", "Position", "POS(float)"),
    helpText("Note: when separate, using 'XPS 2' as pure layer"),
    tags$br(),
    submitButton("Update")
  ),
  mainPanel(
    tabsetPanel(
      tabPanel("GRAPH", plotOutput("r_graph")),
      tabPanel("SEPARATE", plotOutput("s_graph")),
      tabPanel("RAW",
        verbatimTextOutput("summary"),
        tableOutput("table")
      )
    )
  )
))
