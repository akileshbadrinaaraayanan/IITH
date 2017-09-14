@polly function kernel_doitgen(A,C4,sum)
    nr,nq,np = size(A)

    for r = 1:nr, q = 1:nq
        for p = 1:np
            sum[p] = zero(eltype(sum))
            for s = 1:np
                sum[p] += A[r,q,s] * C4[s,p]
            end
        end
        for p = 1:np
            A[r,q,p] = sum[p]
        end
    end
end

let
    nq = 140
    nr = 150
    np = 160

    A = zeros(Float32,nr,nq,np)
    sum = zeros(Float32,np)
    C4 = zeros(Float32,np,np)

    for i = 1:nr, j = 1:nq, k = 1:np
        A[i,j,k] = ((i*j + k)%np) / np
    end
    for i = 1:np, j = 1:np
        C4[i,j] = (i*j % np) / np
    end

    SUITE["doitgen"] = @benchmarkable kernel_doitgen(A, C4, sum) setup = (A = copy($A); C4 = copy($C4); sum = copy($sum))
end
