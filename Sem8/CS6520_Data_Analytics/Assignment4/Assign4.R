# Read input dataset, it didn't have an explicit header so set header = FALSE
data = read.csv('servo.txt', header=FALSE)

# Random 80-20 split of the data into train and test respectively
tmp_data = sort(sample(nrow(data), nrow(data)*0.8))
train = data[tmp_data, ]
test = data[-tmp_data, ]

# Build a regression model where the dependent variable is V5 and it depends on columns V1,V2,V3,V4
# Although V1,V2 are are characters, R takes care of converting it into numeric factors
servo_model = lm(V5 ~ V1 + V2 + V3 + V4, data = train)

# Print model summary
print(summary(servo_model))

#
#Call:
#lm(formula = V5 ~ V1 + V2 + V3 + V4, data = train)

#Residuals:
#    Min      1Q  Median      3Q     Max 
#-1.8953 -0.7149 -0.2870  0.6971  4.2250 

#Coefficients:
#            Estimate Std. Error t value Pr(>|t|)    
#(Intercept)   7.7418     0.6004  12.895  < 2e-16 ***
#V1B          -0.1243     0.2845  -0.437 0.663062    
#V1C          -0.5098     0.2928  -1.741 0.084156 .  
#V1D          -1.3745     0.3358  -4.094 7.67e-05 ***
#V1E          -0.6084     0.2952  -2.061 0.041421 *  
#V2B          -0.6845     0.2849  -2.402 0.017794 *  
#V2C          -0.8821     0.2921  -3.020 0.003084 ** 
#V2D          -1.0613     0.2902  -3.657 0.000378 ***
#V2E          -1.0167     0.3139  -3.239 0.001547 ** 
#V3           -1.6069     0.1798  -8.939 5.10e-15 ***
#V4            0.5625     0.1308   4.301 3.45e-05 ***
#---
#Signif. codes:  0 ‘***’ 0.001 ‘**’ 0.01 ‘*’ 0.05 ‘.’ 0.1 ‘ ’ 1

#Residual standard error: 1.093 on 122 degrees of freedom
#Multiple R-squared:  0.5281,	Adjusted R-squared:  0.4894 
#F-statistic: 13.65 on 10 and 122 DF,  p-value: 7.087e-16

# Use confidence as interval to make predictions
eval_results = predict(servo_model, test, interval="confidence")
print(eval_results)
eval_results = eval_results[, 1]

# Compute RSquare metric : 1 - (SSE/SST)
RSq <- 1 - (sum((test$V5 - eval_results )^2)/sum((test$V5-mean(test$V5))^2))
print(RSq)

#[1] 0.6123821


# Mean Sq Error computation
MSE <- sqrt(mean((test$V5 - eval_results)^2))
print(MSE)

# [1] 1.032821
