import java.io.*;

//DNSRR is an abstract class, there are other classes which extends DNSRR
public abstract class DNSRR 
{
  private String rrName;
  private int rrType, rrClass;
  private long rrTTL, rrCreated;

  //init function assigns values to rrName, rrType, rrClass, rrTTL and rrCreated and calls decode
  void init (String name, int type, int clas, long ttl, DNSInputStream dnsIn) throws IOException 
  {
    rrName = name;
    rrType = type;
    rrClass = clas;
    rrTTL = ttl;
    rrCreated = System.currentTimeMillis();		//created = current time
    decode(dnsIn);
    
  }
  
  protected abstract void decode(DNSInputStream dnsIn) throws IOException;

  public String getRRName() 
  {
    return rrName;	//returns rrName
  }

  public int getRRType() 
  {
    return rrType;			//returns rrType
  }

  public int getRRClass()
  {
    return rrClass;			//returns rrCLass
  }

  public long getRRTTL() 
  {
    return rrTTL;		//returns rrTTL
  }

  //It's valid only if 1000 * rrTTL is greater than diff between current time and time at which it was created
  public boolean isValid() 
  {
    return 1000 * rrTTL > System.currentTimeMillis()-rrCreated;			//checks if its valid or not
  }
}
