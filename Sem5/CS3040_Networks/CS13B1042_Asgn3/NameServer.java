import java.io.*;

//Class for NameServer
public class NameServer extends DNSRR 
{
  private String nameServer;

  protected void decode(DNSInputStream dnsIn) throws IOException 
  {
    nameServer = dnsIn.readDomainName();			//nameServer is obtained
  }

  //returns nameServer
  public String getNameServer() 
  {
    return nameServer;
  }

  //prints in the required format
  public String toString()
  {
    return getRRName() + 
    		"\t nameserver = " + nameServer;
  }
  
}
