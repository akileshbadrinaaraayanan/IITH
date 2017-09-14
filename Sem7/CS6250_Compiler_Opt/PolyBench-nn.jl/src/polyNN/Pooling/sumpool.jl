#SUMPOOL

@polly function sumpool_forward(nn, nd, ih, iw, ow, oh, dh, dw, sh, sw, inp_F, out_F)

    for n=1:nn, d=1:nd, r=1:oh, c=1:ow
        val=Float32(0.0)
        for h=10*r+1:min(10*r+dh,ih), w=10*c:min(10*c+dw,iw)
            val += inp_F[n,d,h,w]
        end
        out_F[n,d,r,c] = val
    end
end

@polly function sumpool_backward(nn, nd, ih, iw, ow, oh, dh, dw, err_in, err_out)

    for n=1:nn, d=1:nd, r=1:oh, c=1:ow
        val=Float32(0.0)
        for h=10*r+1:min(10*r+dh,ih), w=10*c:min(10*c+dw,iw)
            err_in[n,d,h,w] += err_out[n,d,r,c]
        end
    end
end

let
    nn = 150
    nd = 120
    ih = 100
    iw = 100
    dh = 15
    dw = 15
    sh = 10
    sw = 10
    oh = div(ih - dh,sh) + 1
    ow = div(iw - dw,sw) + 1

    inp_F=zeros(Float32,nn,nd,ih,iw)
    out_F=zeros(Float32,nn,nd,oh,ow)
    err_in=zeros(Float32,nn,nd,ih,iw)
    err_out=zeros(Float32,nn,nd,oh,ow)

    #init_array
	
	for a=1:nn,b=1:nd,c=1:oh,d=1:ow
		out_F[a,b,c,d] = Float32((a*b + c*d) % nn)
		err_out[a,b,c,d] = Float32((a+b+c+d) % nn)
	end

	for a=1:nn,b=1:nd,c=1:iw,d=1:ih
		inp_F[a,b,c,d] = Float32((a*b + c*d) % nd)
		err_in[a,b,c,d] = Float32((a+b+c+d) % nd)
	end
	
	if length(ARGS)==0 || ARGS[1]=="forward"
		SUITE["sumpool"] = @benchmarkable sumpool_forward($nn,$nd,$ih,$iw,$ow,$oh,$dh,$dw,$sh,$sw,inp_F,out_F) setup = (inp_F=copy($inp_F); out_F=copy($out_F); err_in=copy($err_in); err_out=copy($err_out))
	else
		SUITE["sumpool"] = @benchmarkable sumpool_backward($nn,$nd,$ih,$iw,$ow,$oh,$dh,$dw, err_in, err_out)  setup = (inp_F=copy($inp_F); out_F=copy($out_F); err_in=copy($err_in); err_out=copy($err_out))
	end
end
