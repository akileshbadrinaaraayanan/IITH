import java.io.*;
import java.util.*;

public class DNSQuery 
{
  private String queryHost;
  private int queryType, queryClass, queryID;
  private static int globalID;
  
  public DNSQuery (String host, int type, int clas) 
  {
    StringTokenizer labels = new StringTokenizer (host, ".");
  
    while (labels.hasMoreTokens ())
      if (labels.nextToken ().length () > 63)			//Label should not be of length more than 63
        throw new IllegalArgumentException ("Invalid hostname: " + host);
    
    queryHost = host;
    queryType = type;
    queryClass = clas;
    
    synchronized (getClass ()) {
      queryID = (++ globalID) % 65536;
    }
  }

  public String getQueryHost () {
    return queryHost;
  }

  public int getQueryType () {
    return queryType;
  }

  public int getQueryClass () {
    return queryClass;
  }

  public int getQueryID () {
    return queryID;
  }

  public byte[] extractQuery () 
  {
    ByteArrayOutputStream byteArrayOut = new ByteArrayOutputStream ();
    DataOutputStream dataOut = new DataOutputStream (byteArrayOut);
    int shift_query = 15, opcode_query = 0, shift_opcode = 11, shift_recurse_please = 8;
    try 
    {
      dataOut.writeShort(queryID);
      
      dataOut.writeShort((0 << shift_query) |
                          (opcode_query << shift_opcode) |
                          (1 << shift_recurse_please));
      
      dataOut.writeShort(1); // QUERIES
      dataOut.writeShort(0); // ANSWERS
      dataOut.writeShort(0); // AUTHORITIES
      dataOut.writeShort(0); // ADDITIONAL
      
      StringTokenizer labels = new StringTokenizer (queryHost, ".");
    
      while(labels.hasMoreTokens())
      {
        String label = labels.nextToken ();
        dataOut.writeByte (label.length ());
        dataOut.writeBytes (label);
      }
      
      dataOut.writeByte(0);
      dataOut.writeShort(queryType);
      dataOut.writeShort(queryClass);
    } 
    catch (IOException ignored) 
    {
    	
    }
    
    return byteArrayOut.toByteArray();			//return
  }

  private ArrayList<DNSRR> answers = new ArrayList<DNSRR>();
  private ArrayList<DNSRR> authorities = new ArrayList<DNSRR>();
  private ArrayList<DNSRR> additional = new ArrayList<DNSRR>();
  
  //receive response back
  public void receiveResponse(byte[] data, int length) throws IOException 
  {
    DNSInputStream dnsIn = new DNSInputStream (data, 0, length);
    
    int id = dnsIn.readShort ();
  
    if (id != queryID)
      throw new IOException ("ID does not match request");
    
    int flags = dnsIn.readShort();
    decodeFlags(flags);
    
    int numQueries = dnsIn.readShort();
    int numAnswers = dnsIn.readShort();
    int numAuthorities = dnsIn.readShort();
    int numAdditional = dnsIn.readShort();
    
    while (numQueries-- > 0) 
    { // discard questions
      
    	dnsIn.readDomainName();
    	dnsIn.readShort();
    	dnsIn.readShort();
    }
    
    try 
    {
      while(numAnswers-- > 0)
        answers.add(dnsIn.readRR());
      
      while(numAuthorities-- > 0)
        authorities.add(dnsIn.readRR());
      
      while(numAdditional-- > 0)
        additional.add(dnsIn.readRR());
      
    } 
    catch (EOFException ex) 
    {
      if (!truncated)
        throw ex;
    }
  }

  private boolean authoritative, truncated, recursive;

  protected void decodeFlags (int flags) throws IOException 
  {
	  int shift_query = 15, shift_authoritative = 10, shift_truncated = 9, shift_recurse_available = 7, shift_response_code = 0;
    boolean isResponse = ((flags >> shift_query) & 1) != 0;
  
    if (!isResponse)
      throw new IOException ("Response flag not set");
    
    authoritative = ((flags >> shift_authoritative) & 1) != 0;
    truncated = ((flags >> shift_truncated) & 1) != 0;
    
    // check recurse request
    recursive = ((flags >> shift_recurse_available) & 1) != 0;
    int code = (flags >> shift_response_code) & 15;
    
    if (code != 0)
      throw new IOException (DNS.codeName (code) + " (" + code + ")");
  }
  
  public boolean isAuthoritative () 
  {
    return authoritative;
  }

  public boolean isTruncated () 
  {
    return truncated;
  }

  public boolean isRecursive () 
  {
    return recursive;
  }

  public Enumeration<DNSRR> getAnswers() 
  {
    return Collections.enumeration(answers);			//returns the answers
  }

  public Enumeration<DNSRR> getAuthorities() 
  {
    return Collections.enumeration(authorities);
  }

  public Enumeration<DNSRR> getAdditional() 
  {
    return Collections.enumeration(additional);
  }
  
}
