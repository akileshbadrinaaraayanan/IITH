@polly function kernel_atax(A, x, y, tmp)
    m,n = size(A)

    for i = 1:n
        y[i] = 0
    end

    for i = 1:m
        tmp[i] = zero(eltype(tmp))

        for j = 1:n
            tmp[i] = tmp[i] + A[i,j] * x[j]
        end

        for j = 1:n
            y[j] = y[j] + A[i,j] * tmp[i]
        end
    end
end

let
    m = 1900
    n = 2100

    A = zeros(Float32, m, n)
    x = zeros(Float32, n)
    y = zeros(Float32, n)
    tmp = zeros(Float32, m)

    for i = 1:n
        x[i] = 1 + (i / n)
    end
    for i = 1:m, j = 1:n
        A[i,j] = ((i+j) % n) / (5*m)
    end

    SUITE["atax"] = @benchmarkable kernel_atax(A, x, y, tmp) setup = (A = copy($A); x = copy($x); y = copy($y); tmp = copy($tmp))
end
