@polly function kernel_heat_3d(tsteps, A, B)
    n = size(A,1)

    for t = 1:tsteps
        for i = 2:(n-1), j = 2:(n-1), k = 2:(n-1)
            B[i,j,k] =   0.125 * (A[i+1,j,k] - 2.0 * A[i,j,k] + A[i-1,j,k]) +
                         0.125 * (A[i,j+1,k] - 2.0 * A[i,j,k] + A[i,j-1,k]) +
                         0.125 * (A[i,j,k+1] - 2.0 * A[i,j,k] + A[i,j,k-1]) +
                         A[i,j,k]
        end
        for i = 2:(n-1), j = 2:(n-1), k = 2:(n-1)
            A[i,j,k] =   0.125 * (B[i+1,j,k] - 2.0 * B[i,j,k] + B[i-1,j,k]) +
                         0.125 * (B[i,j+1,k] - 2.0 * B[i,j,k] + B[i,j-1,k]) +
                         0.125 * (B[i,j,k+1] - 2.0 * B[i,j,k] + B[i,j,k-1]) +
                         B[i,j,k]
        end
    end
end

let
    tsteps = 500
    n = 120

    A = zeros(Float32, n, n, n)
    B = zeros(Float32, n, n, n)

    for i = 1:n, j = 1:n, k = 1:n
        A[i,j,k] = B[i,j,k] = (i + j + (n-k))* 10 / (n)
    end

    SUITE["heat-3d"] = @benchmarkable kernel_heat_3d($tsteps, A, B) setup = (A = copy($A); B = copy($B))
end
