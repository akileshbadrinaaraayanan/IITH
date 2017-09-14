@polly function kernel_jacobi_2d(tsteps, A, B)
    n = size(A,1)

    for t = 1:tsteps
        for i = 2:(n-1), j = 2:(n-1)
            B[i,j] = 0.2 * (A[i,j] + A[i,j-1] + A[i,1+j] + A[1+i,j] + A[i-1,j])
        end
        for i = 2:(n-1), j = 2:(n-1)
            A[i,j] = 0.2 * (B[i,j] + B[i,j-1] + B[i,1+j] + B[1+i,j] + B[i-1,j])
        end
    end
end

let
    tsteps = 500
    n = 1300

    A = zeros(Float32, n, n)
    B = zeros(Float32, n, n)

    for i = 1:n, j = 1:n
        A[i,j] = (i*(j+2) + 2) / n
        B[i,j] = (i*(j+3) + 3) / n
    end

    SUITE["jacobi-2d"] = @benchmarkable kernel_jacobi_2d($tsteps, A, B) setup = (A = copy($A); B = copy($B))
end
