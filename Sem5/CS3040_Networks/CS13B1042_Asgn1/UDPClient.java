import java.awt.Toolkit;
import java.io.*;
import java.net.*;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.*;

/*
 * Client program. Implemented by Akilesh B, CS13B1042. Assignment #1
 * 
 */

public class UDPClient 				// client class
{
	Toolkit toolkit;
	Timer timer;		// timer for scheduling the tasks
	private DatagramSocket clientSocket; 		//client socket	
    byte[] receiveData;			// receive data
    byte[] sendData;			// send data
    private InetAddress IPAddress;			//IP address
    private int port;
    
    public UDPClient(int pt) throws SocketException		//Constructor
    {
    	try {
			clientSocket = new DatagramSocket();			//creating a new socket
		} catch (SocketException e) {
			e.printStackTrace();
		}
        try {
			IPAddress = InetAddress.getByName("localhost");
		} catch (UnknownHostException e) {
			e.printStackTrace();
		}
        sendData = new byte[1024];
        receiveData = new byte[1024];	
      port = pt;
        toolkit = Toolkit.getDefaultToolkit();
        timer = new Timer();
        timer.schedule(new PerfAction(), 0, 1000);			//scheduling a task to repeatedly occur once every 1s
    }
    
	int counter=1, counter1=0;		//declaration of variables
	static int loss = 0;
	static float loss_rate, avg; 
    static long min=1000,t1,t2,t3, max=0, sum=0, sd=0;
   
   ArrayList<Long> list = new ArrayList<Long>();		//list for storing rtt 
   
	class PerfAction extends TimerTask
	{
		
	public void run()
		{
		if(counter <= 10){
				toolkit.beep();
			  String sentence = String.valueOf(counter);
		      sendData = sentence.getBytes();
		      DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, IPAddress, port);	//send packet
		      
		  
		      t1 = System.currentTimeMillis();		// time before sending the packet
		      try {
				clientSocket.send(sendPacket);			//sending the packet
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		      
		      ExecutorService service = Executors.newSingleThreadExecutor();		//service thread for waiting till 1 second to receive back the packet

		      try {
		    	  t3 = -1;
		          Runnable r = new Runnable() {
		              public void run() {
		            	  DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
		     		      try {
		     				clientSocket.receive(receivePacket);
		     			} catch (IOException e) {
		     				// TODO Auto-generated catch block
		     				e.printStackTrace();
		     			}    
		     		      t2 = System.currentTimeMillis();		//time after receiving the packet
		    		      String modifiedSentence = new String(receivePacket.getData());
		     		      
		     		      t3 = t2-t1;		// round trip time
		     		      	if((int)t3 != -1)
		     		      	{	if(t3 < min)		//updating value of max and min
		     		      			min = t3;
		     		      	
		     		      		if(t3 > max)
		     		      			max = t3;

		     		      sum = sum + t3;		// sum of all rtt's used for finding average
		     		      System.out.println("received again "+modifiedSentence);
		     		      
		     		     System.out.println("From "+IPAddress+" received "+receivePacket.getLength()+" bytes time is "+t3+" ms");
		     		      }	     			     		      	
		              }
		          };

		          Future<?> f = service.submit(r);

		          f.get(1, TimeUnit.SECONDS);     // This sets the timer to wait for for 1 second
		      }
		      catch (final InterruptedException e) {
		          // This thread was interrupted either during sleep, wait or join
		      }
		      catch (final TimeoutException e) {
		          //Task taking too long hence timed out
		    	}
		      catch (final ExecutionException e) {
		          // Exception in runnable task
		      }
		      finally {
		          service.shutdownNow();		// If we don't receive the packet within 1 sec, then shutdown this service
		      }
		      if((int) t3 != -1)
		      list.add(t3);		// adding rtt value to the list
		      
		      if(t3 == -1)
		      {
		    	  System.out.println("Packet "+counter+" timed out");
		    	  loss++;
		      }
		      counter++;
		 }
		else
		{
			toolkit.beep();
			
			if(10-loss == 0) {
				System.out.println();
		      System.out.println("------ ping statistics ----------");
		      System.out.println(counter-1+" packets transmitted, "+(10-loss)+" packets received, "+100+"% packet loss");
		      System.out.println("rtt min/avg/max/mdev = "+0+"/"+0+"/"+0+"/"+0+" ms");
		      
				System.exit(0);
			}
			else {
			 avg = (float)sum / (float)(10-loss);		// average rtt value
		      loss_rate = ((float)loss/(float)10)*100;		// loss_rate
		      
		      while(counter1 < list.size())
		      {
		    	  sd += Math.pow((list.get(counter1)-avg),2);		// calculating variance
		    	  counter1++;
		      }
		     
		      sd = (sd/(10-loss));		// calculating rtt value
		  
		      System.out.println();
		      System.out.println("------ ping statistics ----------");
		      System.out.println(counter-1+" packets transmitted, "+(10-loss)+" packets received, "+loss_rate+"% packet loss");
		      System.out.println("rtt min/avg/max/mdev = "+min+"/"+avg+"/"+max+"/"+Math.sqrt(sd)+" ms");
		      
		      System.exit(0);		//exiting in case of failure
			}
		}
		   
		}
	}
	
   public static void main(String args[]) throws Exception
   {
	   	int pt = Integer.parseInt(args[0]);
	   
        UDPClient client1 = new UDPClient(pt);		//client
    }
} 
