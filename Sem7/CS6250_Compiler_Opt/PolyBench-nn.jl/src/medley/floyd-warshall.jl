@polly function kernel_floyd_warshall(path)
    for k = 1:size(path,1), i = 1:size(path,1), j = 1:size(path,1)
        path[i,j] = path[i,j] < path[i,k] + path[k,j] ? path[i,j] : path[i,k] + path[k,j]
    end
end

let
    n = 2800

    path = zeros(Float32, n, n)

    for i = 1:n, j = 1:n
        path[i,j] = i*j%7+1
        if ((i+j-2)%13 == 0 || (i+j-2)%7==0 || (i+j-2)%11 == 0)
            path[i,j] = 999
        end
    end

    SUITE["floyd-warshall"] = @benchmarkable kernel_floyd_warshall(path) setup = (path = copy($path))
end
