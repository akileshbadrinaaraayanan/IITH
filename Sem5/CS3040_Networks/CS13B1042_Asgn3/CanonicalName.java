import java.io.*;

//Class for Canonical name
public class CanonicalName extends DNSRR 
{
  private String canonicalName;

  protected void decode(DNSInputStream dnsIn) throws IOException 
  {
    canonicalName = dnsIn.readDomainName();				//read from input
  }

  public String getCanonicalName() 
  {
    return canonicalName;								//returns cName
  }

  public String toString() 
  {
    return getRRName() +
    		"\t canonical name = " + canonicalName;
  }
}