import java.io.Serializable;
import java.util.Arrays;


public class RDTPacket implements Serializable {
	
	public byte[] data;
	
	public boolean last;
	
	public int seq, checkSum;

	public RDTPacket(int seq, byte[] data, boolean last, int checkSum)
	{
		super();
		this.seq = seq;
		this.data = data;
		this.last = last;
	}

	public int getSeq() {
		return seq;
	}

	public void setSeq(int seq) 
	{
		this.seq = seq;
	}

	public byte[] getData() 
	{
		return data;
	}

	public void setData(byte[] data) 
	{
		this.data = data;
	}

	public boolean isLast()
	{
		return last;
	}

	public void setLast(boolean last) {
		this.last = last;
	}
	/*public static long calculateCheckSum(byte[] arr)
	{
			int length = arr.length;
			long sum = 0;
			long data;
			int i=0;
			while(length > 1)
			{
			      data = (((arr[i] << 8) & 0xFF00) | ((arr[i+1]) & 0xFF));
			      sum+=data;
			      if((sum & 0xFFFF0000) > 0)
			      {
			          sum = sum & 0xFFFF;
			          sum += 1;
			      }
			      i=i+2;
			      length=length-2;
			}
			return sum;
	}*/
	@Override
	public String toString() {
		return "UDPPacket [seq=" + seq + ", data=" + Arrays.toString(data)
				+ ", last=" + last + "]";
	}
	
}