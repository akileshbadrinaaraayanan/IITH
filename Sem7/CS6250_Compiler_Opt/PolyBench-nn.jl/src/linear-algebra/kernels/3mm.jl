# G = (A*B) * (C*D)
@polly function kernel_3mm(E, A, B, F, C, D, G)
    ni,nk = size(A)
    nk,nj = size(B)
    nj,nm = size(C)
    nm,nl = size(D)

    # E := A*B
    for i = 1:ni, j = 1:nj
        E[i,j] = zero(eltype(E))
        for k = 1:nk
            E[i,j] += A[i,k] * B[k,j]
        end
    end

    # F := C*D
    for i = 1:nj, j = 1:nl
        F[i,j] = zero(eltype(F))
        for k = 1:nm
            F[i,j] += C[i,k] * D[k,j]
        end
    end

    # G := E*F
    for i = 1:ni, j = 1:nl
        G[i,j] = zero(eltype(G))
        for k = 1:nj
            G[i,j] += E[i,k] * F[k,j]
        end
    end
end

let
    ni = 800
    nj = 900
    nk = 1000
    nl = 1100
    nm = 1200

    E = zeros(Float32, ni, nj);
    A = zeros(Float32, ni, nk);
    B = zeros(Float32, nk, nj);
    F = zeros(Float32, nj, nl);
    C = zeros(Float32, nj, nm);
    D = zeros(Float32, nm, nl);
    G = zeros(Float32, ni, nl);

    for i = 1:ni, j = 1:nk
        A[i,j] = ((i*j+1) % ni) / (5*ni)
    end
    for i = 1:nk, j = 1:nj
        B[i,j] = ((i*(j+1)+2) % nj) / (5*nj)
    end
    for i = 1:nj, j = 1:nm
        C[i,j] = (i*(j+3) % nl) / (5*nl)
    end
    for i = 1:nm, j = 1:nl
        D[i,j] = ((i*(j+2)+2) % nk) / (5*nk)
    end

    SUITE["3mm"] = @benchmarkable kernel_3mm(E, A, B, F, C, D, G) setup = (E = copy($E); A = copy($A); B = copy($B); F = copy($F); C = copy($C); D = copy($D); G = copy($G))
end
