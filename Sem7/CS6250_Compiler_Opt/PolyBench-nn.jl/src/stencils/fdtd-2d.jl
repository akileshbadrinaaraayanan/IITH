@polly function kernel_fdtd_2d(ex, ey, hz, _fict_)
    nx,ny = size(ex)
    tmax = size(_fict_,1)

    for t = 1:tmax
        for j = 1:ny
            ey[1,j] = _fict_[t]
        end
        for i = 2:nx, j = 1:ny
            ey[i,j] = ey[i,j] - 0.5 * (hz[i,j] - hz[i-1,j])
        end
        for i = 1:nx, j = 2:ny
            ex[i,j] = ex[i,j] - 0.5 * (hz[i,j] - hz[i,j-1])
        end
        for i = 1:(nx - 1), j = 1:(ny - 1)
            hz[i,j] = hz[i,j] - 0.7 * (ex[i,j+1] - ex[i,j] + ey[i+1,j] - ey[i,j])
        end
    end
end

let
    tmax = 500
    nx = 1000
    ny = 1200

    ex = zeros(Float32,nx,ny)
    ey = zeros(Float32,nx,ny)
    hz = zeros(Float32,nx,ny)
    _fict_ = zeros(Float32,tmax)

    for i = 1:tmax
        _fict_[i] = i
    end
    for i = 1:nx, j = 1:ny
        ex[i,j] = (i*(j+1)) / nx
        ey[i,j] = (i*(j+2)) / ny
        hz[i,j] = (i*(j+3)) / nx
    end

    SUITE["fdtd-2d"] = @benchmarkable kernel_fdtd_2d(ex, ey, hz, _fict_) setup = (ex = copy($ex); ey = copy($ey); hz = copy($hz); _fict_ = copy($_fict_))
end
