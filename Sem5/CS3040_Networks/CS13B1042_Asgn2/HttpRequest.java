//A class to handle HttpRequest
public class HttpRequest {


	String requested_filename;
	
	public HttpRequest(String request)
	{
		String lines[] = request.split("\n");	//get lines of request separately
		requested_filename = lines[0].split(" ")[1];	//GET filename is the second word in line 0	
	}
	
}
