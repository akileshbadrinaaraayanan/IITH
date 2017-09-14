import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketTimeoutException;
import java.util.ArrayList;
import java.util.Arrays;


public class Sender {

	// Probability of loss during packet sending
	public static final double loss_probability = 0.1;
	public static final double corrupt_probability = 0.1;
	
	// Maximum Segment Size, window size and time before resending all packets
	public static final int MSS = 4, WINDOW_SIZE = 2, TIMER = 30;

	public static void main(String[] args) throws Exception{

		// Sequence number of the last acked packet and last packet sent 
		int waitingForAck = 0, lastSent = 0;

		byte[] fileBytes = "ABCDEFGHIJKLMNOPQRSTUVXZ".getBytes();

		// Last packet sequence number
		int lastSeq = (int) Math.ceil( (double) fileBytes.length / MSS);

		System.out.println("Count of packets to send: " + lastSeq);

		DatagramSocket toReceiver = new DatagramSocket();

		InetAddress receiverAddress = InetAddress.getByName("localhost"); 		// Receiver address
		
		ArrayList<RDTPacket> sent = new ArrayList<RDTPacket>();		// List of all the packets sent

		while(true){

			// Sending loop
			while(lastSent < lastSeq && lastSent - waitingForAck < WINDOW_SIZE){

				// Array to store part of the bytes to send
				byte[] filePacketBytes = new byte[MSS];

				// Copy segment of data bytes to array
				filePacketBytes = Arrays.copyOfRange(fileBytes, lastSent*MSS, lastSent*MSS + MSS);

				// Create RDTPacket object
				RDTPacket rdtPacketObject = new RDTPacket(lastSent, filePacketBytes, (lastSent == lastSeq-1) ? true : false, 0);

				// Serialize the RDTPacket object
				byte[] sendData = Serializer.toBytes(rdtPacketObject);

				// Create the packet
				DatagramPacket packet = new DatagramPacket(sendData, sendData.length, receiverAddress, 9876 );

				System.out.println("Sending packet with seq no " + lastSent +  " and size " + sendData.length + " bytes");

				// Add packet to the sent list
				sent.add(rdtPacketObject);
				
				double b = Math.random();
				// Send with some probability of loss
				if(b > loss_probability && b > corrupt_probability){
					toReceiver.send(packet);
				}else{
					System.out.println("Lost packet with seq no " + lastSent);
				}

				// Increase the last sent
				lastSent++;

			} // End of sending while
			
			// Byte array for the ACK sent by the receiver
			byte[] ackBytes = new byte[40];
			
			// Creating packet for the ACK
			DatagramPacket ack = new DatagramPacket(ackBytes, ackBytes.length);
			
			try{
				toReceiver.setSoTimeout(TIMER);
				
				// Receive the packet
				toReceiver.receive(ack);
				
				// Unserialize the RDTAck object
				RDTAck ackObject = (RDTAck) Serializer.toObject(ack.getData());
				
				System.out.println("Received ACK for " + ackObject.getPacket());
				
				// If this ack is for the last packet, stop the sender (Note: gbn has a cumulative acking)
				if(ackObject.getPacket() == lastSeq){
					break;
				}
				
				waitingForAck = Math.max(waitingForAck, ackObject.getPacket());
				
			}catch(SocketTimeoutException e){
				// then send all the sent but non-acked packets
				
				for(int i = waitingForAck; i < lastSent; i++){
					
					// Serialize the RDTPacket object
					byte[] sendData = Serializer.toBytes(sent.get(i));

					// Create the packet
					DatagramPacket packet = new DatagramPacket(sendData, sendData.length, receiverAddress, 9876 );
					
					double a = Math.random();
					// Send with some probability
					if(a > loss_probability && a > corrupt_probability)
					{
						toReceiver.send(packet);
					}else{
						System.out.println("Lost packet with seq no " + sent.get(i).getSeq());
					}

					System.out.println("Resending packet with seq no " + sent.get(i).getSeq() +  " and size " + sendData.length + " bytes");
				}
			}		
		}
		
		System.out.println("Finished transmission");

	}

}