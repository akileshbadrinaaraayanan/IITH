import java.util.HashMap;

/**
 * <h1> SymbolTable </h1>
 * A basic Symbol Table class.
 * @author Hrishikesh Vaidya.
 * 
 */
public class SymbolTable {
	
	private HashMap<String, Object> st = new HashMap<>();
	
	/**
	 * <h2> put </h2>
	 * Add a variable to context of the interpreter.
	 * @param key String Variable Name
	 * @param val Object Variable Value
	 */
	public void put(String key, Object val)
	{
		st.put(key,val);
	}
	
	/**
	 * <h2> get </h2>
	 * Get the value of the variable.
	 * @param key Variable Name
	 * @return Variable Value 
	 */
	public Object get(String key){
		return st.get(key);
	}
	
	/**
	 * <h2> contains </h2>
	 * Checks if a given variable exists in context.
	 * @param key Variable Name
	 * @return flag indicating whether variable is in context
	 */
	public boolean contains(String key){
		return st.containsKey(key);
	}
	
	/**
	 * <h2> remove </h2>
	 * Removes Variable from context.
	 * @param key Variable Name
	 */
	public void remove(String key){
		st.remove(key);
	}
	
	/**
	 * <h2> clear </h2>
	 * Clear the state of the symbol table.
	 */
	public void clear()
	{
		st.clear();
	}
	
}
