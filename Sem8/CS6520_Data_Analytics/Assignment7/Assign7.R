## Read input dataset, this is separated by comma
data = read.csv("crx.data.txt",header=FALSE, sep=",")

# replace all '?' in data with NA
data[data == "?" ] = NA

# Drop all the rows that have NA from the data frame
data = data[complete.cases(data),]

# convert all data into numeric so that it fits properly into the logistic regression model
data$V1 = as.numeric(data$V1)
data$V2 = as.numeric(data$V2)
data$V3 = as.numeric(data$V3)
data$V4 = as.numeric(data$V4)
data$V5 = as.numeric(data$V5)
data$V6 = as.numeric(data$V6)
data$V7 = as.numeric(data$V7)
data$V8 = as.numeric(data$V8)
data$V9 = as.numeric(data$V9)
data$V10 = as.numeric(data$V10)
data$V11 = as.numeric(data$V11)
data$V12 = as.numeric(data$V12)
data$V13 = as.numeric(data$V13)
data$V14 = as.numeric(data$V14)
data$V15 = as.numeric(data$V15)

# Its a binary classification problem, so we convert it + into 0 and - into 1
data$V16 = as.character(data$V16)
data$V16[which(data$V16 == "+")] = 0
data$V16[which(data$V16 == "-")] = 1 
data$V16 = as.numeric(data$V16)

# Random 80-20 split of the data into train and test respectively
tmp_data = sort(sample(nrow(data), nrow(data)*0.8))
train = data[tmp_data, ]
test = data[-tmp_data, ]

# Use binomial logistic regression to predict V16 using V1,V2, ... V15
crx_model = glm(formula = V16 ~ V1 + V2 + V3 + V4 + V5 + V6 + V7 + V8 + V9 + V10 + V11 + V12 + V13 + V14 + V15, data=train, family=binomial)
print(summary(crx_model))

#Call:
#glm(formula = V16 ~ V1 + V2 + V3 + V4 + V5 + V6 + V7 + V8 + V9 + 
#    V10 + V11 + V12 + V13 + V14 + V15, family = binomial, data = train)

#Deviance Residuals: 
#    Min       1Q   Median       3Q      Max  
#-2.9241  -0.4995   0.1914   0.3100   2.5861  

#Coefficients:
#              Estimate Std. Error z value Pr(>|z|)    
#(Intercept) -1.506e+01  1.177e+03  -0.013  0.98979    
#V1          -1.936e-01  3.183e-01  -0.608  0.54306    
#V2           4.935e-04  1.733e-03   0.285  0.77578    
#V3           1.508e-02  2.868e-02   0.526  0.59895    
#V4           1.152e+01  5.885e+02   0.020  0.98438    
#V5          -5.499e+00  2.942e+02  -0.019  0.98509    
#V6          -2.230e-02  3.326e-02  -0.671  0.50253    
#V7          -7.862e-02  5.846e-02  -1.345  0.17866    
#V8          -1.343e-01  6.286e-02  -2.137  0.03263 *  
#V9          -3.819e+00  3.665e-01 -10.420  < 2e-16 ***
#V10         -3.321e-01  4.004e-01  -0.829  0.40684    
#V11         -1.634e-01  6.992e-02  -2.337  0.01946 *  
#V12          1.231e-01  2.965e-01   0.415  0.67806    
#V13         -1.047e-01  2.626e-01  -0.399  0.69013    
#V14          2.779e-03  3.263e-03   0.852  0.39433    
#V15         -5.339e-04  1.795e-04  -2.975  0.00293 ** 
#---
#Signif. codes:  0 ‘***’ 0.001 ‘**’ 0.01 ‘*’ 0.05 ‘.’ 0.1 ‘ ’ 1

#(Dispersion parameter for binomial family taken to be 1)

#    Null deviance: 721.16  on 521  degrees of freedom
#Residual deviance: 320.45  on 506  degrees of freedom
#AIC: 352.45

#Number of Fisher Scoring iterations: 13


# Use trained model to evaluate on the test data.
eval_results = predict(crx_model, test, type='response')

# Set threshold of 0.5, ie if > 0.5 then it is class 1 otherwise it is class 0
eval_results = ifelse(eval_results > 0.5, 1, 0)

# Find the misclassification error
misClasificationError = mean(eval_results != test$V16)
print(paste('Accuracy', 1-misClasificationError))

# "Accuracy 0.900763358778626"


## For getting confusion matrix and plotting ROC curve 
eval_res_func = predict(crx_model, type='response')
# Confusion matrix
table(train$V16, eval_res_func > 0.5)

#    FALSE TRUE
#  0   224   19
#  1    52  227




# Firstly install caTools and ggplot2
library(ROCR)
ROCRpred = prediction(eval_res_func, train$V16)
ROCRperf = performance(ROCRpred, 'tpr', 'fpr')
plot(ROCRperf, colorize = TRUE, text.adj = c(0.0,1.0))


#The plot gets saved as Rplots.pdf
