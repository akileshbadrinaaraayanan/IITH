@polly function kernel_nussinov(seq, table)
    n = size(seq,1)

    for i = n:-1:1, j = i+1:n
        if (j-1) >= 1
            table[i,j] = max(table[i,j], table[i,j-1])
        end

        if (i+1) <= n
            table[i,j] = max(table[i,j], table[i+1,j])
        end

        if ((j-1) >= 1) & ((i+1) <= n)
            # don't allow adjacent elements to bond */
            if (i < j-1)
                table[i,j] = max(table[i,j], table[i+1,j-1] + (seq[i] + seq[j] == 3 ? 1 : 0))
            else
                table[i,j] = max(table[i,j], table[i+1,j-1])
            end
        end

        for k = (i+1):(j-1)
            table[i,j] = max(table[i,j], table[i,k] + table[k+1,j])
        end
    end
end

let
    n = 2500

    seq = zeros(Float32, n)
    table = zeros(Float32, n, n)

    # base is AGCT/0..3
    for i = 1:n
        seq[i] = ((i+1)%4)
    end

    for i = 1:n, j = 1:n
        table[i,j] = 0
    end

    SUITE["nussinov"] = @benchmarkable kernel_nussinov(seq, table) setup = (seq = copy($seq); table = copy($table))
end
