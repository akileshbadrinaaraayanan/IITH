import java.io.*;
import java.util.*;
import java.net.*;
 
class UDTReceiver 
{
	Socket socket;
	ServerSocket serverSocket;
	Random random;
	ObjectInputStream ois;
	ObjectOutputStream os;
	
	static double drop_probability = 0.2, corrupt_probability = 0.2;

	
	UDTReceiver(int port) throws IOException {
		
		serverSocket = new ServerSocket(port);
		socket = serverSocket.accept();
		
		os = new ObjectOutputStream(socket.getOutputStream());
		ois = new ObjectInputStream(socket.getInputStream());	
		random = new Random();
	}
	
	DataPacket recv() throws IOException, ClassNotFoundException 
	{
		Object obj;
		obj = ois.readObject();
		while (random.nextDouble() < drop_probability) 
		{
			System.out.println("packet drop at receiver");
			obj = ois.readObject();
		}

		DataPacket dataPkt = ((DataPacket)obj);

									// Randomly corrupt the packet
		if (random.nextDouble() < corrupt_probability) 
		{
			System.out.println("corrupt at receiver " + dataPkt.seq);
			byte data[] = null;
			
			if (dataPkt.length > 0) 
			{
				data = new byte[dataPkt.length];
				random.nextBytes(data);
			} 
			dataPkt = new DataPacket(data, dataPkt.length, random.nextInt(1));
			dataPkt.isCorrupted = true;
		}
		System.out.println("receiver received " + dataPkt.seq);
		return dataPkt;
	}
	
	void send(AckPacket ack) throws IOException 
	{
		System.out.println("ACK sent by receiver" + ack.ack);
		os.writeObject(ack);
		os.flush();
	}
	
	void close() throws IOException
	{
		ois.close();
		os.close();
		socket.close();
		serverSocket.close();
	}
}