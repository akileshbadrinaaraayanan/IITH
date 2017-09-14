@polly function kernel_symm(alpha, beta, C, A, B)
    m,n = size(C)
    for i = 1:m, j = 1:n
        temp2 = zero(eltype(C))
        for k = 1:(i-1)
            C[k,j] += alpha * B[i,j] * A[i,k]
            temp2 += B[k,j] * A[i,k]
        end
        C[i,j] = beta * C[i,j] + alpha * B[i,j] * A[i,i] + alpha * temp2
    end
end

let
    m = 1000
    n = 1200

    C = zeros(Float32,m,n)
    A = zeros(Float32,m,m)
    B = zeros(Float32,m,n)

    alpha = Float32(1.5)
    beta = Float32(1.2)
    for i = 1:m, j = 1:n
        C[i,j] = ((i+j) % 100) / m
        B[i,j] = ((n+i-j) % 100) / m
    end
    for i = 1:m
        for j = 1:i
            A[i,j] = ((i+j) % 100) / m
        end
        for j = i+1:m
            A[i,j] = -999 # regions of arrays that should not be used
        end
    end

    SUITE["symm"] = @benchmarkable kernel_symm($alpha, $beta, C, A, B) setup = (C = copy($C); A = copy($A); B = copy($B))
end
