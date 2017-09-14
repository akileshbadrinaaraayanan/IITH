@polly function kernel_syr2k(alpha, beta, C, A, B)
    n,m = size(A)
    for i = 1:n
        for j = 1:i
            C[i,j] += beta
        end
        for k = 1:m, j =1:i
            C[i,j] += A[j,k] * alpha * B[i,k] + B[j,k] * alpha * A[i,k]
        end
    end
end

let
    m = 1000
    n = 1200

    C = zeros(Float32,n,n)
    A = zeros(Float32,n,m)
    B = zeros(Float32,n,m)

    alpha = Float32(1.5)
    beta = Float32(1.2)
    for i = 1:n, j = 1:m
        A[i,j] = ((i*j+1)%n) / n
        B[i,j] = ((i*j+2)%m) / m
    end
    for i = 1:n, j = 1:n
        C[i,j] = ((i*j+3)%n) / m
    end

    SUITE["syr2k"] = @benchmarkable kernel_syr2k($alpha, $beta, C, A, B) setup = (C = copy($C); A = copy($A); B = copy($B))
end
