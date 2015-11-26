import java.io.*; 
import java.net.*;

public class SimpleServer{
	
	/* CREATE NEW SERVER WITH GIVEN ADDRESS AND PORT */
	private String server_address;
	private int port_no;
	private ServerSocket welcomeSocket;
	private Socket connectionSocket;
	
	SimpleServer(int p_n){
		port_no = p_n;
	}
	
	/* BIND SERVER WITH PORT */
	public void bindServer(){
		try{
			welcomeSocket = new ServerSocket(port_no);
			System.out.println("Binded successfully with Port : "+port_no);
		}catch(Exception e){
			System.err.println("Can't bind server with port! \n"+e);
		}
	}
	
	public void startServer(){
		String clientSentence;          
		String capitalizedSentence;
		while(true)
			 {
				try{
					connectionSocket = welcomeSocket.accept();
					
					/* READY FOR CLIENT ! */
					
					
					System.out.println("I guess client is connected!");
					BufferedReader inFromClient =
				   new BufferedReader(new InputStreamReader(connectionSocket.getInputStream()));
					
					DataOutputStream outToClient = new DataOutputStream(connectionSocket.getOutputStream());
					if( (clientSentence = inFromClient.readLine()) != null){
						System.out.println("He said: " + clientSentence);
					}else{					
						System.out.println("I guess client is closed!");
						break;
					}
					
					System.out.println("Lets reply him with: " + clientSentence.toUpperCase());
					capitalizedSentence = clientSentence.toUpperCase() + '\n';
					outToClient.writeBytes(capitalizedSentence);
					
					break;					
					
				}catch(Exception e){
					System.err.println("Something went wrong! \n"+e);
				}
			 }
		System.out.println("Say bye to him!");
		closeServer();
	}
	
	public void closeServer(){
		try{
			connectionSocket.close();
			welcomeSocket.close();
		}catch(Exception e){
			System.err.println("Something went wrong! \n"+e);
		}
	}
	
	
}