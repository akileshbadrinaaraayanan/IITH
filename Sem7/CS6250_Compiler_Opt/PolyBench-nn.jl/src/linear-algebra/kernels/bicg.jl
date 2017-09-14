@polly function kernel_bicg(A,s,q,p,r)
    n,m = size(A)

    for i = 1:m
        s[i] = zero(eltype(s))
    end

    for i = 1:n
        q[i] = zero(eltype(q));
        for j = 1:m
            s[j] = s[j] + r[i] * A[i,j]
            q[i] = q[i] + A[i,j] * p[j]
        end
    end
end

let
    m = 1900
    n = 2100

    A = zeros(Float32, n, m)
    s = zeros(Float32, m)
    q = zeros(Float32, n)
    p = zeros(Float32, m)
    r = zeros(Float32, n)

    for i = 1:m
        p[i] = (i % m) / m
    end
    for i = 1:n
        r[i] = (i % n) / n
        for j = 1:m
            A[i,j] = (i*(j+1) % n)/n
        end
    end

    SUITE["bicg"] = @benchmarkable kernel_bicg(A, s, q, p, r) setup = (A = copy($A); s = copy($s); q = copy($q); p = copy($p); r = copy($r))
end
