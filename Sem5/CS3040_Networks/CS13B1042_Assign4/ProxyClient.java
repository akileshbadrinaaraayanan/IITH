import java.io.*;
import java.net.*;

//Main class
public class ProxyClient {

	public DatagramPacket sendtoClient(DatagramPacket receivePacket, String nameServer) 
	{
		DatagramSocket clientSocket = null;
		DatagramPacket sendPacket = null;

		int port = 53;		//Standard DNS port

		try
		{
			clientSocket = new DatagramSocket();		//create a client socket
			clientSocket.setSoTimeout(10000);			//set timeout

			InetAddress IPAddress1 = InetAddress.getByName(nameServer);
			byte[] by = receivePacket.getData();

			DatagramPacket sp = new DatagramPacket(by, by.length, IPAddress1, port);
			clientSocket.send(sp);

			byte[] c = new byte[512];		//get the response
			sendPacket = new DatagramPacket(c, c.length);
			clientSocket.receive(sendPacket);

			byte[] dt = sendPacket.getData();
			String s = new String(dt, 0, sendPacket.getLength());

		}

		catch(IOException e)
		{
			System.err.println("IOException "+e);
		}

		return sendPacket;		//return it to proxy server
	}

}
