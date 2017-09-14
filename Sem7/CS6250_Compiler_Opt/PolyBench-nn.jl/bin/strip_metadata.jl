# This is a simple and naive tool for stripping metadata information (dbg, tbaa,
# etc.) from LLVM assembly code as produced by Julia's LLVM code generator.
# Takes as input the name of an assembly (`.ll`) file and prints the
# transformed code to STDOUT.

llvm_ir = readall(ARGS[1])
# removes metadata from function signatures
llvm_ir = replace(llvm_ir, r"\) \#.*\{", "\) \{")
# removes calls to `llvm.dbg.declare` and `llvm.dbg.value`
llvm_ir = replace(llvm_ir, r".*call void .*llvm\.dbg.*\n", "")
# removes metadata which is passed to instructions
llvm_ir = replace(llvm_ir, r", \!dbg.*", "")
# removes tbaa nodes
llvm_ir = replace(llvm_ir, r", \!tbaa.*", "")
print(llvm_ir)
