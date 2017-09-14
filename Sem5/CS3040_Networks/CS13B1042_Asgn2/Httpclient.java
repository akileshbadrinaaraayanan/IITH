//HTTP Client class. Implemented by Akilesh B, cs13b1042
import java.io.*;
import java.net.*;
import java.util.*;

public class Httpclient
{
	public static void main(String args[]) throws IOException
	{
		if(args.length != 3)
		{
			System.out.println("Insufficient arguments");
		}
		Socket s = new Socket();
		
		String host = args[0];			//host
		
		int port = Integer.parseInt(args[1]);		//port number
		
		PrintWriter s_out = null;		//initialize printwriter
		BufferedReader s_in = null;		//initialize bufferedreader
		
		try
		{
			s.connect(new InetSocketAddress(host, port));		//connect to the host
			System.out.println("connected");
			
			s_out = new PrintWriter(s.getOutputStream(), true);		//send to the server
			
			s_in = new BufferedReader(new InputStreamReader(s.getInputStream()));	//reply from the server
		}
		catch(UnknownHostException e)
		{
			System.err.println("no idea about host "+host);
			System.exit(1);
		}
		
		String path = args[2];	//file to be downloaded
		
		//message is the  HTTP request to be sent to the server
		String message = "GET "+path+" HTTP/1.1\r\n";
		message += "Host: "+host+": "+port+"\r\n";
		message += "User-Agent: Myclient/5.0\r\n";
		message += "Connection: keep-alive\r\n\r\n";
		s_out.println(message);
		
		System.out.println(message);
		System.out.println("Message sent");
		
		StringBuilder result = new StringBuilder();
		String response;    //response is the reply received back from the server
		int count1 = 0;
		//read the response line by line
		while((response = s_in.readLine()) != null)
		{
			//System.out.println("hi");
			if(response.startsWith("<html>") || response.startsWith("<HTML>") || count1 == 1)
				{
				result.append(response);		//append the response to result
				result.append("\n");			
				count1 = 1;
				}
			System.out.println(response);
		}
	
		//Download it in result.html file
		File file = new File("result.html");
		FileWriter fileWriter = new FileWriter(file);
		fileWriter.write(result.toString());		//write the result received from the server to this file
		fileWriter.flush();
		fileWriter.close();
		
		//This part is for parsing the reply received. So that if it contains other linked pages, those files should also be downloaded
		ArrayList<String> match = new ArrayList<String>();
		StringTokenizer st = new StringTokenizer(result.toString());		//Tokenize the result with space as delimiter
		
		//As long as there are more elements
		while(st.hasMoreElements())
		{
			String test = st.nextElement().toString();
			if(test.startsWith("href"))			//if the word begins with href
			{
				StringTokenizer st1 = new StringTokenizer(test, "\"");		//get whatever is enclosed within quotation because this is the file name
				int count = 0;
				while(st1.hasMoreElements())
				{
					String temp = st1.nextElement().toString();
					if(count == 1)
						match.add(temp);		//add all the files to the array list.
					count++;
				}
			}
		}
		
		s_out.close();		//Close the I/O
		s_in.close();
		
		s.close();		//Close the socket
		
		
		 for(int i=0; i<match.size(); i++)	//iterate through the elements of match arraylist. It contains other files which needs to be downloaded
		{
		Socket s_new = new Socket();		//open a new socket
		s_new.connect(new InetSocketAddress(host, port));	//connect to host
		PrintWriter s_out1 = new PrintWriter(s_new.getOutputStream(), true);
		
		BufferedReader s_in1 = new BufferedReader(new InputStreamReader(s_new.getInputStream()));
		
		//GET requests for all the new files to be downloaded
		String message1 = "GET /"+match.get(i).toString()+" HTTP/1.1\r\n";
		message1 += "Host: "+host+": "+port+"\r\n";
		message1 += "User-Agent: Myclient/5.0\r\n";
		message1 += "Connection: keep-alive\r\n\r\n";
		s_out1.println(message1);
		
		System.out.println("message sent");
		
		/* Again getting the reply from the server
		 * and storing the reply in result1
		 */
		StringBuilder result1 = new StringBuilder();
		String response1;
		int count2 = 0;
		while((response1 = s_in1.readLine()) != null)
		{
				if(response1.startsWith("<html>") || response1.startsWith("<HTML>") || count2 == 1)
				{
					result1.append(response1);
					result1.append("\n");
					count2 = 1;
				}
		}
	
		//Storing the result in their respective files
		System.out.println("downloading file "+match.get(i).toString());
		File file1 = new File("result_"+match.get(i).toString()+".html");
		FileWriter fileWriter1 = new FileWriter(file1);
		fileWriter1.write(result1.toString());
		fileWriter1.flush();
		fileWriter1.close();
		}
		
	}
}