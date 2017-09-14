@polly function kernel_cholesky(A)
    n = size(A,1)
    for i = 1:n
        # j < i
        for j = 1:(i-1)
            for k = 1:(j-1)
                A[k,j] -= A[i,k] * A[j,k]
            end
            A[i,j] /= A[j,j]
        end
        # i == j case
        for k = 1:(i-1)
            A[i,i] -= A[i,k] * A[i,k]
        end
        @fastmath A[i,i] = sqrt(A[i,i])
    end
end

let
    n = 2000

    A = zeros(Float32, n, n)

    for i = 1:n
        for j = 1:i
            A[i,j] = (-j % n) / n + one(eltype(A))
        end
        for j = i+1:n
            A[i,j] = zero(eltype(A))
        end
        A[i,i] = one(eltype(A))
    end

    # Make the matrix positive semi-definite.
    B = zeros(Float32, n, n)
    for r = 1:n, s = 1:n
        B[r,s] = zero(eltype(B))
    end
    for t = 1:n
        for r = 1:n, s = 1:n
            B[r,s] += A[r,t] * A[s,t]
        end
        for r = 1:n, s = 1:n
            A[r,s] = B[r,s]
        end
    end

    SUITE["cholesky"] = @benchmarkable kernel_cholesky(A) setup = (A = copy($A))
end
