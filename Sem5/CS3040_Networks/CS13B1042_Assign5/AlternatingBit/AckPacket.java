import java.util.*;

class AckPacket implements java.io.Serializable 
{
	public int ack;
	public boolean isCorrupted;

	// Create a normal ACK packet with given ack number.
	AckPacket(int ack)
	{
		this.ack = ack;
		this.isCorrupted = false;
	}
}