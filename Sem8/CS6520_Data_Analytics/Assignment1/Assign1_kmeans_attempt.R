## K-Means clustering attempt to solve this problem. This method did not work out well when compared to PCA approach

# reading NIPS data
nips_data = read.csv('NIPS_1987-2015.csv', header=FALSE)

# remove rows having NA from data frame
nips_data = nips_data[complete.cases(nips_data),]

data_frame_1 = nips_data
data_frame_2 = nips_data

# Consider only 600 attributes
data_frame_1 = data_frame_1[2:600,-1]

#Applied K-Means Clustering with 30 clusters
kMeans <- (kmeans(df_1, centers=30))
kMeans


