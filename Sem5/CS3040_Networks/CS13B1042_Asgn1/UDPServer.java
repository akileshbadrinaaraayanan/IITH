import java.io.*;
import java.net.*;
import java.util.Random;
import java.util.Timer;
import java.util.TimerTask;

/*
 * Server program. Implemented by Akilesh B, CS13B1042. Assignment #1
 * 
 */
public class UDPServer extends Thread			// Server class UDPServer
{
	Timer timer;		//timer object used for scheduling
	private DatagramSocket serverSocket; 		//server socket
    byte[] receiveData;				// receive data
    byte[] sendData;				//send data
    private long AVERAGE_DELAY;			// delay after which packet is sent back
	private int max,min, LOSS_RATE, randomNum;
 	DatagramPacket sendPacket;			//send the datagram packet
    
	public UDPServer(int pt) 			// Constructor
    {
    	LOSS_RATE = 20;				//can be defined by the user
    	receiveData = new byte[1024];
    	sendData = new byte[1024];
    	min=1;
    	max=100;
    	try {
			serverSocket = new DatagramSocket(pt);		// creating a new socket
		} catch (SocketException e) {
			e.printStackTrace();
		}
    }
	
	public void run()
	{
		while(true)
		{
			try{
				Random rand = new Random();
				double MEAN = 500.0f;		// average delay is determined by this mean
		   		double VARIANCE = 49.0f;		// average delay is determined by this variance
		   		RandomGaussian gaussian = new RandomGaussian();
		   		AVERAGE_DELAY = (long)gaussian.getGaussian(MEAN, VARIANCE);
                DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
                serverSocket.receive(receivePacket);
                randomNum = rand.nextInt(max-min+1);
        
				timer = new Timer();
				
				if(AVERAGE_DELAY >= 1000)
					timer.cancel();
				else
					timer.schedule(new PerfAction(), AVERAGE_DELAY);			//to send the packet after a certain delay
                
				String sentence = new String( receivePacket.getData());
                InetAddress IPAddress = receivePacket.getAddress();		// getting the IP address from received packet
                int port = receivePacket.getPort();						// getting port number from received packet
                sendData = sentence.getBytes();
                sendPacket =
                new DatagramPacket(sendData, receivePacket.getLength(), IPAddress, port);
             }
          	catch(IOException e)
              {
                 e.printStackTrace();
              }
		}
	}
	
	class PerfAction extends TimerTask {
		public void run()
		{
			 try {
				 if(randomNum >= LOSS_RATE)		// randomly dropping the packet or sending it
				serverSocket.send(sendPacket);		// sending the packet back to the client
			} catch (IOException e) {
				e.printStackTrace();
			}
			 timer.cancel();			// timer is cancelled
		}
	}
   public static void main(String args[]) 	//main function to start the server
      {
	   		int pt = Integer.parseInt(args[0]);
	   		
	   	    UDPServer server1 = new UDPServer(pt);		//start the server
            server1.start();
      }
}
