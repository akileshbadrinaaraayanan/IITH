import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;


public class HttpResponse 
{
	String response;		//response string
	
	HttpRequest req;		//HttpRequest
	
	String path = "/home/akilesh/Desktop/Networksassn2";		//path of the folder containing the HTML page
	
	public HttpResponse(HttpRequest request)
	{
		 req = request;
		
		File f = new File(path + req.requested_filename);		//create a new file
		
		try {
			response = "HTTP/1.1 200 OK\r\n"; 	//version of HTTP and status code
			
			response += "My server \r\n";		// name of the server
			response += "Content type: text/html \r\n";		//type of content
			
		
			response += "Content length: "+ f.length() + " \r\n";	//Content length
			response += "\r\n";
			
			FileInputStream fi = new FileInputStream(f);	
			int s;
			
			while((s = fi.read()) != -1)	//keep reading as long as EOF is encountered
			{
				response += (char) s;		//append it to the response
			}
			fi.close();						//close the file
			
		} catch (FileNotFoundException e) {
			//error 404 file not found
			response = response.replace("200 OK", "404 Not Found");	//status code 200 OK, replaced by 404 not found
		}
		catch(Exception e){
			//if other error the 500 internal server
			response = response.replace("200 OK", "500 Internal Server Error");	//status code 200 OK, replaced by 500 internal server error
		}
		
		System.out.println(response);		//print the response
		
	}
	
}
