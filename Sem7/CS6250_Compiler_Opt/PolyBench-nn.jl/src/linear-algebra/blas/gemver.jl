@polly function kernel_gemver(alpha, beta, A, u1, v1, u2, v2, w, x, y, z)
    n = size(u1,1)
    for i = 1:n, j = 1:n
        A[i,j] = A[i,j] + u1[i] * v1[j] + u2[i] * v2[j]
    end

    for i = 1:n, j = 1:n
        x[i] = x[i] + beta * A[j,i] * y[j]
    end

    for i = 1:n
        x[i] = x[i] + z[i]
    end

    for i = 1:n, j = 1:n
        w[i] = w[i] + alpha * A[i,j] * x[j]
    end
end

let
    n = 2000

    A = zeros(Float32, n, n)
    u1 = zeros(Float32, n)
    v1 = zeros(Float32, n)
    u2 = zeros(Float32, n)
    v2 = zeros(Float32, n)
    w = zeros(Float32, n)
    x = zeros(Float32, n)
    y = zeros(Float32, n)
    z = zeros(Float32, n)

    alpha = Float32(1.5)
    beta = Float32(1.2)

    fn = Float32(n)

    for i = 1:n
        u1[i] = i;
        u2[i] = ((i+1)/fn)/2.0
        v1[i] = ((i+1)/fn)/4.0
        v2[i] = ((i+1)/fn)/6.0
        y[i] = ((i+1)/fn)/8.0
        z[i] = ((i+1)/fn)/9.0
        x[i] = 0.0
        w[i] = 0.0
        for j = 1:n
            A[i,j] = (i*j % n) / n
        end
    end

    SUITE["gemver"] = @benchmarkable kernel_gemver($alpha, $beta, A, u1, v1, u2, v2, w, x, y, z) setup = (A = copy($A); u1 = copy($u1); v1 = copy($v1); u2 = copy($u2); v2 = copy($v2); w = copy($w); x = copy($x); y = copy($y); z = copy($z))
end
