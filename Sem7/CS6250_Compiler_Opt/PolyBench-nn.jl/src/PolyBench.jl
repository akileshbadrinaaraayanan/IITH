module PolyBench

using BenchmarkTools

const SUITE = BenchmarkGroup()
const BENCH_DIRS = ["datamining",
                    "polyNN/CNN",
                    "polyNN/RNN",
                    "polyNN/LSTM",
					"polyNN/Pooling",
                    "linear-algebra/blas",
                    "linear-algebra/kernels",
                    "linear-algebra/solvers",
                    "medley",
                    "stencils"]

function load!(bench_dir::AbstractString)
    print("loading $bench_dir...")
    bench_dir_path = joinpath(dirname(@__FILE__), bench_dir)
    for bench_file in filter(x -> endswith(x, "jl"), readdir(bench_dir_path))
        bench_file_path = joinpath(bench_dir_path, bench_file)
        bench_name = bench_file[1:end-3]
        print("loading $bench_name...")
        tic()
        eval(PolyBench, :(include($bench_file_path)))
        load_time = toq()
        println("done (took $load_time) seconds)")
    end
    return SUITE
end

function loadall!()
    for bench_dir in BENCH_DIRS
        print("loading $bench_dir...")
        bench_dir_path = joinpath(dirname(@__FILE__), bench_dir)
        for bench_file in filter(x -> endswith(x, "jl"), readdir(bench_dir_path))
            bench_file_path = joinpath(bench_dir_path, bench_file)
            bench_name = bench_file[1:end-3]
            print("loading $bench_name...")
            tic()
            eval(PolyBench, :(include($bench_file_path)))
            load_time = toq()
            println("done (took $load_time) seconds)")
        end
    end
    return SUITE
end

end # module
