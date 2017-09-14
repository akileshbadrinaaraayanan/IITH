import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.regex.PatternSyntaxException;

import org.polymodel.polyhedralIR.AffineFunction;
import org.polymodel.polyhedralIR.Domain;
import org.polymodel.polyhedralIR.Program;
import org.polymodel.polyhedralIR.polyIRCG.generator.CodeGenOptions;
import org.polymodel.polyhedralIR.polyIRCG.generator.TiledCodeGenOptions;
import org.polymodel.prdg.PRDG;
import org.polymodel.prdg.scheduling.ScheduledStatement;

import alphaz.mde.Analysis;
import alphaz.mde.Basic;
import alphaz.mde.Calculator;
import alphaz.mde.CodeGen;
import alphaz.mde.TargetMapping;
import alphaz.mde.Transformation;
import alphaz.mde.transformation.MonoparametricTiling;
import alphaz.mde.transformation.Reduction;


/**
 * 
 * <h1>	CommandProcessor </h1>
 * The CommandProcessor Class is the main workhorse of our application. 
 * It takes a command inputted at the console, parses it, and passes it to 
 * the relevant AlphaZ function. 
 * It also maintains internal state to maintain local variables, print command history, 
 * print help, and undo/redo an action.
 * 
 * @author Hrishikesh Vaidya, Surya Teja Chavali, B Akilesh
 * 
 */

public class CommandProcessor {
	String progName; 
	Pattern exprReg= Pattern.compile("((\\w+)\\s*[=]\\s*)?(\\w+)\\s*[(]\\s*((\\w+|\\d+|([\"][^\"]*[\"]))?(\\s*,\\s*(\\w+|\\d+|([\"][^\"]*[\"])))*)[)];");
	Pattern strReg = Pattern.compile("(\\w+)\\s*=\\s*[\"]([^\"]*)[\"];");
	HashMap<String,String> methodMap;
	SymbolTable st;
	HelpPrinter hp = new HelpPrinter();
	Memento memento = null; 
	ArrayList<String> history = new ArrayList<String>();
	
	/**
	 * <h2> CommandProcessor() </h2>
	 * The constructor of the CommandProcessor class.
	 * Generates the mapping of AlphaZ functions to return types, and also initializes internal state variables.
	 * @throws SecurityException
	 * @throws ClassNotFoundException
	 */
	public CommandProcessor() throws SecurityException, ClassNotFoundException 
	{
		methodMap = genReturnTypeMap();
		st = new SymbolTable();
	}
	
	/**
	 * <h2> genReturnTypeMap() </h2>
	 * Generates the mapping of AlphaZ functions to return types.
	 * @return HashMap<String, String> A map that maps AlphaZ function to return type. 
	 * @throws SecurityException
	 * @throws ClassNotFoundException
	 */
	private HashMap<String,String> genReturnTypeMap() throws SecurityException, ClassNotFoundException 
	{
		List<Class> subCatgs = Arrays.asList(Basic.class, Calculator.class, Reduction.class,MonoparametricTiling.class,Transformation.class,CodeGen.class,Analysis.class,TargetMapping.class);
		HashMap<String,String> methodReturn = new HashMap();
		for(Class subcat: subCatgs) {
			Method[] methods = subcat.getDeclaredMethods();
			for(Method m: methods){
				methodReturn.put(m.getName(), m.getReturnType().toString());
			}
		}
		return methodReturn;
	}
	
	/**
	 * <h2> processParams </h2>
	 * Processes the list of method parameters, represented as a string,
	 * returns the list of converted string literals, integers, and objects corresponding to variable names. 
	 * @param params - An array of Strings representing function parameters.
	 * @return An array of Objects representing the function parameters.
	 * @throws IOException
	 */
	private  Object[] processParams(String[] params) throws IOException{
		List<Object> procParams = new ArrayList<Object>();
		for(String p: params){
			//System.out.println(p);
			if(p.startsWith("\"") && p.endsWith("\"")){
				procParams.add(p.replaceAll("\"",""));
			}
			else if(p.matches("\\d+")){
				procParams.add(Integer.valueOf(p));
			}
			else{
				Object obj = st.get(p);
				if(obj != null){
					procParams.add(obj);
				}
				else{
					if(p.contains("(") && p.contains(")"))
						System.err.println("Nested expressions not supported.");
					else
						System.err.println("Variable " + p + " not declared.");
					return null;
				}
			}
		}
		return procParams.toArray(new Object[procParams.size()]);
		
	}
	
	/**
	 * <h2> computeFunc </h2>
	 * The main workhorse of the CommandProcessor class. Reads the command, obtains the parameters,
	 * passes it to the relevant overloaded (usually AlphaZ) method. If there is an error, prints 
	 * the relevant help message, indicating usage.
	 * @param input The command
	 * @throws ClassNotFoundException
	 * @throws NoSuchMethodException
	 * @throws SecurityException
	 * @throws IllegalAccessException
	 * @throws IllegalArgumentException
	 * @throws InvocationTargetException
	 * @throws IOException
	 */
	public void computeFunc(String input) throws ClassNotFoundException, NoSuchMethodException, SecurityException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, IOException{
		Matcher m = exprReg.matcher(input);
		String func = null;
		String args[] = null;
		String assignVar = null;
		String paramstr = null;
		Object[] params = null;
		
		history.add(input);
		//Parse the arguments to the function.
		if(m.find()) {
			input = input.substring(0, input.length()-1);
			assignVar = m.group(2);
			func = m.group(3);
			paramstr = m.group(4);
			//System.out.println(paramstr);
			if(!paramstr.isEmpty()){
		
				String[] quotesplit = paramstr.split("\"");
				String nonstrparams = "";
				for(int i=0;i<quotesplit.length; i+=2)
					nonstrparams = nonstrparams + quotesplit[i];
				//nonstrparams = nonstrparams.replaceAll(" ", "");
				String[] commasplit = nonstrparams.split(",");
				args = new String[commasplit.length];
				for(int i=0, j=1;i<commasplit.length; i++){
					if(commasplit[i].replaceAll(" ", "").isEmpty()){
						args[i]="\"" + quotesplit[j] + "\"";
						j+=2;
					}
					else
						args[i] = commasplit[i].replaceAll(" ", "");
					//System.out.println(args[i]);
				}
			}
		}
		else{
			Matcher strmm = strReg.matcher(input);
			if(strmm.find()){
				assignVar = strmm.group(1);
				paramstr = strmm.group(2);
				st.put(assignVar, paramstr);
				return;
			}
			else{
				System.err.println("Syntax Error.");
				return;
			}
		}
		
		if(args != null){
			params = processParams(args);
			if(params == null){
				return;
			}
		}
		else
			params = new Object[0];
		if(methodMap.containsKey(func) && methodMap.get(func).equals("void") && assignVar != null){
			System.out.println("Method returns void. Cannot assign to variable");
			return;
		}
		if(st.contains(assignVar)){
			System.out.println("Redeclaration of variable " + assignVar);
			return;
		}
		
		Object result = null;
		
		//Big switch case for to call the relevant function.
		switch(func){
		case "ASave":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Basic.ASave((Program) params[0], (String) params[1]);
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				Basic.ASave((Program) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "ASaveSystem":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Basic.ASaveSystem((Program) params[0], (String) params[1], (String) params[2]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Basic.ASaveSystem((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "AShow":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				result = Basic.AShow((Program) params[0], (String) params[1]);
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				result = Basic.AShow((Program) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "Save":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Basic.Save((Program) params[0], (String) params[1]);
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				Basic.Save((Program) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "SaveSystem":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Basic.SaveSystem((Program) params[0], (String) params[1], (String) params[2]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Basic.SaveSystem((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "Show":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				result = Basic.Show((Program) params[0], (String) params[1]);
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				result = Basic.Show((Program) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "Normalize":
			if((params.length == 1) && (params[0] instanceof Program)){
				Basic.Normalize((Program) params[0]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "PrintAST":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				result = Basic.PrintAST((Program) params[0], (String) params[1]);
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				result = Basic.PrintAST((Program) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "ReadAlphabets":
			if((params.length == 1) && (params[0] instanceof String)){
				if(!new File((String) params[0]).exists()){
					System.err.println("File does not exist");
					return;
				}
				result = Basic.ReadAlphabets((String) params[0]);
				memento = new Memento((Program) result);
				progName = assignVar;
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "RenameSystem":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Basic.RenameSystem((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "RenameVariable":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				Basic.RenameVariable((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "RemoveUnusedVariables":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Basic.RemoveUnusedVariables((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				Basic.RemoveUnusedVariables((Program) params[0]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "PermutationCaseReduce":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Reduction.PermutationCaseReduce((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				Reduction.PermutationCaseReduce((Program) params[0]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Reduction.PermutationCaseReduce((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "ReductionDecomposition":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				Reduction.ReductionDecomposition((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "SimplifyingReduction":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				Reduction.SimplifyingReduction((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 1) && (params[0] instanceof Program)){
//				Reduction.SimplifyingReduction((Program) params[0]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "NormalizeReduction":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Reduction.NormalizeReduction((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				Reduction.NormalizeReduction((Program) params[0]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				Reduction.NormalizeReduction((Program) params[0]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "FactorOutFromReduction":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Reduction.FactorOutFromReduction((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "SplitReductionBody":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Reduction.SplitReductionBody((Program) params[0], (String) params[1], (String) params[2]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "TransformReductionBody":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Reduction.TransformReductionBody((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "SerializeReduction":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Reduction.SerializeReduction((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 1) && (params[0] instanceof Program)){
//				Reduction.SerializeReduction((Program) params[0]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "MergeReductions":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Reduction.MergeReductions((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "ReductionComposition":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Reduction.ReductionComposition((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "monoparametricTiling_noOutlining":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Boolean) && (params[4] instanceof Integer)){
				MonoparametricTiling.monoparametricTiling_noOutlining((Program) params[0], (String) params[1], (String) params[2], (Boolean) params[3], (Integer) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "monoparametricTiling_Outlining_noSubsystem":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Boolean) && (params[4] instanceof Integer)){
				MonoparametricTiling.monoparametricTiling_Outlining_noSubsystem((Program) params[0], (String) params[1], (String) params[2], (Boolean) params[3], (Integer) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "monoparametricTiling_Outlining":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Boolean) && (params[4] instanceof Integer)){
				MonoparametricTiling.monoparametricTiling_Outlining((Program) params[0], (String) params[1], (String) params[2], (Boolean) params[3], (Integer) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setRatio":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				MonoparametricTiling.setRatio((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setMinParamValues":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer)){
				MonoparametricTiling.setMinParamValues((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setTileGroup":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				MonoparametricTiling.setTileGroup((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setCoBPreprocess":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				MonoparametricTiling.setCoBPreprocess((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "CoB":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof AffineFunction)){
				Transformation.CoB((Program) params[0], (String) params[1], (String) params[2], (AffineFunction) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
//				Transformation.CoB((Program) params[0], (String) params[1], (String) params[2]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "ForceCoB":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof AffineFunction)){
				Transformation.ForceCoB((Program) params[0], (String) params[1], (String) params[2], (AffineFunction) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
//				Transformation.ForceCoB((Program) params[0], (String) params[1], (String) params[2]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "Split":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String) && (params[4] instanceof String)){
				Transformation.Split((Program) params[0], (String) params[1], (String) params[2], (String) params[3], (String) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				Transformation.Split((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "Merge":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String) && (params[4] instanceof String)){
				Transformation.Merge((Program) params[0], (String) params[1], (String) params[2], (String) params[3], (String) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "Inline":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String) && (params[4] instanceof Integer)){
				Transformation.Inline((Program) params[0], (String) params[1], (String) params[2], (String) params[3], (Integer) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				Transformation.Inline((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "Simplify":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Transformation.Simplify((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				Transformation.Simplify((Program) params[0]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "createFreeScheduler":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				result = Transformation.createFreeScheduler((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				result = Transformation.createFreeScheduler((Program) params[0]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "SplitUnion":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Transformation.SplitUnion((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "ApplySTMap":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Transformation.ApplySTMap((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "UniformizeInContext":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Integer)){
				Transformation.UniformizeInContext((Program) params[0], (String) params[1], (Integer) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 1) && (params[0] instanceof Program)){
				Transformation.UniformizeInContext((Program) params[0]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Transformation.UniformizeInContext((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "InlineForce":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String) && (params[4] instanceof Integer)){
				Transformation.InlineForce((Program) params[0], (String) params[1], (String) params[2], (String) params[3], (Integer) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				Transformation.InlineForce((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "InlineAll":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Transformation.InlineAll((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "InlineAllForce":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Transformation.InlineAllForce((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "InlineSubSystem":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Transformation.InlineSubSystem((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "OutlineSubSystem":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Transformation.OutlineSubSystem((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "AddLocal":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				Transformation.AddLocal((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
//				Transformation.AddLocal((Program) params[0], (String) params[1], (String) params[2]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "AddLocalUnique":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				Transformation.AddLocalUnique((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "DetectReductions":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Transformation.DetectReductions((Program) params[0], (String) params[1]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "reduceDimVariable":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				result = Transformation.reduceDimVariable((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "alignDimVariable":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				result = Transformation.alignDimVariable((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "generateScheduledCode":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof CodeGenOptions) && (params[3] instanceof String) && (params[4] instanceof Boolean)){
				CodeGen.generateScheduledCode((Program) params[0], (String) params[1], (CodeGenOptions) params[2], (String) params[3], (Boolean) params[4]);
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				CodeGen.generateScheduledCode((Program) params[0], (String) params[1], (String) params[2]);
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Boolean)){
				CodeGen.generateScheduledCode((Program) params[0], (String) params[1], (String) params[2], (Boolean) params[3]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				CodeGen.generateScheduledCode((Program) params[0], (String) params[1]);
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Boolean)){
				CodeGen.generateScheduledCode((Program) params[0], (String) params[1], (Boolean) params[2]);
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof CodeGenOptions) && (params[3] instanceof String)){
				CodeGen.generateScheduledCode((Program) params[0], (String) params[1], (CodeGenOptions) params[2], (String) params[3]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "generateWriteC":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof CodeGenOptions) && (params[3] instanceof String)){
				CodeGen.generateWriteC((Program) params[0], (String) params[1], (CodeGenOptions) params[2], (String) params[3]);
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				CodeGen.generateWriteC((Program) params[0], (String) params[1], (String) params[2]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				CodeGen.generateWriteC((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "generateWrapper":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof CodeGenOptions) && (params[3] instanceof String)){
				CodeGen.generateWrapper((Program) params[0], (String) params[1], (CodeGenOptions) params[2], (String) params[3]);
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				CodeGen.generateWrapper((Program) params[0], (String) params[1], (String) params[2]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				CodeGen.generateWrapper((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "generateVerificationCode":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				CodeGen.generateVerificationCode((Program) params[0], (String) params[1], (String) params[2]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				CodeGen.generateVerificationCode((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "generateMakefile":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof CodeGenOptions) && (params[3] instanceof String)){
				CodeGen.generateMakefile((Program) params[0], (String) params[1], (CodeGenOptions) params[2], (String) params[3]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				CodeGen.generateMakefile((Program) params[0], (String) params[1]);
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				CodeGen.generateMakefile((Program) params[0], (String) params[1], (String) params[2]);
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof CodeGenOptions)){
				CodeGen.generateMakefile((Program) params[0], (String) params[1], (CodeGenOptions) params[2]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "generateMakefileInternal":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof CodeGenOptions) && (params[3] instanceof String)){
				CodeGen.generateMakefileInternal((Program) params[0], (String) params[1], (CodeGenOptions) params[2], (String) params[3]);
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				CodeGen.generateMakefileInternal((Program) params[0], (String) params[1], (String) params[2]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "createCGOptionForWriteC":
			if((params.length == 0)){
				result = CodeGen.createCGOptionForWriteC();
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "createCGOptionForScheduledC":
			if((params.length == 0)){
				result = CodeGen.createCGOptionForScheduledC();
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "createCGOptionForHybridScheduledCGPU":
			if((params.length == 1) && (params[0] instanceof Integer)){
				result = CodeGen.createCGOptionForHybridScheduledCGPU((Integer) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "createCGOptionForHybridScheduledC":
			if((params.length == 1) && (params[0] instanceof Integer)){
				result = CodeGen.createCGOptionForHybridScheduledC((Integer) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setCGOptionFlattenArrays":
			if((params.length == 2) && (params[0] instanceof CodeGenOptions) && (params[1] instanceof Integer)){
				CodeGen.setCGOptionFlattenArrays((CodeGenOptions) params[0], (Integer) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setCGOptionDisableNormalize_depreciated":
			if((params.length == 1) && (params[0] instanceof CodeGenOptions)){
				CodeGen.setCGOptionDisableNormalize_depreciated((CodeGenOptions) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "createTiledCGOptionForScheduledC":
			if((params.length == 0)){
				result = CodeGen.createTiledCGOptionForScheduledC();
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "createCGOptionsForPCOT":
			if((params.length == 0)){
				result = CodeGen.createCGOptionsForPCOT();
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setTiledCGOptionOptimize":
			if((params.length == 2) && (params[0] instanceof TiledCodeGenOptions) && (params[1] instanceof Integer)){
				CodeGen.setTiledCGOptionOptimize((TiledCodeGenOptions) params[0], (Integer) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "getDefaultCodeGenOptions":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				result = CodeGen.getDefaultCodeGenOptions((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "generateScanC":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof CodeGenOptions) && (params[3] instanceof String)){
				CodeGen.generateScanC((Program) params[0], (String) params[1], (CodeGenOptions) params[2], (String) params[3]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "generatePCOTCode":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof TiledCodeGenOptions) && (params[3] instanceof String) && (params[4] instanceof Boolean)){
				CodeGen.generatePCOTCode((Program) params[0], (String) params[1], (TiledCodeGenOptions) params[2], (String) params[3], (Boolean) params[4]);
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				CodeGen.generatePCOTCode((Program) params[0], (String) params[1], (String) params[2]);
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Boolean)){
				CodeGen.generatePCOTCode((Program) params[0], (String) params[1], (String) params[2], (Boolean) params[3]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				CodeGen.generatePCOTCode((Program) params[0], (String) params[1]);
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Boolean)){
				CodeGen.generatePCOTCode((Program) params[0], (String) params[1], (Boolean) params[2]);
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof TiledCodeGenOptions) && (params[3] instanceof String)){
				CodeGen.generatePCOTCode((Program) params[0], (String) params[1], (TiledCodeGenOptions) params[2], (String) params[3]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "generateFMPPCode":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof TiledCodeGenOptions) && (params[4] instanceof String)){
				CodeGen.generateFMPPCode((Program) params[0], (String) params[1], (String) params[2], (TiledCodeGenOptions) params[3], (String) params[4]);
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				CodeGen.generateFMPPCode((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				CodeGen.generateFMPPCode((Program) params[0], (String) params[1], (String) params[2]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				CodeGen.generateFMPPCode((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setVecOptionForTiledC":
			if((params.length == 4) && (params[0] instanceof TiledCodeGenOptions) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				CodeGen.setVecOptionForTiledC((TiledCodeGenOptions) params[0], (String) params[1], (String) params[2], (String) params[3]);
			}
			else if((params.length == 1) && (params[0] instanceof TiledCodeGenOptions)){
				CodeGen.setVecOptionForTiledC((TiledCodeGenOptions) params[0]);
			}
			else if((params.length == 2) && (params[0] instanceof TiledCodeGenOptions) && (params[1] instanceof String)){
				CodeGen.setVecOptionForTiledC((TiledCodeGenOptions) params[0], (String) params[1]);
			}
			else if((params.length == 3) && (params[0] instanceof TiledCodeGenOptions) && (params[1] instanceof String) && (params[2] instanceof String)){
				CodeGen.setVecOptionForTiledC((TiledCodeGenOptions) params[0], (String) params[1], (String) params[2]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setVecVarsForTiledC":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof TiledCodeGenOptions) && (params[3] instanceof String)){
				CodeGen.setVecVarsForTiledC((Program) params[0], (String) params[1], (TiledCodeGenOptions) params[2], (String) params[3]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setVecVarForTiledC":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof TiledCodeGenOptions) && (params[3] instanceof String) && (params[4] instanceof String)){
				CodeGen.setVecVarForTiledC((Program) params[0], (String) params[1], (TiledCodeGenOptions) params[2], (String) params[3], (String) params[4]);
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof TiledCodeGenOptions) && (params[3] instanceof String)){
				CodeGen.setVecVarForTiledC((Program) params[0], (String) params[1], (TiledCodeGenOptions) params[2], (String) params[3]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "addRecursionDepthForPCOT":
			if((params.length == 1) && (params[0] instanceof TiledCodeGenOptions)){
				result = CodeGen.addRecursionDepthForPCOT((TiledCodeGenOptions) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "readDomain":
			if((params.length == 2) && (params[0] instanceof Domain) && (params[1] instanceof String)){
				result = Calculator.readDomain((Domain) params[0], (String) params[1]);
			}
			else if((params.length == 1) && (params[0] instanceof String)){
				result = Calculator.readDomain((String) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "readFunction":
			if((params.length == 2) && (params[0] instanceof Domain) && (params[1] instanceof String)){
				result = Calculator.readFunction((Domain) params[0], (String) params[1]);
			}
			else if((params.length == 1) && (params[0] instanceof String)){
				result = Calculator.readFunction((String) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "inverse":
			if((params.length == 1) && (params[0] instanceof AffineFunction)){
				result = Calculator.inverse((AffineFunction) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "inverseInContext":
			if((params.length == 2) && (params[0] instanceof Domain) && (params[1] instanceof AffineFunction)){
				result = Calculator.inverseInContext((Domain) params[0], (AffineFunction) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "compose":
			if((params.length == 2) && (params[0] instanceof AffineFunction) && (params[1] instanceof AffineFunction)){
				result = Calculator.compose((AffineFunction) params[0], (AffineFunction) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "intersection":
			if((params.length == 2) && (params[0] instanceof Domain) && (params[1] instanceof Domain)){
				result = Calculator.intersection((Domain) params[0], (Domain) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "union":
			if((params.length == 2) && (params[0] instanceof Domain) && (params[1] instanceof Domain)){
				result = Calculator.union((Domain) params[0], (Domain) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "join":
			if((params.length == 2) && (params[0] instanceof AffineFunction) && (params[1] instanceof AffineFunction)){
				result = Calculator.join((AffineFunction) params[0], (AffineFunction) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "isEmpty":
			if((params.length == 1) && (params[0] instanceof Domain)){
				result = Calculator.isEmpty((Domain) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "isEquivalent":
			if((params.length == 2) && (params[0] instanceof Domain) && (params[1] instanceof Domain)){
				result = Calculator.isEquivalent((Domain) params[0], (Domain) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "image":
			if((params.length == 2) && (params[0] instanceof AffineFunction) && (params[1] instanceof Domain)){
				result = Calculator.image((AffineFunction) params[0], (Domain) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "preImage":
			if((params.length == 2) && (params[0] instanceof AffineFunction) && (params[1] instanceof Domain)){
				result = Calculator.preImage((AffineFunction) params[0], (Domain) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "difference":
			if((params.length == 2) && (params[0] instanceof Domain) && (params[1] instanceof Domain)){
				result = Calculator.difference((Domain) params[0], (Domain) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "simplifyInContext":
			if((params.length == 2) && (params[0] instanceof Domain) && (params[1] instanceof Domain)){
				result = Calculator.simplifyInContext((Domain) params[0], (Domain) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "BuildPRDG":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Integer)){
				result = Analysis.BuildPRDG((Program) params[0], (String) params[1], (Integer) params[2]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				result = Analysis.BuildPRDG((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "ExportPRDG":
			if((params.length == 2) && (params[0] instanceof PRDG) && (params[1] instanceof String)){
				Analysis.ExportPRDG((PRDG) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "CheckProgram":
			if((params.length == 1) && (params[0] instanceof Program)){
				Analysis.CheckProgram((Program) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "VerifyTargetMapping":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				Analysis.VerifyTargetMapping((Program) params[0], (String) params[1], (String) params[2]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "CheckSystem":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				Analysis.CheckSystem((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "Farkas1DScheduler":
			if((params.length == 1) && (params[0] instanceof PRDG)){
				result = Analysis.Farkas1DScheduler((PRDG) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "FarkasMDScheduler":
			if((params.length == 1) && (params[0] instanceof PRDG)){
				result = Analysis.FarkasMDScheduler((PRDG) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "PlutoScheduler":
			if((params.length == 1) && (params[0] instanceof PRDG)){
				result = Analysis.PlutoScheduler((PRDG) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "revertPRDGEdges":
			if((params.length == 1) && (params[0] instanceof PRDG)){
				result = Analysis.revertPRDGEdges((PRDG) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "printScheduledStatements":
			if((params.length == 1) && (params[0] instanceof List<?>)){
				Analysis.printScheduledStatements((List<ScheduledStatement>) params[0]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setSpaceTimeMap":
			
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Integer) && (params[3] instanceof String) && (params[4] instanceof AffineFunction)){
				TargetMapping.setSpaceTimeMap((Program) params[0], (String) params[1], (Integer) params[2], (String) params[3], (AffineFunction) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				TargetMapping.setSpaceTimeMap((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
//				TargetMapping.setSpaceTimeMap((Program) params[0], (String) params[1], (String) params[2]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setMemoryMap":
			if((params.length == 6) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String) && (params[4] instanceof AffineFunction) && (params[5] instanceof String)){
				TargetMapping.setMemoryMap((Program) params[0], (String) params[1], (String) params[2], (String) params[3], (AffineFunction) params[4], (String) params[5]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				TargetMapping.setMemoryMap((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof AffineFunction) && (params[4] instanceof String)){
				TargetMapping.setMemoryMap((Program) params[0], (String) params[1], (String) params[2], (AffineFunction) params[3], (String) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof AffineFunction)){
				TargetMapping.setMemoryMap((Program) params[0], (String) params[1], (String) params[2], (AffineFunction) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String) && (params[4] instanceof AffineFunction)){
				TargetMapping.setMemoryMap((Program) params[0], (String) params[1], (String) params[2], (String) params[3], (AffineFunction) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				TargetMapping.setMemoryMap((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
//				TargetMapping.setMemoryMap((Program) params[0], (String) params[1], (String) params[2]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setMemorySpace":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				TargetMapping.setMemorySpace((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setStatementOrdering":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				TargetMapping.setStatementOrdering((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "listSpaceTimeMaps":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Integer)){
				TargetMapping.listSpaceTimeMaps((Program) params[0], (String) params[1], (Integer) params[2]);
			}
			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				TargetMapping.listSpaceTimeMaps((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "listMemoryMaps":
			if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
				TargetMapping.listMemoryMaps((Program) params[0], (String) params[1]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setSchedule":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof List<?>)){
				TargetMapping.setSchedule((Program) params[0], (String) params[1], (List<ScheduledStatement>) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setParallel":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Integer) && (params[3] instanceof String) && (params[4] instanceof String)){
				TargetMapping.setParallel((Program) params[0], (String) params[1], (Integer) params[2], (String) params[3], (String) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof String)){
				TargetMapping.setParallel((Program) params[0], (String) params[1], (String) params[2], (String) params[3]);
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "CreateSpaceTimeLevel":
			if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Integer)){
				TargetMapping.CreateSpaceTimeLevel((Program) params[0], (String) params[1], (Integer) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setOrderingDimensions":
			if((params.length == 4) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Integer) && (params[3] instanceof String)){
				TargetMapping.setOrderingDimensions((Program) params[0], (String) params[1], (Integer) params[2], (String) params[3]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Integer)){
				TargetMapping.setOrderingDimensions((Program) params[0], (String) params[1], (Integer) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				TargetMapping.setOrderingDimensions((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 2) && (params[0] instanceof Program) && (params[1] instanceof String)){
//				TargetMapping.setOrderingDimensions((Program) params[0], (String) params[1]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setSpaceTimeMapForMemoryAllocation":
			if((params.length == 6) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer) && (params[5] instanceof AffineFunction)){
				TargetMapping.setSpaceTimeMapForMemoryAllocation((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4], (AffineFunction) params[5]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer)){
//				TargetMapping.setSpaceTimeMapForMemoryAllocation((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setSpaceTimeMapForValueCopy":
			if((params.length == 6) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer) && (params[5] instanceof AffineFunction)){
				TargetMapping.setSpaceTimeMapForValueCopy((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4], (AffineFunction) params[5]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer)){
//				TargetMapping.setSpaceTimeMapForValueCopy((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setSpaceTimeMapForMemoryFree":
			if((params.length == 6) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer) && (params[5] instanceof AffineFunction)){
				TargetMapping.setSpaceTimeMapForMemoryFree((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4], (AffineFunction) params[5]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer)){
//				TargetMapping.setSpaceTimeMapForMemoryFree((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setSpaceTimeMapForUseEquationOptimization":
			if((params.length == 8) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer) && (params[5] instanceof AffineFunction) && (params[6] instanceof AffineFunction) && (params[7] instanceof AffineFunction)){
				TargetMapping.setSpaceTimeMapForUseEquationOptimization((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4], (AffineFunction) params[5], (AffineFunction) params[6], (AffineFunction) params[7]);
				memento.recordAction(input, (Program) st.get(progName));
			}
//			else if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer)){
//				TargetMapping.setSpaceTimeMapForUseEquationOptimization((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4]);
//				memento.recordAction(input, (Program) st.get(progName));
//			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setMemorySpaceForUseEquationOptimization":
			if((params.length == 6) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer) && (params[5] instanceof String)){
				TargetMapping.setMemorySpaceForUseEquationOptimization((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4], (String) params[5]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setBandForTiling":
			if((params.length == 7) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof String) && (params[5] instanceof Integer) && (params[6] instanceof Integer)){
				TargetMapping.setBandForTiling((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (String) params[4], (Integer) params[5], (Integer) params[6]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 6) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer) && (params[5] instanceof Integer)){
				TargetMapping.setBandForTiling((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4], (Integer) params[5]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setSubTilingWithinBand":
			if((params.length == 7) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof Integer) && (params[5] instanceof Integer) && (params[6] instanceof String)){
				TargetMapping.setSubTilingWithinBand((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (Integer) params[4], (Integer) params[5], (String) params[6]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String) && (params[3] instanceof Integer) && (params[4] instanceof String)){
				TargetMapping.setSubTilingWithinBand((Program) params[0], (String) params[1], (String) params[2], (Integer) params[3], (String) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		case "setDefaultDTilerConfiguration":
			if((params.length == 5) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof Integer) && (params[3] instanceof Integer) && (params[4] instanceof String)){
				TargetMapping.setDefaultDTilerConfiguration((Program) params[0], (String) params[1], (Integer) params[2], (Integer) params[3], (String) params[4]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else if((params.length == 3) && (params[0] instanceof Program) && (params[1] instanceof String) && (params[2] instanceof String)){
				TargetMapping.setDefaultDTilerConfiguration((Program) params[0], (String) params[1], (String) params[2]);
				memento.recordAction(input, (Program) st.get(progName));
			}
			else{
				hp.printHelp(func);
			}
			break;
		
		case "history":
			if(params.length == 0)
			{
				for(String cmd: history)
				{
					System.out.println(cmd);
				}
			}
			else {
				try {
					int num = (Integer) params[0];
					for(int i = history.size() - 1 - num ; i > 0; i++)
					{
						String op = String.format("%4d\t%s", i+1, history.get(i));
						System.out.println(op);
					}
				}
				catch(ClassCastException e)
				{
					System.out.println("Invalid argument: " + params[0].toString());
				}
				
			}
			break;
	
//		case "searchHistory":
//			if(params.length != 1)
//			{
//				try {
//					String pat = (String) params[0];
//					for(int i = 0; i < history.size(); i++)
//					{
//						if(history.get(i).contains(pat)) {
//							String op = String.format("%4d\t%s", i+1, history.get(i));
//							System.out.println(op);
//						}
//					}
//				}
//				catch(PatternSyntaxException e)
//				{
//					System.out.println("Invalid regex or string: " + params[0]);
//				}
//			}
//			else {
//				System.out.println("Invalid syntax: "+input);
//			}
//			break;
	
		case "undo":
			if(params.length == 0)
			{
				result = memento.undo(1);
			}
			else {
				try {
					int num = (Integer) params[0];
					result = memento.undo(num);
				}
				catch(NumberFormatException e)
				{
					System.err.println("Invalid argument: " + params[0].toString());
					return;
				}
			}
			st.remove(progName);
			st.put(progName, result);
			break;
			
		case "redo":
			if(params.length == 0)
			{
				result = memento.redo(1);
			}
			else {
				try {
					int num = (Integer) params[0];
					result = memento.redo(num);
				}
				catch(NumberFormatException e)
				{
					System.out.println("Invalid argument: " + params[0].toString());
				}
			}
			st.remove(progName);
			st.put(progName, result);
			break;
	
		case "printTransform":
			memento.printTransform();
			break;
		case "delVar":
			String varName=String.valueOf(params[0]);
			if(varName.equals(progName)) {
				System.out.println("Removing program from context...");
				memento.clear();
			}
			if(st.contains(varName)) {
				st.remove(varName);
			} else {
				System.err.println("Attempted to remove a variable that is not declared.");
				return;
			}
			break;
		case "print":
			if(params.length==1 &&((params[0] instanceof String) || params[0] instanceof Domain || params[0] instanceof AffineFunction)){
				System.out.println(params[0]);
			}
			else{
				System.err.println("Unsupported parameter type");
				return;
			}
			break;
		case "clear":
			clear();
			break;
		case "help":
			String f = (String) params[0];
			hp.printHelp(f);
			break;
		default:
			System.err.println("No matching function defined.");
			return;
		}
		if(methodMap.containsKey(func) && assignVar != null){
			st.put(assignVar, result);
		}
	}
	
	/**
	 * <h2> clear </h2>
	 * Clears the current context - Symbol Table, program, and memento.
	 */
	private void clear()
	{
		if(memento !=null){
			memento.clear();
			memento = null;
		}
		st.clear();
	}
}