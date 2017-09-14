@polly function kernel_lu(A)
    n = size(A,1)
    for i = 1:n
        for j = 1:(i-1)
            for k = 1:(j-1)
                A[i,j] -= A[i,k] * A[k,j]
            end
            A[i,j] /= A[j,j]
        end
        for j = i:n, k = 1:(i-1)
            A[i,j] -= A[i,k] * A[k,j]
        end
    end
end

let
    n = 2000

    A = zeros(Float32, n, n)

    for i = 1:n
        for j = 1:i
            A[i,j] = (-j % n) / n + 1
        end
        for j = i+1:n
            A[i,j] = 0
        end
        A[i,i] = 1
    end

    # Make the matrix positive semi-definite.
    # not necessary for LU, but using same code as cholesky
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

    SUITE["lu"] = @benchmarkable kernel_lu(A) setup = (A = copy($A))
end
