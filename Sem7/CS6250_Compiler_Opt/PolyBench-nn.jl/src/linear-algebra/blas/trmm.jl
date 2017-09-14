@polly function kernel_trmm(alpha, A, B)
    m,n = size(B)
    for i = 1:m, j = 1:n
        for k = (i+1):m
            B[i,j] += A[k,i] * B[k,j]
        end
        B[i,j] = alpha * B[i,j]
    end
end

let
    m = 1000
    n = 1200

    A = zeros(Float32,m,m)
    B = zeros(Float32,m,n)

    alpha = Float32(1.5)
    for i = 1:m
        for j = 1:(i-1)
            A[i,j] = ((i+j) % m)/m
        end
        A[i,i] = 1.0
        for j = 1:n
            B[i,j] = ((n+(i-j)) % n)/n
        end
    end

    SUITE["trmm"] = @benchmarkable kernel_trmm($alpha, A, B) setup = (A = copy($A); B = copy($B))
end
