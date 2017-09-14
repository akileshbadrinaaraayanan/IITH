/*
 * It is an unreliable data sender.
 */

import java.io.*;
import java.util.*;
import java.net.*;
 
class UDTSender 
{
	Socket s;
	ObjectInputStream is;
	ObjectOutputStream os;
	Random random;

	static double drop_probability = 0.2;
	static double corrupt_probability = 0.2;

	UDTSender (String hostname, int port) throws IOException
	{
		// Connect to TCP server at the given hostname and port.
		s = new Socket(hostname, port);

		os = new ObjectOutputStream(s.getOutputStream());
		is = new ObjectInputStream(s.getInputStream());

		// Initialize random number generator.
		random = new Random();
	}

	void send(DataPacket p) throws IOException
	{
		System.out.println("Sender sends " + p.seq);
		
		os.writeObject(p);
		os.flush();
	}

	AckPacket recv() throws IOException, ClassNotFoundException
	{
		Object obj = is.readObject();
		while (random.nextDouble() < drop_probability) 
		{
			System.out.println("packet drop at sender");
			obj = is.readObject();
		}
		AckPacket ack = (AckPacket)obj;

		// randomly corrupt the received packet.
		if (random.nextDouble() < corrupt_probability) 
		{
			System.out.println("corrupt ACK at sender" + ack.ack);
			// create a return a new corrupted ACK.
			ack = new AckPacket(random.nextInt(1));
			ack.isCorrupted = true;
		}
		System.out.println("received ACK at sender" + ack.ack);
		return ack;
	}

	/*void close() throws IOException
	{
		s.close();
	}*/
}