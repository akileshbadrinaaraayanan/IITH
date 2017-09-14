@polly function kernel_durbin(r, y)
    n = size(r,1)
    z = zeros(eltype(r),n)
    y[1] = -r[1]
    beta = one(eltype(y))
    alpha = -r[1]

    for k = 2:n
        beta = (1 - alpha * alpha) * beta
        sum = zero(eltype(y))
        for i = 1:(k-1)
            sum += r[k-i] * y[i]
        end
        alpha = - (r[k] + sum)/beta

        for i = 1:(k-1)
            z[i] = y[i] + alpha * y[k-i]
        end
        for i = 1:(k-1)
            y[i] = z[i]
        end
        y[k] = alpha
    end
end

let
    n = 2000

    r = zeros(Float32, n)
    y = zeros(Float32, n)

    for i = 1:n
        r[i] = (n+1-i)
    end

    SUITE["durbin"] = @benchmarkable kernel_durbin(r, y) setup = (r = copy($r); y = copy($y))
end
