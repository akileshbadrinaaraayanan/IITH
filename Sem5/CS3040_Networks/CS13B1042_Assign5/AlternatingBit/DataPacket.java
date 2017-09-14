import java.util.*;

class DataPacket implements java.io.Serializable {
	public int seq;
	public byte data[];
	public int length;
	public boolean isCorrupted;
	
	DataPacket(byte data[], int length, int seq)
	{
		if (length > 0) {
			this.data = new byte[length];
			System.arraycopy(data, 0, this.data, 0, length);
		} else {
			this.data = null;
		}
		this.seq = seq;
		this.length = length;
		this.isCorrupted = false;
	}
}