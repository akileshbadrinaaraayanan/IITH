# CNN

function get_index(p, u, R, r)
	p*u + R - r
end

@polly function cnn_forward(inp_F, W, out_F,nu,nv)
	nn,nk,np,nq = size(out_F)
	nc,nh,nw = size(inp_F)[2:end]
	nr,ns = size(W)[3:end]

	for n=1:nn, k=1:nk, p=1:np, q=1:nq, c=1:nc, r=1:nr, s=1:ns
		out_F[n,k,p,q] += W[k,c,r,s] * inp_F[n,c,get_index(p,nu,nr,r),get_index(q,nv,ns,s)]
	end

end

@polly function cnn_backward(err_out, W, err_in, nu, nv)
	nn,nk,np,nq = size(err_out)
	nc,nh,nw = size(err_in)[2:end]
	nr,ns = size(W)[3:end]

	for n=1:nn, c=1:nc, h=1:nh, w=1:nw, k=1:nk, r=1:nr, s=1:ns, p=1:np, q=1:nq
		if (nu*p - (h - nr + r)==0) && (nv*q - (w - ns + s)==0)
			err_in[n,c,h,w] += W[k,c,r,s] * err_out[n,k,p,q]
		end
	end

end

let
	nn = 50
	nk = 40
	nc = 75
	nr = 6
	ns = 6
	nw = 50
	nh = 50
	nu = 5
	nv = 5
	np = div((nh - nr),nu) + 1
	nq = div((nw - ns),nv) + 1
	out_F = zeros(Float32,nn,nk,np,nq)
	W = zeros(Float32,nk,nc,nr,ns)
	inp_F = zeros(Float32,nn,nc,nh,nw)
	err_in = zeros(Float32,nn,nc,nh,nw)
	err_out = zeros(Float32,nn,nk,np,nq)

	#init_array
	
	for a=1:nn, b=1:nk, e=1:np, d=1:nq
		out_F[a,b,e,d] = Float32((a*b)%nn)
		err_out[a,b,e,d] = Float32((e*d)%nk)
	end

	for a=1:nk, b=1:nc, e=1:nr, d=1:ns
		W[a,b,e,d] = Float32(((a*b)%nc) / (10*nc))
	end

	for a=1:nn, b=1:nc, e=1:nh, d=1:nw
		inp_F[a,b,e,d] = Float32((a*b)%nc)
		err_in[a,b,e,d] = Float32((e*d)%nc)
	end
	
	if length(ARGS)==0 || ARGS[1]=="forward"
		print("Forward pass loaded")
		SUITE["cnn"] =@benchmarkable cnn_forward(inp_F, W, out_F, $nu, $nv) setup = (inp_F = copy($inp_F); W = copy($W); out_F = copy($out_F); err_out = copy($err_out); err_in = copy($err_in))
	else
		print("Backward pass loaded")
		SUITE["cnn"] =@benchmarkable cnn_backward(err_out, W, err_in, $nu, $nv) setup = (inp_F = copy($inp_F); W = copy($W); out_F = copy($out_F); err_out = copy($err_out); err_in = copy($err_in))
	end

end
