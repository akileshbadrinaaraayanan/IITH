/*This is a file server that receives a file.
 *
 * Usage: java FileReceiver <port> <name of file received>
 */

import java.util.*;
import java.io.*;

class FileReceiver 
{	
	public static void main(String args[])
	{
		if (args.length < 2) 
		{
			System.err.println("Correct usage: java FileReceiver <port> <filename>");
			return;
		} 
		int port = Integer.parseInt(args[0]);
		String filename = args[1];
        
		try 
		{
			// Create new reliable transport object and file output stream.
			
			RDTReceiver rdt = new RDTReceiver(port);        
			FileOutputStream fos = new FileOutputStream(filename);
	        
			// Repeatedly receive data from the reliable transport until
			// the end of file.
			byte data[] = rdt.recv();
			while (data != null)
			{
				fos.write(data);
				data = rdt.recv();
			}
			System.out.println("Successfully received");
			rdt.close();	//close both
			fos.close();
		} 
		catch (IOException e) 
		{
		} 
		catch (ClassNotFoundException e) 
		{
		}
    }
}