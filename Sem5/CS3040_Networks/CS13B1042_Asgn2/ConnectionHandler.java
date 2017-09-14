import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ConnectionHandler extends Thread
{
	Socket s;			//socket
	
	BufferedReader b;		//for getting input from client
	PrintWriter p;			//for sending output to client
	
	public ConnectionHandler(Socket s) throws IOException	//Constructor
	{
		this.s = s;
		b = new BufferedReader(new InputStreamReader(s.getInputStream()));
		p = new PrintWriter(s.getOutputStream());
	}
	
	public void run()
	{
		try {
			
		String requests = "";		//get the requests
		
		while(requests.length() == 0 || b.ready())		// read the requests from buffered reader
			requests += (char) b.read();
		
		
		System.out.println(requests);   	// printing the requests
		
		HttpRequest req = new HttpRequest(requests);	//pass the requests to HttpRequest class
		
		HttpResponse res = new HttpResponse(req);
		
		p.write(res.response.toCharArray());   //write output to print writer
		
		p.close();
		b.close();		//closing I/O
		
		s.close();		//closing the socket
		
		}catch(Exception e)
		{
			e.printStackTrace();
		}
	}
}
