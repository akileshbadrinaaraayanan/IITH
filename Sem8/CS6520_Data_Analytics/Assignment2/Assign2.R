#data = read.csv('breast_cancer.txt', header=FALSE)
#x_train = data[, 2:10]
#summary(data)

# Solving the breast cancer detection problem wherein I have to predict the class (benign (2) or malignant (4)) based on attributes 1-10
# Read the breast cancer dataset, set the header to FALSE because there are no headers in the txt file 
data = read.csv('breast_cancer.txt', header=FALSE)

#The dataset contains missing attributes '?'. I handle the missing attributes in the below mentioned way:
# Firstly I replace all '?' by NA
data$V7 <- gsub("?", NA, data$V7, fixed=TRUE)

#data$V7 <- as.integer(as.character(data$V7))
#colMean <- mean(data$V7, na.rm=TRUE)

# Then I impute the NA value with the mean of the other values present in the column. The NA value can also be imputed using mode 
for(i in 1:ncol(data)) {
data[,i] <- as.integer(as.character(data[,i]))
data[is.na(data[,i]), i] <- as.integer(mean(data[,i], na.rm=TRUE))
}


# I use logistic regression classifier, to do that the two classes should be '0' and '1' instead of '2' and '4'
data$V11[data$V11 == 2] = 0
data$V11[data$V11 == 4] = 1

#train <- data[1:(0.8*nrow(data)), ]
#test <- data[((0.8*nrow(data))+1):(nrow(data)+1), ]

# Random 80-20 split of the data into train and test respectively.
tmp_data = sort(sample(nrow(data), nrow(data)*0.8))
train = data[tmp_data, ]
test = data[-tmp_data, ]

# Use binomial logistic regression to predict V11 using V2,V3 .... V10
cancer_model = glm(formula = V11 ~ V2 + V3 + V4 + V5 + V6 + V7 + V8 + V9 + V10, data=train, family=binomial)
print(summary(cancer_model))


#Call:
#glm(formula = V11 ~ V2 + V3 + V4 + V5 + V6 + V7 + V8 + V9 + V10, 
#    family = binomial, data = train)

#Deviance Residuals: 
#    Min       1Q   Median       3Q      Max  
#-3.4761  -0.1217  -0.0682   0.0235   2.4589  

#Coefficients:
#              Estimate Std. Error z value Pr(>|z|)    
##(Intercept) -10.026808   1.243886  -8.061 7.58e-16 ***
#V2            0.538591   0.150714   3.574 0.000352 ***
#V3            0.002983   0.206373   0.014 0.988467    
#V4            0.249248   0.233618   1.067 0.286015    
#V5            0.326314   0.125067   2.609 0.009078 ** 
#V6            0.090798   0.162711   0.558 0.576820    
#V7            0.352217   0.093885   3.752 0.000176 ***
#V8            0.539756   0.173556   3.110 0.001871 ** 
#V9            0.172816   0.112336   1.538 0.123953    
#V10           0.563736   0.519917   1.084 0.278240    
#---
#Signif. codes:  0 ‘***’ 0.001 ‘**’ 0.01 ‘*’ 0.05 ‘.’ 0.1 ‘ ’ 1

#(Dispersion parameter for binomial family taken to be 1)

#    Null deviance: 721.781  on 558  degrees of freedom
#Residual deviance:  96.984  on 549  degrees of freedom
#AIC: 116.98

#Number of Fisher Scoring iterations: 8


# Use trained model to evaluate on the test data.
eval_results = predict(cancer_model, test, type='response')
# Set threshold of 0.5, ie if > 0.5 then it is class 1 otherwise it is class 0
eval_results = ifelse(eval_results > 0.5, 1, 0)

#print(eval_results)
#print(test$V11)

# Find the misclassification error
misClasificationError = mean(eval_results != test$V11)
print(paste('Accuracy', 1-misClasificationError))


#[1] "Accuracy 0.971428571428571"


## For getting confusion matrix and plotting ROC curve 
eval_res_func = predict(cancer_model, type='response')
# Confusion matrix
table(train$V11, eval_res_func > 0.5)


#    FALSE TRUE
#  0   355   10
#  1     9  185


# Firstly install caTools and ggplot2
library(ROCR)
ROCRpred = prediction(eval_res_func, train$V11)
ROCRperf = performance(ROCRpred, 'tpr', 'fpr')
plot(ROCRperf, colorize = TRUE, text.adj = c(0.0,1.0))


#The plot gets saved as Rplots.pdf


