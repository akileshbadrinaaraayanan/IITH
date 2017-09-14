@polly function kernel_gramschmidt(A,R,Q)
    m,n = size(A)
    for k = 1:n
        nrm = zero(eltype(A))
        for i = 1:m
            nrm += A[i,k] * A[i,k]
        end
        @fastmath R[k,k] = sqrt(nrm)
        for i = 1:m
            Q[i,k] = A[i,k] / R[k,k]
        end
        for j = (k+1):n
            R[k,j] = zero(eltype(R))
            for i = 1:m
                R[k,j] += Q[i,k] * A[i,j]
            end
            for i = 1:m
                A[i,j] = A[i,j] - Q[i,k] * R[k,j]
            end
        end
    end
end

let
    m = 1000
    n = 1200

    A = zeros(Float32,m,n)
    R = zeros(Float32,n,n)
    Q = zeros(Float32,m,n)

    for i = 1:m, j = 1:n
        A[i,j] = ((((i*j) % m) / m )*100) + 10
        Q[i,j] = zero(eltype(Q))
    end
    for i = 1:n, j = 1:n
        R[i,j] = zero(eltype(Q))
    end

    SUITE["gramschmidt"] = @benchmarkable kernel_gramschmidt(A, R, Q) setup = (A = copy($A); R = copy($R); Q = copy($Q))
end
