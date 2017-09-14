@polly function kernel_jacobi_1d(tsteps, A, B)
    n = size(A,1)

    for t = 1:tsteps
        for i = 2:(n-1)
            B[i] = 0.33333 * (A[i-1] + A[i] + A[i + 1])
        end
        for i = 2:(n-1)
            A[i] = 0.33333 * (B[i-1] + B[i] + B[i + 1])
        end
    end
end

let
    tsteps = 500
    n = 2000

    A = zeros(Float32, n)
    B = zeros(Float32, n)

    for i = 1:n
        A[i] = (i+ 2) / n
        B[i] = (i+ 3) / n
    end

    SUITE["jacobi-1d"] = @benchmarkable kernel_jacobi_1d($tsteps, A, B) setup = (A = copy($A); B = copy($B))
end
