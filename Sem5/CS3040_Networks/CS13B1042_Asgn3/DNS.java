public final class DNS 
{

  private static final String[] typeNames = 
	  {
    "Address", "NameServer", "MailDomain", "MailForwarder",
    "CanonicalName", "StartOfAuthority", "MailBox", "MailGroup",
    "MailRename", "Null", "WellKnownServices", "Pointer",
    "HostInfo", "MailInfo", "MailExchanger", "Text"
  };
  
  public static String typeName (int type) 
  {
    return ((type >= 1) && (type <= 16)) ? typeNames[type - 1] : "Unknown";
  }
  
  private static final String[] codeNames = 
	 {
    "Format error", "Server failure", "Name not known",
    "Not implemented", "Refused"
  };
  
  public static String codeName (int code) 
  {
    return ((code >= 1) && (code <= 5)) ? codeNames[code - 1] : "Unknown error";
  }
  
}
