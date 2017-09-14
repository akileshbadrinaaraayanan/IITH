@polly function kernel_mvt(x1, x2, y1, y2, A)
    n = size(x1,1)
    for i = 1:n, j = 1:n
        x1[i] = x1[i] + A[i,j] * y1[j]
    end
    for i = 1:n, j = 1:n
        x2[i] = x2[i] + A[j,i] * y2[j]
    end
end

let
    n = 2000

    A = zeros(Float32, n, n)
    x1 = zeros(Float32, n)
    x2 = zeros(Float32, n)
    y1 = zeros(Float32, n)
    y2 = zeros(Float32, n)

    for i = 1:n
        x1[i] = (i % n) / n
        x2[i] = ((i + 1) % n) / n
        y1[i] = ((i + 3) % n) / n
        y2[i] = ((i + 4) % n) / n
        for j = 1:n
            A[i,j] = (i*j % n) / n
        end
    end

    SUITE["mvt"] = @benchmarkable kernel_mvt(x1, x2, y1, y2, A) setup = (x1 = copy($x1); x2 = copy($x2); y1 = copy($y1); y2 = copy($y2); A = copy($A))
end
