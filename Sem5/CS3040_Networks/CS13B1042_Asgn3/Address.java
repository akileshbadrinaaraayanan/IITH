import java.io.*;
import java.net.*;

//Class for Address
public class Address extends DNSRR 
{
  private int[] ipAddress = new int[4];
  
  //decode accordingly
  protected void decode(DNSInputStream dnsIn) throws IOException 
  {
    for (int i=0;i<4;i++)
    {
      ipAddress[i] = dnsIn.readByte(); //assigns value to ipAddress array
    }
  }
  
  public byte[] getAddress() 				//type cast the byte and returns the address
  {
    byte[] ip = new byte[4];
    
    for (int j=0;j<4;j++)
    {
      ip[j] = (byte)ipAddress[j];		//type case into byte
    }
    
    return ip;							//return ip
  }
  
  //returns INetAddress
  public InetAddress getInetAddress() throws UnknownHostException 
  {
    return InetAddress.getByName(toByteString());
  }
  
  
  private String toByteString() 
  {
    return ipAddress[0] + "." + ipAddress[1] + "." +		//prints in the form of actual IP Address
      ipAddress[2] + "." + ipAddress[3];
  }
  
  public String toString() 
  {
    return getRRName() + 
    		"\t internet address: " + toByteString();
  }
  
}
