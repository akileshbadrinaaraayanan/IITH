@polly function kernel_gemm(alpha, beta, C, A, B)
    ni,nk = size(A)
    nk,nj = size(B)
    for i = 1:ni
        for j = 1:nj
            C[i,j] *= beta
        end
        for k = 1:nk, j = 1:nj
            C[i,j] += alpha * A[i,k] * B[k,j]
        end
    end
end

let
    ni = 1000
    nj = 1100
    nk = 1200

    C = zeros(Float32,ni,nj)
    A = zeros(Float32,ni,nk)
    B = zeros(Float32,nk,nj)

    alpha = Float32(1.5)
    beta = Float32(1.2)
    for i = 1:ni, j = 1:nj
        C[i,j] = ((i*j+1) % ni) / ni
    end
    for i = 1:ni, j = 1:nk
        A[i,j] = (i*(j+1) % nk) / nk
    end
    for i = 1:nk, j = 1:nj
        B[i,j] = (i*(j+2) % nj) / nj
    end

    SUITE["gemm"] = @benchmarkable kernel_gemm($alpha, $beta, C, A, B) setup = (C = copy($C); A = copy($A); B = copy($B))
end
