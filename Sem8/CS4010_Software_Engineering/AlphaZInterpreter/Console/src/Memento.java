import java.util.ArrayList;
import java.util.Stack;

import org.polymodel.polyhedralIR.*;

/**
 * <h1> Memento </h1>
 * Memento class which caches the state of the program after each change for the purpose of undo/redo
 * It basically does this by maintaining two stacks - an undoStack and a redoStack, to which we push
 * on each action and undo respectively.
 * Another auxiliary function of the Memento class is to record the net change carried out to the program 
 * after it has been read and a complex sequence of operations has been carried out on it.
 * @author Surya Teja Chavali
 *
 */
public final class Memento
{
	private Program currState;

	private Stack<Program> undoStack = new Stack<Program>();
	private Stack<Program> redoStack = new Stack<Program>();

	private ArrayList<String> undone = new ArrayList<String>();
	private ArrayList<String> transformations = new ArrayList<String>();

	/**
	 * <h2> Memento() </h2>
	 * Constructor of an instance of Memento, with initial state as Program p.
	 * @param p The initial state of the Memento.
	 */
	public Memento(Program p)
	{
		currState = p.copy();
	}

	/**
	 * <h2> clear() </h2>
	 * Clears the current state of the Memento.
	 */
	public void clear()
	{
		currState = null;
		undoStack.clear();
		redoStack.clear();
		undone.clear();
		transformations.clear();
	}
	
	/**
	 * <h2> recordAction </h2>
	 * Records the current action by recording the command, as well as the current program state.
	 * Updates the undo stack, clears redo stack, and updates the net transformation list.
	 * @param action String representing the command.
	 * @param program State of the program after the command executes.
	 */
	public final void recordAction(String action, Program program)
	{
		undoStack.push(currState.copy());
		redoStack.clear();
		undone.clear();
		transformations.add(action);
		currState = program.copy();
	}

	/**
	 * <h2> canUndo </h2>
	 * Checks whether it is possible to undo at a given time.
	 * @return boolean representing whether it is possible to undo.
	 */
	public final boolean canUndo()
	{
		return !undoStack.empty();
	}

	/**
	 * <h2> canRedo </h2>
	 * Checks whether it is possible to redo at a given time.
	 * @return boolean representing whether it is possible to redo.
	 */
	public final boolean canRedo()
	{
		return !redoStack.empty();
	}

	/**
	 * <h2> undo </h2>
	 * Undo the last 'n' actions.
	 * Update state variables accordingly.
	 * @param n The number of actions to undo
	 * @return State of the program before the action.
	 */
	public final Program undo(int n)
	{
		int n0 = n;
		while(!undoStack.empty() && n > 0)
		{
			String last = transformations.get(transformations.size() - 1);
			undone.add(last);
			System.out.println("Undid: " + last);
			transformations.remove(transformations.size() - 1);
			redoStack.push(currState.copy());
			currState = undoStack.pop();
			n--;
		}

		if(n > 0) 
			System.out.println("Could not undo all " + n0 + " actions");
	
		return currState;
	}

	/**
	 * <h2> redo </h2>
	 * ReDo the last 'n' undone actions.
	 * Update state variables accordingly.
	 * @param n The number of actions to redo
	 * @return State of the program after redo.
	 */
	public final Program redo(int n)
	{
		int n0 = n;
		while(!redoStack.empty() && n > 0)
		{
			String last = undone.get(undone.size() -1);
			transformations.add(last);
			System.out.println("Redid: " +  last);
			undone.remove(undone.size() - 1);
			undoStack.push(currState.copy());
			currState = redoStack.pop();
			n--;
		}
		
		if(n > 0) 
			System.out.println("Could not redo all " + n0 + " actions");

		return currState;
	}

	/**
	 * <h2> printTransform </h2>
	 * Print the effective transformation carried out so far. 
	 * Different from history, which also keeps track of actions which have been undone.
	 */
	public final void printTransform()
	{
		for(String cmd: transformations)
		{
			System.out.println(cmd);	
		}
	}
};