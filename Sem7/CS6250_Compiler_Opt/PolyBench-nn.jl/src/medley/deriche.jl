@polly function kernel_deriche(alpha, imgIn, imgOut, y1, y2)
    w,h = size(imgIn)

    k = (1.0-exp(-alpha))*(1.0-exp(-alpha))/(1.0+2.0*alpha*exp(-alpha)-exp(2.0*alpha))
    a1 = a5 = k
    a2 = a6 = k*exp(-alpha)*(alpha-1.0)
    a3 = a7 = k*exp(-alpha)*(alpha+1.0)
    a4 = a8 = -k*exp(-2.0*alpha)
    b1 = 2.0^(-alpha)
    b2 = -exp(-2.0*alpha)
    c1 = c2 = 1

    for i = 1:w
        ym1 = 0.0;
        ym2 = 0.0;
        xm1 = 0.0;
        for j = 1:h
            y1[i,j] = a1*imgIn[i,j] + a2*xm1 + b1*ym1 + b2*ym2
            xm1 = imgIn[i,j]
            ym2 = ym1
            ym1 = y1[i,j]
        end
    end

    for i = 1:w
        yp1 = 0.0
        yp2 = 0.0
        xp1 = 0.0
        xp2 = 0.0
        for j = h:-1:1
            y2[i,j] = a3*xp1 + a4*xp2 + b1*yp1 + b2*yp2
            xp2 = xp1
            xp1 = imgIn[i,j]
            yp2 = yp1
            yp1 = y2[i,j]
        end
    end

    for i = 1:w, j = 1:h
        imgOut[i,j] = c1 * (y1[i,j] + y2[i,j])
    end

    for j = 1:h
        tm1 = 0.0
        ym1 = 0.0
        ym2 = 0.0
        for i = 1:w
            y1[i,j] = a5*imgOut[i,j] + a6*tm1 + b1*ym1 + b2*ym2
            tm1 = imgOut[i,j]
            ym2 = ym1
            ym1 = y1[i,j]
        end
    end


    for j = 1:h
        tp1 = 0.0
        tp2 = 0.0
        yp1 = 0.0
        yp2 = 0.0
        for i = w:-1:1
            y2[i,j] = a7*tp1 + a8*tp2 + b1*yp1 + b2*yp2
            tp2 = tp1
            tp1 = imgOut[i,j]
            yp2 = yp1
            yp1 = y2[i,j]
        end
    end

    for i=1:w, j=1:h
        imgOut[i,j] = c2*(y1[i,j] + y2[i,j])
    end
end

let
    w = 4096
    h = 2160

    imgIn = zeros(Float32, w, h)
    imgOut = zeros(Float32, w, h)
    y1 = zeros(Float32, w, h)
    y2 = zeros(Float32, w, h)

    alpha = Float32(0.25) # parameter of the filter

    # input should be between 0 and 1 (grayscale image pixel)
    for i = 1:w, j = 1:h
        imgIn[i,j] = ((313*i+991*j)%65536) / 65535.0
    end

    SUITE["deriche"] = @benchmarkable kernel_deriche($alpha, imgIn, imgOut, y1, y2) setup = (imgIn = copy($imgIn); imgOut = copy($imgOut); y1 = copy($y1); y2 = copy($y2))
end
