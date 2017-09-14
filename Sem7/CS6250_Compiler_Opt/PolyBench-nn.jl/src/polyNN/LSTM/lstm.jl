# LSTM

@polly function lstm_forward(nt,np,ns,nq,s_F,inp_F,c_F,U,W,IFOG)
	for t=1:nt
		fill!(IFOG,0.0)
		for r=1:ns
			for p=1:np,k=1:4
				IFOG[r,k] +=U[r,p,k] * inp_F[t,p]
			end
			if t>1
				for s=1:ns,k=1:4
					IFOG[r,k] +=W[r,s] * s_F[t-1,s]
				end
			end
		end
		if t>1
			for s=1:ns
				c_F[t,s] = c_F[t-1,s] * IFOG[s,2] + IFOG[s,1]*IFOG[s,4]
			end
		end
		for s=1:ns
			s_F[t,s] = c_F[t,s] * IFOG[s,3]
		end
	end
end

@polly function lstm_backward(nt,np,ns,nq,s_F,inp_F,c_F,U,W,IFOG,del_S,del_C,del_U,del_W,del_IFOG)

	for t=nt:-1:1
		for s=1:ns
			del_IFOG[s,3] = c_F[t,s] * del_S[t,s]
		end
		for s=1:ns
			del_C[t,s] += IFOG[s,3] * del_S[t,s]
		end
		if t>1
			for s=1:ns
				del_IFOG[s,2] = c_F[t-1,s] * del_C[t,s]
				del_C[t-1,s] = IFOG[s,2] * del_C[t,s]
			end
		end
		for s=1:ns
			del_IFOG[s,1] = IFOG[s,4] * del_C[t,s]
			del_IFOG[s,4] = IFOG[s,1] * del_C[t,s]
		end

		for s=1:ns
			for p=1:np
				for k=1:4
					del_U[s,p,k] = inp_F[t,p] * IFOG[s,k]
				end
			end
		end

		if t>1
			for s=1:ns
				for r=1:ns
					for k=1:4
						del_W[s,r,k] = s_F[t-1,r] * IFOG[s,k]
					end
				end
			end
		end

		if t>1
			for s=1:ns
				for r=1:ns
					for k=1:4
						del_S[t-1,r] += W[r,s,k] * del_IFOG[s,k]
					end
				end
			end
		end
	end
end #function end
			
let
	nt = 5
	np = 700
	nq = 600
	ns = 650

	s_F = zeros(Float32,nt,ns)
	inp_F = zeros(Float32,nt,np)
	c_F = zeros(Float32,nt,ns)
	U = zeros(Float32,ns,np,4)
	W = zeros(Float32,ns,ns,4)
	IFOG = zeros(Float32,ns,4)
	del_S = zeros(Float32,nt,ns)
	del_C = zeros(Float32,nt,ns)
	del_U = zeros(Float32,ns,np,4)
	del_W = zeros(Float32,ns,ns,4)
	del_IFOG = zeros(Float32,ns,4)

	#init array

	for a=1:nt, b=1:ns
		s_F[a,b] = Float32((a*b) % nt)
		del_S[a,b] = Float32((a*(b+2)) % ns)
		del_C[a,b] = Float32(((a+2)*(b+5)) % nt)
		c_F[a,b] = Float32((a*b) % ns)
	end

	for a=1:ns, b=1:np, c=1:4
		U[a,b,c] = Float32((a*(b+c)) % np)
		del_U[a,b,c] = Float32(((a+c)*b) % ns)
	end

	for a=1:ns,b=1:ns,c=1:4
		W[a,b,c] = Float32(((a+1)*(b+c)) % ns)
		del_W[a,b,c] = Float32(((a+c)*(b+1)) % nt)
	end

	for b=1:ns,c=1:4
		IFOG[b,c] = (b+2*c+1) / nt
	end
	
	for a=1:nt,b=1:np
		inp_F[a,b] = ((a*b) % np)
	end

	if (length(ARGS)==0) || ARGS[1]=="forward"
		SUITE["lstm"]=@benchmarkable lstm_forward($nt,$np,$ns,$nq,s_F,inp_F,c_F,U,W,IFOG) setup = (s_F=copy($s_F); inp_F=copy($inp_F); c_F=copy($c_F); U=copy($U); W=copy($W); IFOG=copy($IFOG))
	else
		SUITE["lstm"]=@benchmarkable lstm_backward($nt,$np,$ns,$nq,s_F,inp_F,c_F,U,W,IFOG,del_S,del_C,del_U,del_W,del_IFOG) setup = (s_F=copy($s_F); inp_F=copy($inp_F); c_F=copy($c_F); U=copy($U); W=copy($W); IFOG=copy($IFOG))
	end
end
