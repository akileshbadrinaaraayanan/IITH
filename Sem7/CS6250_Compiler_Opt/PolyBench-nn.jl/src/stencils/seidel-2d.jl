@polly function kernel_seidel_2d(tsteps, A)
    n = size(A,1)

    for t = 1:tsteps, i = 2:(n-1), j = 2:(n-1)
        A[i,j] = (A[i-1,j-1] + A[i-1,j] + A[i-1,j+1] + A[i,j-1] + A[i,j] + A[i,j+1] + A[i+1,j-1] + A[i+1,j] + A[i+1,j+1])/9.0
    end
end

let
    tsteps = 500
    n = 2000

    A = zeros(Float32, n, n)

    for i = 1:n, j = 1:n
        A[i,j] = (i*(j+2) + 2) / n
    end

    SUITE["seidel-2d"] = @benchmarkable kernel_seidel_2d($tsteps, A) setup = (A = copy($A))
end
