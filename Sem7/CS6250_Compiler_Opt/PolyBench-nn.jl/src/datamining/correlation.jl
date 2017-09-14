@polly function kernel_correlation(float_n, data, corr, mean, stddev)
    n,m = size(data)
    eps = 0.1;

    for j = 1:m
        mean[j] = zero(eltype(mean))
        for i = 1:n
            mean[j] += data[i,j]
        end
        mean[j] /= float_n
    end

    for j = 1:m
        stddev[j] = zero(eltype(stddev))
        for i = 1:n
            stddev[j] += (data[i,j] - mean[j]) * (data[i,j] - mean[j])
        end
        stddev[j] /= float_n
        @fastmath stddev[j] = sqrt(stddev[j])
        stddev[j] = stddev[j] <= eps ? one(eltype(stddev)) : stddev[j]
    end

    # Center and reduce the column vectors.
    for i = 1:n, j = 1:m
        data[i,j] -= mean[j]
        @fastmath data[i,j] /= sqrt(float_n) * stddev[j]
    end

    # Calculate the m * m correlation matrix.
    for i = 1:(m-1)
        corr[i,i] = one(eltype(corr))
        for j = (i+1):m
            corr[i,j] = one(eltype(corr))
            for k = 1:n
                corr[i,j] += (data[k,i] * data[k,j])
            end
            corr[j,i] = corr[i,j]
        end
    end

    corr[m-1,m-1] = one(eltype(corr))
end

let
    m = 1200
    n = 1400

    data = zeros(Float32,n,m)
    corr = zeros(Float32,m,m)
    mean = zeros(Float32,m)
    stddev = zeros(Float32,m)

    float_n = Float32(n);

    for i = 1:n, j = 1:m
        data[i,j] = (i*j)/m + i
    end

    SUITE["correlation"] = @benchmarkable kernel_correlation($float_n, data, corr, mean, stddev) setup=(data=copy($data); corr=copy($corr); mean=copy($mean); stddev=copy($stddev))
end
