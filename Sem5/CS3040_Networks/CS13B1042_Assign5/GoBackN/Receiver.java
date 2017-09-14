import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.ArrayList;


public class Receiver {
	
	// Probability of ACK loss
	public static final double loss_probability = 0.1;
	public static final double corrupt_probability = 0.1;
	
	public static void main(String[] args) throws Exception{
		
		DatagramSocket fromSender = new DatagramSocket(9876);
		
		int constant = 83, waitingFor = 0;	// 83 is the base size (in bytes) of a serialized RDTPacket object 
		
		byte[] receivedData = new byte[Sender.MSS + constant + Integer.SIZE];
		
		ArrayList<RDTPacket> received = new ArrayList<RDTPacket>();
		
		int end = 0;
		
		while(end == 0)
		{	
			System.out.println("Waiting for packet");
			
			// Receive packet
			DatagramPacket receivedPacket = new DatagramPacket(receivedData, receivedData.length);
			fromSender.receive(receivedPacket);
			
			// Unserialize to a RDTPacket object
			RDTPacket packet = (RDTPacket) Serializer.toObject(receivedPacket.getData());
			
			System.out.println("Packet with sequence number " + packet.getSeq() + " received (last: " + packet.isLast() + " )");
		
			if(packet.isLast() &&  packet.getSeq() == waitingFor)
			{	
				waitingFor++;
				received.add(packet);
				
				System.out.println("Last packet received");
				
				end = 1;	
			}else if(packet.getSeq() == waitingFor)
			{
				waitingFor++;
				received.add(packet);
				System.out.println("Packet is stored in buffer");
			}
			else
			{
				System.out.println("Packet is discarded as they are not in order");
			}
			
			// Create an RDTAck object
			RDTAck ackObject = new RDTAck(waitingFor);
			
			// Serialize
			byte[] ackBytes = Serializer.toBytes(ackObject);
			
			
			DatagramPacket ackPacket = new DatagramPacket(ackBytes, ackBytes.length, receivedPacket.getAddress(), receivedPacket.getPort());
			
			double a = Math.random();
			// Send with some probability of loss
			if(a > loss_probability && a > corrupt_probability)
			{
				fromSender.send(ackPacket);
			}else{
				System.out.println("Lost ack with sequence number " + ackObject.getPacket());
			}
			
			System.out.println("Sending ACK to seq no " + waitingFor + " with " + ackBytes.length  + " bytes");
			

		}
		
		// Print the data received
		System.out.println(" ********** DATA ********* ");
		
		for(RDTPacket p : received){
			for(byte b: p.getData()){
				System.out.print((char) b);
			}
		}
		
	}
	
	
}