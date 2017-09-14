@polly function kernel_covariance(float_n, data, cov, mean)
    n,m = size(data)

    for j = 1:m
        mean[j] = zero(eltype(mean))
        for i = 1:n
            mean[j] += data[i,j]
        end
        mean[j] /= float_n
    end

    for i = 1:n, j = 1:m
        data[i,j] -= mean[j]
    end

    for i = 1:m, j = i:m
        cov[i,j] = zero(eltype(cov))
        for k = 1:n
            cov[i,j] += data[k,i] * data[k,j]
        end
        cov[i,j] /= (float_n - one(eltype(cov)))
        cov[j,i] = cov[i,j]
    end
end

let
    m = 1200
    n = 1400

    data = zeros(Float32,n,m)
    cov = zeros(Float32,m,m)
    mean = zeros(Float32,m)

    float_n = Float32(n)

    for i = 1:n, j = 1:m
        data[i,j] = (i*j) / m
    end

    SUITE["covariance"] = @benchmarkable kernel_covariance($float_n, data, cov, mean) setup = (data = copy($data); cov = copy($cov); mean = copy($mean))
end
