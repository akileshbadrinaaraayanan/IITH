@polly function kernel_gesummv(alpha, beta, A, B, tmp, x, y)
    n = size(tmp,1)
    for i = 1:n
        tmp[i] = zero(eltype(tmp))
        y[i] = zero(eltype(tmp))
        for j = 1:n
            tmp[i] = A[i,j] * x[j] + tmp[i]
            y[i] = B[i,j] * x[j] + y[i]
        end
        y[i] = alpha * tmp[i] + beta * y[i]
    end
end

let
    n = 1300

    A = zeros(Float32, n, n)
    B = zeros(Float32, n, n)
    tmp = zeros(Float32, n)
    x = zeros(Float32, n)
    y = zeros(Float32, n)

    alpha = Float32(1.5)
    beta = Float32(1.2)
    for i = 1:n
        x[i] = (i % n) / n
        for j = 1:n
            A[i,j] = ((i*j+1) % n) / n
            B[i,j] = ((i*j+2) % n) / n
        end
    end

    SUITE["gesummv"] = @benchmarkable kernel_gesummv($alpha, $beta, A, B, tmp, x, y) setup = (A = copy($A); B = copy($B); tmp = copy($tmp); x = copy($x); y = copy($y))
end
