import java.io.*; 
import java.net.*;

public class SimpleClient{
	String remote_host = "localhost";
	int port_no;
	Socket clientSocket;
	
	SimpleClient(int p_n, String r_host){
		port_no = p_n;
		remote_host = r_host;
	}
	
	SimpleClient(int p_n){
		port_no = p_n;
	}
	
	public void initClient(){
		try{
			clientSocket = new Socket(remote_host, port_no);
			System.out.println("Binded successfully with Server on Port : "+port_no);
		}catch(Exception e){
			System.err.println("Can't bind with port! \n"+e);
		}
	
	}
	
	public void sendServer(String msg){
		try{
			DataOutputStream outToServer = new DataOutputStream(clientSocket.getOutputStream());
			outToServer.writeBytes(msg + '\n');
			
			BufferedReader inFromServer = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
			String modifiedSentence = inFromServer.readLine();   
			System.out.println("FROM SERVER: " + modifiedSentence);
		}catch(Exception e){
			System.err.println("Something went wrong! \n"+e);
		}
		
	}
	
	public void closeClinet(){
		try{
			clientSocket.close();
		}catch(Exception e){
			System.err.println("Something went wrong! \n"+e);
		}
	}

}