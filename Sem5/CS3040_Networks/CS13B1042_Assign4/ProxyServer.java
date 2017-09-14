import java.net.*;
import java.io.*;
import java.util.*;

// Main class
public class ProxyServer 
{
	public static void main(String args[])
	{
		DatagramSocket serverSocket = null;			//serverSocket

		try
		{
			int ser_port = Integer.parseInt(args[0]);		//server port number
			serverSocket = new DatagramSocket(ser_port);		

			byte[] receiveData = new byte[512];

			int max_size = Integer.parseInt(args[1]);		//max cache size
			HashMap<String, byte[]> cache = new HashMap<String, byte[]>(max_size);		//cache is a hashmap of string and bytearray
			ArrayList<String> list = new ArrayList<String>();		//to keep track of order of entries in cache

			String nameServer = args[2];		//get the name server

			DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);

			System.out.println("created server socket");

			while(true)
			{
				serverSocket.receive(receivePacket);
				byte[] data = receivePacket.getData();
				String str = new String(data, 0, receivePacket.getLength());


				InetAddress IPAddress = receivePacket.getAddress();
				int port = receivePacket.getPort();

				byte[] modsendstr1 = Arrays.copyOfRange(data, 2, data.length);		//remove the headers - first two bytes
				byte[] head = Arrays.copyOfRange(data, 0, 2);

				String ssend1 = new String(modsendstr1);

				if(cache.containsKey(ssend1))		//check if its already present in cache
				{
					System.out.println("It is already cached! Retrieving from cache");
					byte[] b = new byte[512];
					b = cache.get(ssend1);		//get value of corresponding key

					ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
					outputStream.write(head);
					outputStream.write(b);
					byte[] c = outputStream.toByteArray();		//before sending the response back, append the header again

					DatagramPacket sendPacket1 = new DatagramPacket(c, c.length, IPAddress, port);
					serverSocket.send(sendPacket1);
				}

				else		//if not already present in cache
				{
					DatagramPacket sendPacket = (new ProxyClient()).sendtoClient(receivePacket, nameServer);	//send request to local DNS server

					byte[] sendData = sendPacket.getData();
					byte[] modsendData = Arrays.copyOfRange(sendData, 2, sendData.length);	//remove headers

					byte[] modsendstr = Arrays.copyOfRange(data, 2, data.length);

					String ssend = new String(modsendstr);

					list.add(ssend);

					if(cache.size() < max_size)		//if there is still space in cache directly add
						cache.put(ssend, modsendData);
					else
					{
						cache.remove(list.get(0));		//if not, remove the oldest entry in cache and add new entry
						list.remove(0);
						cache.put(ssend, modsendData);
					}

					DatagramPacket sendPacket1 = new DatagramPacket(sendData, sendData.length, IPAddress, port);
					serverSocket.send(sendPacket1);              //send the response
				}
				System.out.println("cache size is "+cache.size());				
			}

		}

		catch(IOException e)
		{
			System.err.println("IOException "+e);
		}
	}
}
