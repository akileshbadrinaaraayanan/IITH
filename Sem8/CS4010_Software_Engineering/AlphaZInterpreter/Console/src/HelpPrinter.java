/**
 *  <h1> HelpPrinter </h1>
 *  Prints help for every command.
 * @author Hrishikesh Vaidya, Surya Teja Chavali, Akilesh B
 *
 */
public final class HelpPrinter {

	/**
	 * <h2> printHelp </h2>
	 * The only method in HelpPrinter. Prints help relevant to AlphaZ function.
	 * @param funcname - Name of the function.
	 */
	public void printHelp(String funcname) {
		System.out.println(funcname);
		System.out.println();
		switch(funcname){
		case "ASave":
			System.out.println("Description: Outputs the program to the specified file with array syntax.When filename is not specified, saves as \"original_filename-ASave.ab\".");
			System.out.println("Usage: ASave(Program prog, String filename)");
			break;
		case "ASaveSystem":
			System.out.println("Description: Outputs the result of AShow to the specified file.When filename is not specified, saves as \"original_filename-ASave.ab\".");
			System.out.println("Usage: ASaveSystem(Program prog, String system, String filename)");
			break;
		case "AShow":
			System.out.println("Description: Prints out the program using array notation.Prints out the entire program when target system is unspecified.");
			System.out.println("Usage: AShow(Program prog, String system)");
			break;
		case "Save":
			System.out.println("Description: Saves the output of show to the specified file.When output filename is unspecified, saves to \"original_filename-Show.ab\".");
			System.out.println("Usage: Save(Program prog, String filename)");
			break;
		case "SaveSystem":
			System.out.println("Description: Saves the output of show to the specified file.When output filename is unspecified, saves to \"original_filename-Show.ab\".");
			System.out.println("Usage: SaveSystem(Program prog, String system, String filename)");
			break;
		case "Show":
			System.out.println("Description: Pretty prints the program using alpha-purist syntax.Prints out the entire program when target system is unspecified.");
			System.out.println("Usage: Show(Program prog, String system)");
			break;
		case "Normalize":
			System.out.println("Description: Normalizes the program. Normalization rules are described in more detail at: url{http://www.cs.colostate.edu/AlphaZ/wiki/doku.php?id=normalize}.");
			System.out.println("Usage: Normalize(Program prog)");
			break;
		case "PrintAST":
			System.out.println("Description: Prints out the AST of the program.Prints out the entire program when target system is unspecified.");
			System.out.println("Usage: PrintAST(Program prog, String system)");
			break;
		case "ReadAlphabets":
			System.out.println("Description: Parses the given alphabets program and returns a Program object.");
			System.out.println("Usage: ReadAlphabets(String file)");
			break;
		case "RenameSystem":
			System.out.println("Description: Renames an AffineSystem.");
			System.out.println("Usage: RenameSystem(Program prog, String system, String newSystemName)");
			break;
		case "RenameVariable":
			System.out.println("Description: Renames a variable.");
			System.out.println("Usage: RenameVariable(Program prog, String system, String varName, String newVarName)");
			break;
		case "RemoveUnusedVariables":
			System.out.println("Description: Removes unused variables and equations. Unused variables are variables that is not used by the definition of output.");
			System.out.println("Usage: RemoveUnusedVariables(Program prog, String system)");
			break;
		case "PermutationCaseReduce":
			System.out.println("Description: Transformations involving reduce expressions.");
			System.out.println("Usage: PermutationCaseReduce()");
			break;
		case "ReductionDecomposition":
			System.out.println("Description: Transformations involving reduce expressions.");
			System.out.println("Usage: ReductionDecomposition()");
			break;
		case "SimplifyingReduction":
			System.out.println("Description: Transformations involving reduce expressions.");
			System.out.println("Usage: SimplifyingReduction()");
			break;
		case "NormalizeReduction":
			System.out.println("Description: Transformations involving reduce expressions.");
			System.out.println("Usage: NormalizeReduction()");
			break;
		case "FactorOutFromReduction":
			System.out.println("Description: Transformations involving reduce expressions.");
			System.out.println("Usage: FactorOutFromReduction()");
			break;
		case "SplitReductionBody":
			System.out.println("Description: Transformations involving reduce expressions.");
			System.out.println("Usage: SplitReductionBody()");
			break;
		case "TransformReductionBody":
			System.out.println("Description: Transformations involving reduce expressions.");
			System.out.println("Usage: TransformReductionBody()");
			break;
		case "SerializeReduction":
			System.out.println("Description: Transformations involving reduce expressions.");
			System.out.println("Usage: SerializeReduction()");
			break;
		case "MergeReductions":
			System.out.println("Description: Transformations involving reduce expressions.");
			System.out.println("Usage: MergeReductions()");
			break;
		case "ReductionComposition":
			System.out.println("Description: Transformations involving reduce expressions.");
			System.out.println("Usage: ReductionComposition()");
			break;
		case "monoparametricTiling_noOutlining":
			System.out.println("Description: Commands which parametrize the monoparametric tiling transformation, plus the transformation itself");
			System.out.println("Usage: monoparametricTiling_noOutlining()");
			break;
		case "monoparametricTiling_Outlining_noSubsystem":
			System.out.println("Description: Commands which parametrize the monoparametric tiling transformation, plus the transformation itself");
			System.out.println("Usage: monoparametricTiling_Outlining_noSubsystem()");
			break;
		case "monoparametricTiling_Outlining":
			System.out.println("Description: Commands which parametrize the monoparametric tiling transformation, plus the transformation itself");
			System.out.println("Usage: monoparametricTiling_Outlining()");
			break;
		case "setRatio":
			System.out.println("Description: Commands which parametrize the monoparametric tiling transformation, plus the transformation itself");
			System.out.println("Usage: setRatio()");
			break;
		case "setMinParamValues":
			System.out.println("Description: Commands which parametrize the monoparametric tiling transformation, plus the transformation itself");
			System.out.println("Usage: setMinParamValues()");
			break;
		case "setTileGroup":
			System.out.println("Description: Commands which parametrize the monoparametric tiling transformation, plus the transformation itself");
			System.out.println("Usage: setTileGroup()");
			break;
		case "setCoBPreprocess":
			System.out.println("Description: Commands which parametrize the monoparametric tiling transformation, plus the transformation itself");
			System.out.println("Usage: setCoBPreprocess()");
			break;
		case "CoB":
			System.out.println("Description: Change of Basis transforms the domain of a variable to the image by the given function, while adding necessary dependence expressions to maintain the original semantics of the program. Details can be found at url{http://www.cs.colostate.edu/AlphaZ/wiki/doku.php?id=change_of_basis}.");
			System.out.println("Usage: CoB(Program prog, String systemName, String varName, AffineFunction function)");
			break;
		case "ForceCoB":
			System.out.println("Description: Force the change of basis even if the CoB is for input/output of a system.");
			System.out.println("Usage: ForceCoB(Program prog, String systemName, String targetName, AffineFunction function)");
			break;
		case "Split":
			System.out.println("Description: Splits a variable into two, where the domain of two variables are disjoint and union of them is equivalent to the original domain of the variable.If newName is not given, it creates a variable with prefix \"_split\" appended to the original name.");
			System.out.println("Usage: Split(Program prog, String systemName, String varName, String newName, String sepDomain)");
			break;
		case "Merge":
			System.out.println("Description: Merge two variables (of name var1Name and var2Name) into a single one.These variables must both be locals and must have disjoint domains. The name of the merge of these two variables is \"newName\".");
			System.out.println("Usage: Merge(Program prog, String systemName, String var1Name, String var2Name, String newName)");
			break;
		case "Inline":
			System.out.println("Description: Inlines the nth(number) reference to inlineEq in the definition of targetEq once. If the number is not specified, all references to inlineEq are inlined.");
			System.out.println("Usage: Inline(Program prog, String systemName, String targetEq, String inlineEq, int number)");
			break;
		case "Simplify":
			System.out.println("Description: Simplifies the program in multiple ways. The domains are simplified by simplifyInContext (a.k.a. gist). When the domain of a reduction body is a single point, reduction is removed.");
			System.out.println("Usage: Simplify(Program prog, String system)");
			break;
		case "createFreeScheduler":
			System.out.println("Description: Takes an alphabetes program and converts it to a program that computes the fastest possible schedule (free shcedule).This transformation DOES NOT preserve the original semantics, but instead creates a new Program object.Implemented by Alex Klein as a class project in cs560@spring11");
			System.out.println("Usage: createFreeScheduler(Program prog, String newProgPrefix)");
			break;
		case "SplitUnion":
			System.out.println("Description: Replace an expression that has unions of polyhedra as its context domain with a case expression that splits the union into multiple disjiont polyhedra.");
			System.out.println("Usage: SplitUnion(Program prog, String nodeID)");
			break;
		case "ApplySTMap":
			System.out.println("Description: Applies sequence of CoBs using STMap given for each variable.This is identical to the pre-processing step of ScheduledC code generator, where all variables are mapped to a common space so that ordering of iterations can be defined.Assumes correct schedule [TODO:run verifier before applying the transformation once the verifier is fixed]All STMaps and MemoryMaps given for the system will also be transformed appropriately.");
			System.out.println("Usage: ApplySTMap(Program prog, String systemName)");
			break;
		case "UniformizeInContext":
			System.out.println("Description: Attempts to uniformize all dependencies when possible.");
			System.out.println("Usage: UniformizeInContext(Program prog, String system, int excludeInputs)");
			break;
		case "InlineForce":
			System.out.println("Description: Force the inline action even if there is a self loop dependence");
			System.out.println("Usage: InlineForce(Program prog, String systemName, String targetEq, String inlineEq, int number)");
			break;
		case "InlineAll":
			System.out.println("Description: Inline all the possible equation in a system when there is no self loop dependence");
			System.out.println("Usage: InlineAll(Program prog, String systemName, String inlineEq)");
			break;
		case "InlineAllForce":
			System.out.println("Description: Inline all the possible equations in a system even with self loop dependence once");
			System.out.println("Usage: InlineAllForce(Program prog, String systemName, String inlineEq)");
			break;
		case "InlineSubSystem":
			System.out.println("Description: Inline the subsystem used by the UseEquation specified by nodeID");
			System.out.println("Usage: InlineSubSystem(Program prog, String systemName, String label)");
			break;
		case "OutlineSubSystem":
			System.out.println("Description: Outline a list of equations of a given system. These equation are defined through a list of variable (\"listEquations\") that corresponds to the variable of a StandardEquation or to the output of a UseEquation.[V1] The created use equation will have no extension domain, and only variable expressions as inputs.");
			System.out.println("Usage: OutlineSubSystem(Program prog, String system, String listEquations)");
			break;
		case "AddLocal":
			System.out.println("Description: Adds a local variable (of name specified by \"nameLocal\") for the expression \"expr\" and replace each occurrence of this expression in the considered system.\"expr\" can be specified through a expression of the system (located in \"nodeID\"), or through an external expression \"expr\" whose list of indexes is \"lInds\".");
			System.out.println("Usage: AddLocal(Program prog, String system, String nameLocal, String nodeID)");
			break;
		case "AddLocalUnique":
			System.out.println("Description: Replace only the expression (specified by the location \"nodeID\") by a new local variable (of name \"nameLocal\").");
			System.out.println("Usage: AddLocalUnique(Program prog, String system, String nameLocal, String nodeID)");
			break;
		case "DetectReductions":
			System.out.println("Description: Detects simple reductions in the program and exposes as reduce expressions.");
			System.out.println("Usage: DetectReductions(Program prog, String system)");
			break;
		case "reduceDimVariable":
			System.out.println("Description: Use the equalities of the domain of the specified variable in order to reduce its number of dimension.Perform the corresponding change of basis and return the affine function used in this CoB.");
			System.out.println("Usage: reduceDimVariable(Program prog, String syst, String varName)");
			break;
		case "alignDimVariable":
			System.out.println("Description: Align the bounding box of the domain of a variable with the point vec{0}, by using a translation.Note: might not work if the lower bound is not an affine functionReturn the affine function used to perform the translation.");
			System.out.println("Usage: alignDimVariable(Program prog, String syst, String varName)");
			break;
		case "generateScheduledCode":
			System.out.println("Description: Generates scheduledC code for a system using the TargetMapping specified for the system.Detailed options can be given through optional argument.");
			System.out.println("Usage: generateScheduledCode(Program prog, String system, CodeGenOptions options, String outDir, Boolean genVerifier)");
			break;
		case "generateWriteC":
			System.out.println("Description: Generates WriteC code for a system.TargetMapping is ignored except for MemoryMapping given for output variables.Detailed options can be given through optional argument.");
			System.out.println("Usage: generateWriteC(Program prog, String system, CodeGenOptions options, String outDir)");
			break;
		case "generateWrapper":
			System.out.println("Description: Generates a wrapper code for the given sytem using the specified TargetMapping.");
			System.out.println("Usage: generateWrapper(Program prog, String system, CodeGenOptions options, String outDir)");
			break;
		case "generateVerificationCode":
			System.out.println("Description: Generates code for verifying generated program. The code is generated using demand-driven code generator (WriteC).");
			System.out.println("Usage: generateVerificationCode(Program prog, String system, String outDir)");
			break;
		case "generateMakefile":
			System.out.println("Description: Generates Makefile to compile generated code + wrapper.");
			System.out.println("Usage: generateMakefile(Program prog, String system, CodeGenOptions CodeGenOptions, String outDir)");
			break;
		case "createCGOptionForWriteC":
			System.out.println("Description: Creates instance of CodeGenOptions for WriteC using default values.");
			System.out.println("Usage: createCGOptionForWriteC()");
			break;
		case "createCGOptionForScheduledC":
			System.out.println("Description: Creates instance of CodeGenOptions for ScheduledC using default values.");
			System.out.println("Usage: createCGOptionForScheduledC()");
			break;
		case "createCGOptionForHybridScheduledCGPU":
			System.out.println("Description: Creates instance of CodeGenOptions for Hybrid ScheduledC for GPU using dimToInsertChecks to insert checks.");
			System.out.println("Usage: createCGOptionForHybridScheduledCGPU(int dimToInsertChecks)");
			break;
		case "createCGOptionForHybridScheduledC":
			System.out.println("Description: Creates instance of CodeGenOptions for Hybrid ScheduledC using dimToInsertChecks to insert checks.");
			System.out.println("Usage: createCGOptionForHybridScheduledC(int dimToInsertChecks)");
			break;
		case "setCGOptionFlattenArrays":
			System.out.println("Description: Specifies true/false (1/0) values for if the multi-dimensional arrays allocated should be flattened to 1D or not.");
			System.out.println("Usage: setCGOptionFlattenArrays(CodeGenOptions CGoptions, int flatten)");
			break;
		case "setCGOptionDisableNormalize_depreciated":
			System.out.println("Description: Options to skip Normalize before generating code with the ScheduleC code generator.This is an option added as an workaround to scalability problems with Normalize, and will be removed in the future.Not normalizing may lead to significantly inefficient code.");
			System.out.println("Usage: setCGOptionDisableNormalize_depreciated(CodeGenOptions options)");
			break;
		case "createTiledCGOptionForScheduledC":
			System.out.println("Description: Creates instance of CodeGenOptions for ScheduledC using default values for tiling.");
			System.out.println("Usage: createTiledCGOptionForScheduledC()");
			break;
		case "createCGOptionsForPCOT":
			System.out.println("Description: Creates instance of CodeGenOptions for PCOTC using default values for tiling.");
			System.out.println("Usage: createCGOptionsForPCOT()");
			break;
		case "setTiledCGOptionOptimize":
			System.out.println("Description: Applies optimizations to the tiled code generated using full-tile splitting of a selected statement group.Statement group is selected with heuristics that may not be accurate.");
			System.out.println("Usage: setTiledCGOptionOptimize(TiledCodeGenOptions options, int optimize)");
			break;
		case "generateScanC":
			System.out.println("Description: code generator for a scan subsystem ( a subsystem that contains only scan computations)");
			System.out.println("Usage: generateScanC(Program prog, String systemName, CodeGenOptions options, String outDir)");
			break;
		case "generatePCOTCode":
			System.out.println("Description: CPU code generator for Polyhedral Cache Oblivious Tiling");
			System.out.println("Usage: generatePCOTCode(Program prog, String system, TiledCodeGenOptions options, String outDir, Boolean genVerifier)");
			break;
		case "generateFMPPCode":
			System.out.println("Description: generate code using the flattened multi-pass tiling and parallelization strategy");
			System.out.println("Usage: generateFMPPCode(Program prog, String systemName, String TilingType, TiledCodeGenOptions CodeGenOptions, String outDir)");
			break;
		case "setVecOptionForTiledC":
			System.out.println("Description: Turn on the vectorization option for TiledC. Right now it is only open for DTiler -- Where only 1 level of tiling is applied and all dimensions are tiled.Four types of vectorization strategy is supported: naive, opt nobuff -- optimized vectorization strategy without temporary buffering, all aligned -- optimized vectorization strategy with temporary buffering and all vectors aligned, mostly aligned -- optimized vectorization strategy with temporary buffering and mostly aligned strategy. The default strategy is set to be \"opt nobuff\".Register block size only corresponds to the data dimensions of stencil computations, the default register block size is set to unroll the innermost vectorized dimension 4 times. ");
			System.out.println("Usage: setVecOptionForTiledC(TiledCodeGenOptions options, String vecSet, String vecStrategy, String registerBlockSize)");
			break;
		case "setVecVarsForTiledC":
			System.out.println("Description: Set the variables for vectorization. All the variable statement must occur under the same innermost loop nest. ");
			System.out.println("Usage: setVecVarsForTiledC(Program prog, String systemName, TiledCodeGenOptions options, String vars)");
			break;
		case "setVecVarForTiledC":
			System.out.println("Usage: setVecVarForTiledC(Program prog, String systemName, TiledCodeGenOptions options, String var, String domain)");
			break;
		case "addRecursionDepthForPCOT":
			System.out.println("Description: Generates code with recursion depth as an input parameter.");
			System.out.println("Usage: addRecursionDepthForPCOT(TiledCodeGenOptions options)");
			break;
		case "readDomain":
			System.out.println("Description: Reads domain in String and returns a Domain object.");
			System.out.println("Usage: readDomain(Domain paramDomain, String domain)");
			break;
		case "readFunction":
			System.out.println("Description: Reads affine function in String and returns an AffineFunction object.");
			System.out.println("Usage: readFunction(Domain paramDomain, String function)");
			break;
		case "inverse":
			System.out.println("Description: Compute the inverse of the give affine function.");
			System.out.println("Usage: inverse(AffineFunction function)");
			break;
		case "inverseInContext":
			System.out.println("Description: Computes inverse of the given affine function, in the context of the given domain.");
			System.out.println("Usage: inverseInContext(Domain domain, AffineFunction function)");
			break;
		case "compose":
			System.out.println("Description: Returns a function, that computes function2@function1.");
			System.out.println("Usage: compose(AffineFunction function1, AffineFunction function2)");
			break;
		case "intersection":
			System.out.println("Description: Returns the intersection of two domains given.");
			System.out.println("Usage: intersection(Domain domain1, Domain domain2)");
			break;
		case "union":
			System.out.println("Description: Returns the union of two domains given.");
			System.out.println("Usage: union(Domain domain1, Domain domain2)");
			break;
		case "join":
			System.out.println("Description: Returns a function that computes function1@function2.");
			System.out.println("Usage: join(AffineFunction function1, AffineFunction function2)");
			break;
		case "isEmpty":
			System.out.println("Description: Returns true if the domain is empty.");
			System.out.println("Usage: isEmpty(Domain domain)");
			break;
		case "isEquivalent":
			System.out.println("Description: Returns true if the two domains/functions given are equivalent.");
			System.out.println("Usage: isEquivalent(Domain domainA, Domain domainB)");
			break;
		case "image":
			System.out.println("Description: Compute the image of the given domain by the given function.");
			System.out.println("Usage: image(AffineFunction function, Domain domain)");
			break;
		case "preImage":
			System.out.println("Description: Compute the pre-image of the given domain by the given function.");
			System.out.println("Usage: preImage(AffineFunction function, Domain domain)");
			break;
		case "difference":
			System.out.println("Description: Returns domainA / domainB.");
			System.out.println("Usage: difference(Domain domainA, Domain domainB)");
			break;
		case "simplifyInContext":
			System.out.println("Description: Takes domain and context domain, and returns the domain with constraints that are redundant with the context removed.");
			System.out.println("Usage: simplifyInContext(Domain domain, Domain context)");
			break;
		case "BuildPRDG":
			System.out.println("Description: Constructs a PRDG for the specified AffineSystem. Input variables are excluded from the PRDG default. To override this option, set the optional argument noInput to 0.");
			System.out.println("Usage: BuildPRDG(Program prog, String systemName, int noInputs)");
			break;
		case "ExportPRDG":
			System.out.println("Description: Exports the given PRDG as a dot file.");
			System.out.println("Usage: ExportPRDG(PRDG prdg, String filename)");
			break;
		case "CheckProgram":
			System.out.println("Description: Performs uniqueness and completeness check of the program.The program is a valid alphabets program, if it passes this check.Details of the check can be found at url{http://www.cs.colostate.edu/AlphaZ/wiki/doku.php?id=check_program}.");
			System.out.println("Usage: CheckProgram(Program prog)");
			break;
		case "VerifyTargetMapping":
			System.out.println("Description: Verifies the target mapping given to a system. The third input takes \"NONE\", \"MIN\", \"MAX\" to control the verbosity.");
			System.out.println("Usage: VerifyTargetMapping(Program prog, String system, String verbose)");
			break;
		case "CheckSystem":
			System.out.println("Description: CheckSystem is CheckProgram applied to the specified system.");
			System.out.println("Usage: CheckSystem(Program prog, String system)");
			break;
		case "Farkas1DScheduler":
			System.out.println("Description: Farkas mono-dimensional scheduler.");
			System.out.println("Usage: Farkas1DScheduler(PRDG prdg)");
			break;
		case "FarkasMDScheduler":
			System.out.println("Description: Farkas multi-dimensional scheduler. Uses ISL implementation.");
			System.out.println("Usage: FarkasMDScheduler(PRDG prdg)");
			break;
		case "PlutoScheduler":
			System.out.println("Description: Pluto scheduler. Uses implementation in ISL.");
			System.out.println("Usage: PlutoScheduler(PRDG prdg)");
			break;
		case "setSpaceTimeMap":
			System.out.println("Description: Specifies a space time mapping of a variable at a certain level. By default (when there is no value for level), the space-time map is set up for the first level of targetMapping.");
			System.out.println("Usage: setSpaceTimeMap(Program prog, String system, int level, String varList, AffineFunction stMap)");
			break;
		case "setMemoryMap":
			System.out.println("Description: Specifies a memory map for a variable. Some variables may share the same meory map and memory space, then they can be set up using one command by providing a list of variables. ");
			System.out.println("Usage: setMemoryMap(Program prog, String system, String varList, String memorySpace, AffineFunction memoryMap, String modFactors)");
			break;
		case "setMemorySpace":
			System.out.println("Description: Specifies the memory space for a variable. Normally, each variable has a separate memory space. However, some variables may share the same memory space, and this can be set up once by providing a list of variable.");
			System.out.println("Usage: setMemorySpace(Program prog, String system, String space, String varList)");
			break;
		case "setStatementOrdering":
			System.out.println("Description: Specifies the ordering of the statements (alphabets variables) in the generated code. This is similar to adding an additional ordering dimensions to the last dimension of space-time mapping, and providing ordering information.AlphaZ provides an alternative to such specification by allowing the user to specify partial orderings between statements.A total order is deduced at the time of code generation.");
			System.out.println("Usage: setStatementOrdering(Program prog, String system, String predecessor, String successor)");
			break;
		case "listSpaceTimeMaps":
			System.out.println("Description: Lists all SpaceTime maps that have been specified for a system.");
			System.out.println("Usage: listSpaceTimeMaps(Program prog, String system, int level)");
			break;
		case "listMemoryMaps":
			System.out.println("Description: Lists all memory mappings specified for a system.");
			System.out.println("Usage: listMemoryMaps(Program prog, String system)");
			break;
		case "setSchedule":
			System.out.println("Description: Applies schedules found by a scheduler (such as Farkas scheduler) to target mapping.");
			System.out.println("Usage: setSchedule(Program prog, String system, List<ScheduledStatement> schedules)");
			break;
		case "setParallel":
			System.out.println("Description: Specify the parallel dimensions for a system. The nth dimension (specified by parallelDims, start with zero) with the specified ordering prefix to be parallel.");
			System.out.println("Usage: setParallel(Program prog, String system, int level, String orderingPrefix, String parallelDims)");
			break;
		case "CreateSpaceTimeLevel":
			System.out.println("Description: Create a spacetimelevel data stucture for level (level)");
			System.out.println("Usage: CreateSpaceTimeLevel(Program prog, String system, int level)");
			break;
		case "setOrderingDimensions":
			System.out.println("Description: specify which dimension is the ordering dimension for each level of the targetMapping. Dimension starts with zero. If no value is specified for the targetMapping level, it is the first level by default. ");
			System.out.println("Usage: setOrderingDimensions(Program prog, String system, int level, String dims)");
			break;
		case "setSpaceTimeMapForMemoryAllocation":
			System.out.println("Description: set space time map for the memory allocation statement for the input/output of the use equation.isInput -- 0: for nTh input of the use equaitonisInput -- 1: for nTh output of the use equation");
			System.out.println("Usage: setSpaceTimeMapForMemoryAllocation(Program prog, String system, String label, int isInput, int num, AffineFunction stMap)");
			break;
		case "setSpaceTimeMapForValueCopy":
			System.out.println("Description: set the space time map for the value copy statement for input/output of a use equation");
			System.out.println("Usage: setSpaceTimeMapForValueCopy(Program prog, String system, String label, int isInput, int num, AffineFunction stMap)");
			break;
		case "setSpaceTimeMapForMemoryFree":
			System.out.println("Description: set the space time map for the memory free statement for the input/ouput of a useEquation.isInput -- 0, for the input of the useEquationisInput -- 1, for the output of the useEquation");
			System.out.println("Usage: setSpaceTimeMapForMemoryFree(Program prog, String system, String label, int isInput, int num, AffineFunction stMap)");
			break;
		case "setSpaceTimeMapForUseEquationOptimization":
			System.out.println("Description: set the space time map for the memory allocation, value copy, memory allocation statement for the input/output of the useEuqation.isInput -- 0: for the input of the useEquationisInput -- 1: for the output of the useEquation");
			System.out.println("Usage: setSpaceTimeMapForUseEquationOptimization(Program prog, String system, String label, int isInput, int num, AffineFunction stMapForMemoryAllocation, AffineFunction stMapForValueCopy, AffineFunction stMapForMemoryFree)");
			break;
		case "setMemorySpaceForUseEquationOptimization":
			System.out.println("Description: set memory space for the input/ouput of the useEquation");
			System.out.println("Usage: setMemorySpaceForUseEquationOptimization(Program prog, String system, String label, int isInput, int num, String spaceName)");
			break;
		case "setBandForTiling":
			System.out.println("Description: configure a band of continuous dimensions for tiling.");
			System.out.println("Usage: setBandForTiling(Program prog, String system, String bandName, int levels, String orderingPrefix, int startDim, int endDim)");
			break;
		case "setSubTilingWithinBand":
			System.out.println("Description: Configure the subtiling specification for a band. The band is identified by the name. There are two types of tiling type: sequential and openmp wavefront. Represented with \"sequential\" and \"wavefront\" separately.");
			System.out.println("Usage: setSubTilingWithinBand(Program prog, String system, String bandName, int level, int startDim, int endDim, String tilingType)");
			break;
		case "setDefaultDTilerConfiguration":
			System.out.println("Description: Set up the default configuration for dtiler: apply parametric one level tiling to all the dimensions from [start to end].");
			System.out.println("Usage: setDefaultDTilerConfiguration(Program prog, String system, int startDim, int endDim, String tilingType)");
			break;
		case "history":
			System.out.println("Description: Print out command history so far.");
			System.out.println("Usage: history()");
			System.out.println("Usage: history(int n)");
			break;
		case "searchHistory":
			System.out.println("Description: Search History for a particular pattern - can be a regex.");
			System.out.println("Usage: searchHistory(String pattern)");
			break;
		case "undo":
			System.out.println("Descrgiption: Undo n times, default 1.");
			System.out.println("Usage: undo()");
			System.out.println("Usage: undo(int n)");
			break;
		case "redo":
			System.out.println("Descrgiption: Redo n times, default 1.");
			System.out.println("Usage: redo()");
			System.out.println("Usage: redo(int n)");
			break;
		case "printTransform":
			System.out.println("Description: Print the effective transformation carried out on the program so far, after a long sequence of actions.");
			System.out.println("Usage: printTransform()");
			break;
		case "delVar":
			System.out.println("Description: Removes variable from context. If you ask to remove program from context, it's undo/redo history will also be erased.");
			System.out.println("Usage: delVar(String varName)");
			break;
		case "clear":
			System.out.println("Description: Clears the symbol table, program, and undo/redo history");
			System.out.println("Usage: clear()");
			break;
		case "help":
			System.out.println("Description: Prints help messages for commands.");
			System.out.println("Usage: help(String funcName)");
			break;
		default:
			System.out.println("No relevant documentation found");
		}
	}
}