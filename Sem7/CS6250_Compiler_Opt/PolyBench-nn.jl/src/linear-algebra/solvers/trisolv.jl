@polly function kernel_trisolv(L, x, b)
    n = size(L,1)
    for i = 1:n
        x[i] = b[i]
        for j = 1:(i-1)
            x[i] -= L[i,j] * x[j]
        end
        x[i] = x[i] / L[i,i]
    end
end

let
    n = 2000

    L = zeros(Float32, n, n)
    x = zeros(Float32, n)
    b = zeros(Float32, n)

    for i = 1:n
        x[i] = -999
        b[i] = i
        for j = 1:i
            L[i,j] = (i+n-j+1)*2/n
        end
    end

    SUITE["trisolv"] = @benchmarkable kernel_trisolv(L, x, b) setup = (L = copy($L); x = copy($x); b = copy($b))
end
