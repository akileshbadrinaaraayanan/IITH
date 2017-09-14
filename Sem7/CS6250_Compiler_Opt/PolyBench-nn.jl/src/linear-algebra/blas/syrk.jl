@polly function kernel_syrk(alpha, beta, C, A)
    n,m = size(A)
    for i = 1:n
        for j = 1:i
            C[i,j] *= beta
        end
        for k = 1:m, j = 1:i
            C[i,j] += alpha * A[i,k] * A[j,k]
        end
    end
end

let
    m = 1000
    n = 1200

    C = zeros(Float32,n,n)
    A = zeros(Float32,n,m)


    alpha = Float32(1.5)
    beta = Float32(1.2)
    for i = 1:n, j = 1:m
        A[i,j] = ((i*j+1)%n) / n
    end
    for i = 1:n, j = 1:n
        C[i,j] = ((i*j+2)%m) / m
    end

    SUITE["syrk"] = @benchmarkable kernel_syrk($alpha, $beta, C, A) setup = (C = copy($C); A = copy($A))
end
