# PolyBench.jl

This package is a Julia port of the [PolyBench](http://web.cse.ohio-state.edu/~pouchet/software/polybench/) 4.1 benchmark suite used to evaluate the effectiveness of the polyhedral optimizer Polly in an Julia environment.

## Installation

You can install PolyBench.jl via executing the following command in your Julia REPL:

```
julia> Pkg.clone("https://github.com/MatthiasJReisinger/PolyBench.jl")
```

## Loading and running the benchmarks

Before the benchmarks can be run they have to be loaded:

```
julia> using PolyBench
julia> PolyBench.load!()
loading correlation...done (took 1.116060449) seconds)
loading covariance...done (took 1.039935161) seconds)
loading gemm...done (took 1.241035048) seconds)
loading gemver...done (took 1.358709486) seconds)
loading gesummv...done (took 1.214638159) seconds)
loading symm...done (took 1.33747356) seconds)
loading syr2k...done (took 1.226170809) seconds)
loading syrk...done (took 1.131447019) seconds)
loading trmm...done (took 1.18713334) seconds)
loading 2mm...done (took 1.411474273) seconds)
loading 3mm...done (took 1.422906759) seconds)
loading atax...done (took 1.108593501) seconds)
loading bicg...done (took 1.201980043) seconds)
loading doitgen...done (took 1.262327945) seconds)
loading mvt...done (took 1.19654039) seconds)
loading cholesky...done (took 157.365705549) seconds)
loading durbin...done (took 0.948544528) seconds)
loading gramschmidt...done (took 1.20634688) seconds)
loading lu...done (took 161.607451688) seconds)
loading ludcmp...done (took 158.630719482) seconds)
loading trisolv...done (took 1.083143289) seconds)
loading deriche...done (took 1.331020799) seconds)
loading floyd-warshall...done (took 1.158192308) seconds)
loading nussinov...done (took 1.135938128) seconds)
loading adi...done (took 1.118563244) seconds)
loading fdtd-2d...done (took 1.282940421) seconds)
loading heat-3d...done (took 1.246343404) seconds)
loading jacobi-1d...done (took 1.004145512) seconds)
loading jacobi-2d...done (took 1.210474674) seconds)
loading seidel-2d...done (took 1.334741505) seconds)
```

The benchmark suite can then be run via the following command:

```
julia> run(PolyBench.SUITE, verbose=true)
(1/30) benchmarking "deriche"...
done (took 21.685070286 seconds)
(2/30) benchmarking "2mm"...
done (took 11.237588999 seconds)
(3/30) benchmarking "3mm"...
done (took 13.830681214 seconds)
(4/30) benchmarking "jacobi-1d"...
done (took 7.25996884 seconds)
(5/30) benchmarking "durbin"...
done (took 8.213167735 seconds)
(6/30) benchmarking "lu"...
done (took 11.189646541 seconds)
...
```
