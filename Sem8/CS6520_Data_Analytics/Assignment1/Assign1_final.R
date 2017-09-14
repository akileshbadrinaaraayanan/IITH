# reading NIPS data
nips_data = read.csv('NIPS_1987-2015.csv', header=FALSE)

# remove rows having NA from data frame
nips_data = nips_data[complete.cases(nips_data),]

data_frame_1 = nips_data
data_frame_2 = nips_data

data_frame_2 = data_frame_2[2:5811,2:5812]

#Converting Dataframe to Matrix
df_mat = apply(as.matrix(data_frame_2), 2, as.numeric)

#Take the transpose 
data_frame_2 = as.data.frame(t(df_mat))
rownames(data_frame_2) = NULL

#Removing null columns as they are not allowed while doing PCA
data_frame_2 = data_frame_2[, colSums(abs(data_frame_2)) != 0]

predictor = data_frame_2[, 1:(dim(data_frame_2)[2]-1)]

#PCA determination using correlation matrix
pca = princomp(na.omit(predictor), cor=T) 
pc = pca$scores

#Negate the principal component scores
pc_1 = -1*pc[,1] 
pc_2 = -1*pc[,2] 

#Plotting PCs
X = cbind(pc_1, pc_2)
# using 4 clusters
clus = kmeans(X,4)
plot(pc_1, pc_2, col=clus$cluster, xlab = "PC 1", ylab = "PC 2")

print(paste('Principal Component Analysis is performed successfully'))


