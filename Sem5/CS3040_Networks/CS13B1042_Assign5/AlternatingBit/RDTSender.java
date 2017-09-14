import java.io.*;
import java.util.*;

class RDTSender 
{
	UDTSender udt;
    int seqNumber;

	RDTSender(String hostname, int port) throws IOException
	{
		udt = new UDTSender(hostname, port);
        seqNumber = 0;
	}

	void send(byte[] data, int length) throws IOException, ClassNotFoundException
	{
        // send packet
		DataPacket p = new DataPacket(data, length, seqNumber);
		Timer timer = new Timer();

		sendPacket(p, timer);

		int notSentSuccessfully = 1;

		while (notSentSuccessfully == 1) {
			// receive ACK
			AckPacket ack = udt.recv();

			int ackNumber = ack.ack;

			// sequence number should be the same as the sent data packet
			if (ackNumber == seqNumber && !ack.isCorrupted) {
				notSentSuccessfully = 0;
				timer.cancel();
			}
		}
        // Update sequence number
        seqNumber = (seqNumber + 1) % 2;
        return;
	}

	void sendPacket(DataPacket p, Timer timer) throws IOException, ClassNotFoundException
	{
		udt.send(p);
        timer.schedule(new SenderTimer(this, p, timer), 50);
	}

				// It is called when there is no more data to send. 
	void close() throws IOException, ClassNotFoundException
	{
		DataPacket p = new DataPacket(null, 0, seqNumber);
		udt.send(p);
		
		try 
		{
			Timer timer = new Timer();

			sendPacket(p, timer);

			int notSentSuccessfully = 1;

			while (notSentSuccessfully == 1) 
			{
				AckPacket ack = udt.recv();	// receive ACK

				int ackNumber = ack.ack;

												//sequence number should be the same as the sent data packet
				if (ackNumber == seqNumber) 
				{
					notSentSuccessfully = 0;
					timer.cancel();
				}
			}
		} catch (EOFException e) 
		{
        } 
		finally 
		{
			//udt.close();
		}
	}

	private class SenderTimer extends TimerTask 
	{
		RDTSender rdtSender;
		DataPacket packet;
		Timer timer;

		SenderTimer(RDTSender s, DataPacket p, Timer t) 
		{
			rdtSender = s;
			packet = p;
			timer = t;
		}

		public void run() 
		{
			try 
			{
				rdtSender.sendPacket(packet, timer);
			}
			catch (ClassNotFoundException e) 
			{
			}
			catch (IOException e) 
			{
			}
        }
	}
}