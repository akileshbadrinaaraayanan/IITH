# RNN:

@polly function rnn_forward(nt,np,ns,nq,out_F,s_F,inp_F,U,W,V)

	for t=1:nt
		for r=1:ns
			for p=1:np
				s_F[t,r] += U[r,p] * inp_F[t,p]
			end

			if t>1
				for s=1:ns
					s_F[t,r] += W[r,s] * s_F[t-1,s]
				end
			end
		end

		for q=1:nq
			for r=1:ns
				out_F[t,q] +=V[q,r] * s_F[t,r]
			end
		end
	end
end

@polly function rnn_backward(nt,np,ns,nq,bt,inp_F,s_F,W,V,err_out,del_U,del_W,del_V,del_TA,del_TB)

	for t=nt:-1:1
		for q=1:nq, s=1:ns
				del_V[q,s] = err_out[t,q] * s_F[t,s]
		end

		for s=1:ns
			del_TA[s] = Float32(0.0)
			for q=1:nq
				del_TA[s] += V[q,s] * err_out[t,q]
			end
		end

		for step=t+1:-1:max(0,t-bt)
			if step > 1
				for r=1:nr, s=1:ns
					del_W[r,s] +=del_TA[r] * s_F[step-1,s]
				end
			end

			for s=1:ns, p=1:np
				del_U[s,p] += del_TA[s] * inp_F[step,p]
			end

			for r=1:ns
				del_TB[r] = Float32(0.0)
				for s=1:ns
					del_TB[r] +=del_TA[s] * W[s,r]
				end
			end

			for r=1:ns
				del_TA[r] = del_TB[r]
			end
		end # step
	end # t
end

let
	nt = 100
	np = 200
	nq = 250
	ns = 310
	bt = 3

	out_F = zeros(Float32,nt,nq)
	s_F = zeros(Float32,nt,ns)
	inp_F = zeros(Float32,nt,np)
	U = zeros(Float32,ns,np)
	W = zeros(Float32,ns,ns)
	V = zeros(Float32,nq,ns)
	err_out = zeros(Float32,nt,nq)
	del_U = zeros(Float32,ns,np)
	del_W = zeros(Float32,ns,ns)
	del_V = zeros(Float32,nq,ns)
	del_TA = zeros(Float32,ns)
	del_TB = zeros(Float32,ns)

	#init_array
	for a=1:nt, b=1:nq
		out_F[a,b] = Float32((a*b)%nq) / nq
		err_out[a,b] = Float32((a*(b+1))%nq) / nq
	end

	for a=1:nt, b=1:ns
		s_F[a,b] = Float32((a*b)%nt) / nt
	end

	for a=1:nt, b=1:np
		inp_F[a,b] = Float32((a*b)%np) / np
	end

	for a=1:ns
		for b=1:np
			U[a,b] = Float32((a*b)%nt) / nt
			del_U[a,b] = Float32(((a+1)*(b+1))%nt) / nt
		end
		for b=1:ns
			W[a,b] = Float32((a*b)%ns) / ns
			del_W[a,b] = Float32(((a+1)*(b+1))%nt) / nt
		end
	end

	for a=1:nq, b=1:ns
		V[a,b] = Float32(((a+1)*b)%nq) / nq
		del_V[a,b] = Float32(((a+1)*(b+1))%nt) / nt
	end

	if length(ARGS)==0 || ARGS[1]=="forward"
		print("Forward pass loaded")
		SUITE["rnn"] = @benchmarkable rnn_forward($nt,$np,$ns,$nq, out_F, s_F, inp_F, U, W, V) setup = (inp_F=copy($inp_F); out_F=copy($out_F); s_F=copy($s_F); U = copy($U); W = copy($W); V = copy($V); err_out=copy($err_out); del_U=copy($del_U); del_W=copy($del_W); del_V=copy($del_V); del_TA=copy($del_TA); del_TB=copy($del_TB))
	else
		print("Backward pass loaded")
		SUITE["rnn"] = @benchmarkable rnn_backward($nt,$np,$ns,$nq,$bt, inp_F, s_F, W, V, del_U, del_W, del_V, del_TA, del_TB) setup = (inp_F=copy($inp_F); out_F=copy($out_F); s_F=copy($s_F); U = copy($U); W = copy($W); V = copy($V); err_out=copy($err_out); del_U=copy($del_U); del_W=copy($del_W); del_V=copy($del_V); del_TA=copy($del_TA); del_TB=copy($del_TB))
	end
end

