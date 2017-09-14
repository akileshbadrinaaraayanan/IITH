data = read.csv("bank-additional/bank-additional-full.csv",header=TRUE, sep=";")

data$y = as.character(data$y)
data$y[which(data$y=="yes")] = 0
data$y[which(data$y=="no")] = 1 
data$y = as.numeric(data$y)

# Random 80-20 split of the data into train and test respectively.
tmp_data = sort(sample(nrow(data), nrow(data)*0.8))
train = data[tmp_data, ]
test = data[-tmp_data, ]

bank_model = glm(formula = y~age + job + marital + education + default + housing + loan + contact + month + day_of_week + duration + campaign + pdays + previous + poutcome + emp.var.rate + cons.price.idx + cons.conf.idx + euribor3m + nr.employed, data=train, family=binomial)

print(summary(bank_model))

#Call:
#glm(formula = y ~ age + job + marital + education + default + 
#    housing + loan + contact + month + day_of_week + duration + 
#    campaign + pdays + previous + poutcome + emp.var.rate + cons.price.idx + 
#    cons.conf.idx + euribor3m + nr.employed, family = binomial, 
#    data = train)

#Deviance Residuals: 
#    Min       1Q   Median       3Q      Max  
#-3.3854   0.1347   0.1862   0.2990   5.7704  

#Coefficients: (1 not defined because of singularities)
#                               Estimate Std. Error z value Pr(>|z|)    
#(Intercept)                   2.450e+02  4.232e+01   5.790 7.04e-09 ***
#age                           6.513e-04  2.718e-03   0.240 0.810630    
#jobblue-collar                1.959e-01  8.988e-02   2.179 0.029318 *  
#jobentrepreneur               1.131e-01  1.387e-01   0.815 0.415162    
#jobhousemaid                 -5.700e-02  1.633e-01  -0.349 0.727058    
#jobmanagement                 2.979e-02  9.513e-02   0.313 0.754165    
#jobretired                   -3.119e-01  1.204e-01  -2.591 0.009580 ** 
#jobself-employed              2.143e-01  1.330e-01   1.610 0.107294    
#jobservices                   1.305e-01  9.662e-02   1.351 0.176701    
#jobstudent                   -1.478e-01  1.240e-01  -1.191 0.233481    
#jobtechnician                -3.541e-02  7.934e-02  -0.446 0.655358    
#jobunemployed                 1.587e-02  1.435e-01   0.111 0.911902    
#jobunknown                   -9.763e-02  2.575e-01  -0.379 0.704521    
#maritalmarried                2.016e-02  7.666e-02   0.263 0.792535    
#maritalsingle                -4.446e-02  8.721e-02  -0.510 0.610177    
#maritalunknown                3.012e-02  4.701e-01   0.064 0.948910    
#educationbasic.6y            -3.908e-02  1.373e-01  -0.285 0.775874    
#educationbasic.9y             2.726e-02  1.072e-01   0.254 0.799232    
#educationhigh.school         -5.958e-02  1.031e-01  -0.578 0.563264    
#educationilliterate          -1.079e+00  7.528e-01  -1.433 0.151824    
#educationprofessional.course -1.251e-01  1.133e-01  -1.104 0.269752    
#educationuniversity.degree   -2.204e-01  1.028e-01  -2.143 0.032089 *  
#educationunknown             -1.914e-01  1.330e-01  -1.438 0.150342    
#defaultunknown                2.918e-01  7.567e-02   3.856 0.000115 ***
#defaultyes                    7.264e+00  1.134e+02   0.064 0.948904    
#housingunknown                1.642e-01  1.582e-01   1.038 0.299405    
#housingyes                    3.132e-02  4.621e-02   0.678 0.498011    
#loanunknown                          NA         NA      NA       NA    
#loanyes                       5.316e-02  6.410e-02   0.829 0.406888    
#contacttelephone              6.304e-01  8.594e-02   7.336 2.21e-13 ***
#monthaug                     -9.864e-01  1.346e-01  -7.327 2.35e-13 ***
#monthdec                     -3.514e-01  2.400e-01  -1.464 0.143169    
#monthjul                     -1.755e-01  1.081e-01  -1.624 0.104418    
#monthjun                      4.919e-01  1.401e-01   3.512 0.000445 ***
#monthmar                     -2.070e+00  1.615e-01 -12.816  < 2e-16 ***
#monthmay                      3.947e-01  9.270e-02   4.257 2.07e-05 ***
#monthnov                      3.716e-01  1.351e-01   2.751 0.005936 ** 
#monthoct                     -3.227e-01  1.708e-01  -1.889 0.058862 .  
#monthsep                     -4.161e-01  2.000e-01  -2.080 0.037500 *  
#day_of_weekmon                1.334e-01  7.343e-02   1.816 0.069354 .  
#day_of_weekthu               -1.733e-02  7.132e-02  -0.243 0.808024    
#day_of_weektue               -2.321e-02  7.336e-02  -0.316 0.751694    
#day_of_weekwed               -1.042e-01  7.333e-02  -1.421 0.155444    
#duration                     -4.718e-03  8.338e-05 -56.586  < 2e-16 ***
#campaign                      4.487e-02  1.301e-02   3.449 0.000563 ***
#pdays                         8.441e-04  2.443e-04   3.455 0.000549 ***
#previous                     -2.149e-02  6.800e-02  -0.316 0.751929    
#poutcomenonexistent          -5.692e-01  1.076e-01  -5.291 1.22e-07 ***
#poutcomesuccess              -1.016e+00  2.383e-01  -4.264 2.01e-05 ***
#emp.var.rate                  1.847e+00  1.568e-01  11.782  < 2e-16 ***
#cons.price.idx               -2.266e+00  2.789e-01  -8.126 4.43e-16 ***
#cons.conf.idx                -1.444e-02  8.688e-03  -1.663 0.096399 .  
#euribor3m                    -3.884e-01  1.449e-01  -2.680 0.007366 ** 
#nr.employed                  -5.547e-03  3.446e-03  -1.610 0.107453    
#---
#Signif. codes:  0 ‘***’ 0.001 ‘**’ 0.01 ‘*’ 0.05 ‘.’ 0.1 ‘ ’ 1

#(Dispersion parameter for binomial family taken to be 1)

#    Null deviance: 23244  on 32949  degrees of freedom
#Residual deviance: 13666  on 32897  degrees of freedom
#AIC: 13772

#Number of Fisher Scoring iterations: 10

# anova bank_model with chisq
#anova(bank_model, test="Chisq")

# Use trained model to evaluate on the test data.
eval_results = predict(bank_model, test, type='response')

# Set threshold of 0.5, ie if > 0.5 then it is class 1 otherwise it is class 0
eval_results = ifelse(eval_results > 0.5, 1, 0)

# Find the misclassification error
misClasificationError = mean(eval_results != test$y)
print(paste('Accuracy', 1-misClasificationError))
#[1] "Accuracy 0.911022092740957"

## For getting confusion matrix and plotting ROC curve 
eval_res_func = predict(bank_model, type='response')
# Confusion matrix
table(train$y, eval_res_func > 0.5)
   
#    FALSE  TRUE
#  0  1602  2121
#  1   780 28447


# Firstly install caTools and ggplot2
library(ROCR)
ROCRpred = prediction(eval_res_func, train$y)
ROCRperf = performance(ROCRpred, 'tpr', 'fpr')
plot(ROCRperf, colorize = TRUE, text.adj = c(0.0,1.0))


#The plot gets saved as Rplots.pdf
