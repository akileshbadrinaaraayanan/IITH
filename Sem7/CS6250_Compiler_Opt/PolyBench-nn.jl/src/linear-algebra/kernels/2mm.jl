# D := alpha*A*B*C + beta*D
@polly function kernel_2mm(alpha, beta, tmp, A, B, C, D)
    ni,nk = size(A)
    nj,nl = size(C)
    for i = 1:ni, j = 1:nj
        tmp[i,j] = zero(eltype(tmp))
        for k = 1:nk
            tmp[i,j] += alpha * A[i,k] * B[k,j]
        end
    end

    for i = 1:ni, j = 1:nl
        D[i,j] *= beta
        for k = 1:nj
            D[i,j] += tmp[i,k] * C[k,j]
        end
    end
end

let
    ni = 800
    nj = 900
    nk = 1100
    nl = 1200

    tmp = zeros(Float32,ni,nj)
    A = zeros(Float32,ni,nk)
    B = zeros(Float32,nk,nj)
    C = zeros(Float32,nj,nl)
    D = zeros(Float32,ni,nl)

    alpha = Float32(1.5)
    beta = Float32(1.2)

    for i = 1:ni, j = 1:nk
        A[i,j] = ((i * j + 1) % ni) / ni
    end
    for i = 1:nk, j = 1:nj
        B[i,j] = ((i * j + 1) % nj) / nj
    end
    for i = 1:nj, j = 1:nl
        C[i,j] = ((i * (j + 3) + 1) % nl) / nl
    end
    for i = 1:ni, j = 1:nl
        D[i,j] = (i * (j + 2) % nk) / nk
    end

    SUITE["2mm"] = @benchmarkable kernel_2mm($alpha, $beta, tmp, A, B, C, D) setup = (tmp = copy($tmp); A = copy($A); B = copy($B); C = copy($C); D = copy($D))
end


