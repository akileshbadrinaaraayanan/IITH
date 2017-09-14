# Read the madelon train data
data = read.table("madelon_train.data.txt", header=FALSE)

# Use only first 20 columns for making the predictions although there are 500 columns
train = data[,1:20]

# Load the labels from madelon_train.labels.txt
train_label_data = read.table("madelon_train.labels.txt", header=FALSE)
train_labels = train_label_data

# Make all the -1 in labels to 0
train_labels$V1[train_labels$V1==-1] = 0

# Add the labels as an additional last column to train
train$V21 = as.matrix(train_labels)

# Use binomial logistic regression to predict V21 from V1,V2, ...., V20
madelon_model = glm(formula = V21~V1+V2+V3+V4+V5+V6+V7+V8+V9+V10+V11+V12+V13+V14+V15+V16+V17+V18+V19+V20, data=train, family=binomial)
print(summary(madelon_model))

#Call:
#glm(formula = V21 ~ V1 + V2 + V3 + V4 + V5 + V6 + V7 + V8 + V9 + 
#    V10 + V11 + V12 + V13 + V14 + V15 + V16 + V17 + V18 + V19 + 
#    V20, family = binomial, data = train)

#Deviance Residuals: 
#     Min        1Q    Median        3Q       Max  
#-1.49006  -1.16757   0.01531   1.16830   1.48198  

#Coefficients:
#              Estimate Std. Error z value Pr(>|z|)  
#(Intercept) -3.2711282 17.2652202  -0.189   0.8497  
#V1           0.0005400  0.0070614   0.076   0.9390  
#V2           0.0011002  0.0014947   0.736   0.4617  
#V3           0.0006730  0.0011587   0.581   0.5614  
#V4          -0.0001542  0.0049905  -0.031   0.9753  
#V5           0.0019111  0.0010950   1.745   0.0809 .
#V6          -0.0095351  0.0066593  -1.432   0.1522  
#V7          -0.0011223  0.0011162  -1.006   0.3146  
#V8           0.0220573  0.0327030   0.674   0.5000  
#V9           0.0017066  0.0030100   0.567   0.5707  
#V10         -0.0028116  0.0062814  -0.448   0.6544  
#V11         -0.0025640  0.0012366  -2.073   0.0381 *
#V12          0.0006252  0.0016081   0.389   0.6975  
#V13          0.0018526  0.0013235   1.400   0.1616  
#V14         -0.0006170  0.0010599  -0.582   0.5605  
#V15         -0.0032574  0.0037284  -0.874   0.3823  
#V16          0.0017169  0.0020558   0.835   0.4036  
#V17         -0.0025512  0.0033586  -0.760   0.4475  
#V18         -0.0004597  0.0014859  -0.309   0.7570  
#V19          0.0028570  0.0019389   1.474   0.1406  
#V20         -0.0051529  0.0045451  -1.134   0.2569  
#---
#Signif. codes:  0 ‘***’ 0.001 ‘**’ 0.01 ‘*’ 0.05 ‘.’ 0.1 ‘ ’ 1

#(Dispersion parameter for binomial family taken to be 1)

#    Null deviance: 2772.6  on 1999  degrees of freedom
#Residual deviance: 2752.1  on 1979  degrees of freedom
#AIC: 2794.1

#Number of Fisher Scoring iterations: 4


# Read the validation data
valid_data = read.table("madelon_valid.data.txt", header=FALSE)
valid = valid_data[,1:20]

# Use trained model to evaluate on the validation data.
eval_results = predict(madelon_model, valid, type='response')
# Set threshold of 0.5, ie if > 0.5 then it is class 1 otherwise it is class 0
eval_results = ifelse(eval_results > 0.5, 1, 0)

print(eval_results)

# Use the validation labels file to compute misclassification error
valid_label_data = read.table("madelon_valid.labels.txt", header=FALSE)
valid_labels = valid_label_data

valid_labels$V1[valid_labels$V1 == -1] = 0


# Calculate Classification Error
misClassificationError = mean(eval_results != valid_labels$V1)

# Report Accuracy
print(paste('Accuracy',1-misClassificationError))

# [1] "Accuracy 0.513333333333333"

