import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

// Main class
public class Main {

	ServerSocket serverSocket;			//create a serverSocket
	
	public static void main(String[] args) throws IOException
	{
		int port = Integer.parseInt(args[0]);
		
		new Main().startServer(port);			//start the main server
	}
	public void startServer(int port) throws IOException
	{
		System.out.println("server is started");
		serverSocket = new ServerSocket(port);		// Specify the port
		
		acceptRequests();		// Keep accepting requests
	}
	
	private void acceptRequests() throws IOException 
	{
		while(true)				//as long as it is true keep accepting requests
		{
			Socket s = serverSocket.accept();
			
			ConnectionHandler c = new ConnectionHandler(s);		//object of connection handler
			c.start();
		}
	}
}
