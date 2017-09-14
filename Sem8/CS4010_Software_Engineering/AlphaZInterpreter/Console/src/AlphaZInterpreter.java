import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.nio.file.Files;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * 
 * @author Hrishikesh Vaidya
 *
 */

public class AlphaZInterpreter {
	
	public static void main(String[] args) throws IOException, ClassNotFoundException, NoSuchMethodException, SecurityException, IllegalAccessException, IllegalArgumentException, InvocationTargetException, InterruptedException {
		Scanner reader = new Scanner(System.in);
		CommandProcessor processor = new CommandProcessor();
		while(true){
			Thread.sleep(1500);
			System.out.print(">>>");
			String s=reader.nextLine();
			Pattern p= Pattern.compile("(\\w+)[(](\"[^\"]*\")[)];");
			if(s.contains("readScript")){
				Matcher m = p.matcher(s);
				if(m.find()){
					if(m.group(1).equals("readScript")){
						String filename = m.group(2);
						if(filename.startsWith("\"") && filename.endsWith("\"")){
							try{
								filename = filename.replaceAll("\"", "");
								Scanner scan = new Scanner(new File(filename));
							    while(scan.hasNextLine()){
							        String line = scan.nextLine();
							        if(line.startsWith("#") || line.isEmpty())
							        	continue;
							        processor.computeFunc(line);
							    }
							}
							catch(FileNotFoundException e){
								System.err.println("Filepath does not exist");
							}
								
						}
						else{
							System.err.println("Incorrect parameter types");
						}
						
					}
					else{
						System.err.println("Function does not exist");
					}
				}
			}
			else if(s.equals("exit")){
				System.out.println("Program terminated");
				break;
			}
			else if(s != null && !s.isEmpty())
				processor.computeFunc(s);
			else
				continue;
		}
		reader.close();
	}

}
