// RDTReceiver : Encapsulate a reliable data receiver that runs
 
import java.io.*;
import java.util.*;

class RDTReceiver 
{
	UDTReceiver udt;
	int seqNumber;

	RDTReceiver(int port) throws IOException
	{
		seqNumber = 0;
		udt = new UDTReceiver(port);
	}

	byte[] recv() throws IOException, ClassNotFoundException
	{
		DataPacket p = udt.recv();

		while (p.seq != seqNumber || p.isCorrupted)
		{
			AckPacket ack = new AckPacket((seqNumber + 1) % 2);
        	udt.send(ack);
        	p = udt.recv();
		}

        AckPacket ack = new AckPacket(p.seq);
        udt.send(ack);
        
        seqNumber = (seqNumber + 1) % 2;
        					
		if (p.length > 0)	// deliver data
		{
			byte [] copy = new byte[p.length];
			System.arraycopy(p.data, 0, copy, 0, p.length);
			return copy;
		} 
		else 
			return null;
	}

	void close() throws IOException
	{
		udt.close();
	}
}