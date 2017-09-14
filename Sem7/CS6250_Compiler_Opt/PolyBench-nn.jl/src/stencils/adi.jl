@polly function kernel_adi(tsteps, u, v, p, q)
    n = size(u,1)

    DX = 1.0 / n
    DY = 1.0 / n
    DT = 1.0 / tsteps
    B1 = 2.0
    B2 = 1.0
    mul1 = B1 * DT / (DX * DX)
    mul2 = B2 * DT / (DY * DY)

    a = -mul1 /  2.0;
    b = 1.0 + mul1
    c = a
    d = -mul2 / 2.0
    _e = 1.0 + mul2
    f = d

    for t = 1:tsteps
        # Column Sweep
        for i = 2:(n-1)
            v[1,i] = 1.0
            p[i,1] = 0.0
            q[i,1] = v[1,i]
            for j = 2:(n-1)
                p[i,j] = -c / (a * p[i,j-1] + b)
                q[i,j] = (-d*u[j,i-1]+(1.0+2.0*d)*u[j,i] - f*u[j,i+1]-a*q[i,j-1])/(a*p[i,j-1]+b)
            end

            v[n,i] = 1.0
            for j = (n-1):-1:2
                v[j,i] = p[i,j] * v[j+1,i] + q[i,j]
            end
        end

        # Row Sweep
        for i = 2:(n-1)
            u[i,1] = 1.0
            p[i,1] = 0.0
            q[i,1] = u[i,1]
            for j = 2:(n-1)
                p[i,j] = -f / (d*p[i,j-1] + _e)
                q[i,j] = (-a*v[i-1,j]+(1.0+2.0*a)*v[i,j] - c*v[i+1,j]-d*q[i,j-1])/(d*p[i,j-1]+_e)
            end

            u[i,n] = 1.0
            for j = (n-1):-1:2
                u[i,j] = p[i,j] * u[i,j+1] + q[i,j]
            end
        end
    end
end

let
    tsteps = 500
    n = 1000

    u = zeros(Float32, n, n)
    v = zeros(Float32, n, n)
    p = zeros(Float32, n, n)
    q = zeros(Float32, n, n)

    for i = 1:n, j = 1:n
        u[i,j] =  (i + n-j) / n
    end

    SUITE["adi"] = @benchmarkable kernel_adi($tsteps, u, v, p, q) setup = (u = copy($u); v = copy($v); p = copy($p); q = copy($q))
end
