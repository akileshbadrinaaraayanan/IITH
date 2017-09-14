@polly function kernel_ludcmp(A, b, x, y)
    n = size(A,1)

    for i = 1:n
        for j = 1:(i-1)
            w = A[i,j]
            for k = 1:(j-1)
                w -= A[i,k] * A[k,j]
            end
            A[i,j] = w / A[j,j]
        end
        for j = i:n
            w = A[i,j]
            for k = 1:(i-1)
                w -= A[i,k] * A[k,j]
            end
            A[i,j] = w
        end
    end

    for i = 1:n
        w = b[i]
        for j = 1:(i-1)
            w -= A[i,j] * y[j]
        end
        y[i] = w
    end

    for i = n:-1:1
        w = y[i]
        for j = (i+1):n
            w -= A[i,j] * x[j]
        end
        x[i] = w / A[i,i]
    end
end

let
    n = 2000

    A = zeros(Float32, n, n)
    b = zeros(Float32, n)
    x = zeros(Float32, n)
    y = zeros(Float32, n)

    fn = Float32(n)

    for i = 1:n
        x[i] = 0
        y[i] = 0
        b[i] = (i+1)/fn/2.0 + 4
    end

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

    SUITE["ludcmp"] = @benchmarkable kernel_ludcmp(A, b, x, y) setup = (A = copy($A); b = copy($b); x = copy($x); y = copy($y))
end
