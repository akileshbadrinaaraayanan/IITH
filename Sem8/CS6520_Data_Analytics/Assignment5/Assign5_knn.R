## Load class library that has knn inbuilt. k nearest neighbors classifier is used in this example
library(class)

# Read the madelon train data
train_data = read.table("madelon_train.data.txt", header=FALSE)

# Load the labels from madelon_train.labels.txt
train_labels= read.table("madelon_train.labels.txt", header=FALSE)

# Make all the -1 in labels to 0
train_labels$V1[train_labels$V1==-1] = 0

## Now do the same for validation data
# Read the validation data
valid_data = read.table("madelon_valid.data.txt", header=FALSE)

# Use the validation labels file to compute misclassification error
valid_labels = read.table("madelon_valid.labels.txt", header=FALSE)

# Make all the -1 in valid labels to 0
valid_labels$V1[valid_labels$V1 == -1] = 0

train_labels_1 = train_labels[, 1]
madelon_model_results = knn(train=train_data, test=valid_data, cl=train_labels_1, k=44)


# Calculate Classification Error
misClassificationError = mean(madelon_model_results != valid_labels$V1)

# Report Accuracy
print(paste('Accuracy', 1-misClassificationError))
# [1] "Accuracy 0.766666666666667"



