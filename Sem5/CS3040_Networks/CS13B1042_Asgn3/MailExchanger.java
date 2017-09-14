//Class for Mail Exchanger
import java.io.*;

public class MailExchanger extends DNSRR 
{
  private String mx;
  private int priority;

//decode dnsIn. Lower the value of priority, higher the preference is
  protected void decode(DNSInputStream dnsIn) throws IOException 
  {
    priority = dnsIn.readShort();			//assign priority
    mx = dnsIn.readDomainName();			//assign mx
  }
  
  public String getMX()		//get Mx() 
  {
    return mx;			
  }

  public int getPriority() 
  {
    return priority;
  }

  public String toString()				//printing 
  {
    return getRRName() + 
      "\t mail exchanger = "+priority+" "+mx;
  }
}
