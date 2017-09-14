import java.io.*;

import java.net.*;
import java.util.*;

public class NSLookup 
{
  public static void main (String[] args) 
  {
	  
    if (args.length != 2)
      throw new IllegalArgumentException ("syntax: nslookup -type=<type> <host> or nslookup <host>");
    
    int eqIdx = args[0].indexOf ("=");
    
    int spIdx = args[0].indexOf(" ");
     
    int constant = 0, default_port = 53, connect_internet=1;
    
    if(eqIdx != -1 && spIdx != -1)
    {
    	 String type = args[0].substring(eqIdx+1, spIdx);
    	 System.out.println("type of request is "+type);
    	 System.out.println();
    	 
    	 if(type.equals("ns"))
    	 	{
    		 constant = 2;
    	 	}
    	 else if(type.equals("a"))
    	 	{
    		 constant = 1;
    	 	}
    	 else if(type.equals("cname"))
    	 {
    		 constant = 5;
    	 }
    	 else if(type.equals("soa"))
    	 {
    		 constant = 6;
    	 }
    	 else if(type.equals("mx"))
    	 {
    		 constant = 15;
    	 }
    }
    else
    {
    	constant = 1;
    }
    
    String hostName = args[0].substring(spIdx+1);
    
    String nameServer = args[1];
    
   
    System.out.println ("Server: " +nameServer);
    System.out.println("Address: "+nameServer+"#"+default_port);
    System.out.println ("Request: " + hostName);
    
    DNSQuery query = new DNSQuery (hostName, constant, connect_internet);
    try 
    {
      Socket socket = new Socket (nameServer, default_port);
      socket.setSoTimeout (10000);
      
      sendQuery (query, socket);		//send the query to the server
      getResponse (query, socket); 		//get response from the server
      
      socket.close ();			//close the socket
      printRRs (query);			//to print the resource records
    } 
    catch (IOException ex) 
    {
      System.out.println (ex);
    }
  }
  public static void sendQuery (DNSQuery query, Socket socket) throws IOException
  {
    BufferedOutputStream bufout = new BufferedOutputStream (socket.getOutputStream ());
    DataOutputStream dataOut = new DataOutputStream (bufout);
    
    byte[] data = query.extractQuery ();
    dataOut.writeShort (data.length);
    
    dataOut.write (data);
    dataOut.flush ();
  }

  public static void getResponse (DNSQuery query, Socket socket) throws IOException 
  {
    InputStream bufferedIn = new BufferedInputStream (socket.getInputStream ());
    DataInputStream dataIn = new DataInputStream (bufferedIn);
    
    int responseLength = dataIn.readUnsignedShort ();
    byte[] data = new byte[responseLength];		//allocate space equal to the response length
    
    dataIn.readFully (data);
    query.receiveResponse (data, responseLength);	//receive response
  }

  public static void printRRs (DNSQuery query) 
  {
    Enumeration<DNSRR> answers = query.getAnswers();
    
    if(answers.hasMoreElements())
    {
    	if(query.isAuthoritative())
    	{
    		System.out.println("\n Authoritative answer: \n");
    	}
    	else
    	{
    		System.out.println("\nNon-authoritative answer:\n");
    	}
    }
    while(answers.hasMoreElements())
      System.out.println (answers.nextElement());
    
    Enumeration<DNSRR> authorities = query.getAuthorities();
    
    if(authorities.hasMoreElements())
      System.out.println ("\nAuthoritative answers can be found from:\n");
    
    while(authorities.hasMoreElements())
      System.out.println (authorities.nextElement());
    
    Enumeration<DNSRR> additional = query.getAdditional();
    
    if(additional.hasMoreElements())
      System.out.println ("\nAdditional information:\n");
    
    while(additional.hasMoreElements())
      System.out.println (additional.nextElement ());
  }
}
