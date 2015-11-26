import java.util.*;

public class JavaNetworking{
	
	public static void main(String args[]){
		System.out.println("What you want to be?");	
		System.out.println("1. Server");	
		System.out.println("2. Client");	
		
		Scanner in = new Scanner(System.in);
		int ch = in.nextInt();
		
		if(ch == 1){
			/* SERVER APPLICATION */
			System.out.println("SERVER APPLICATION");
			System.out.println("Specify port");
			int p_n = in.nextInt();
			
			SimpleServer s = new SimpleServer(p_n);
			s.bindServer();
			s.startServer();
			
		
		}else{
			/* CLIENT APPLICATION */
			System.out.println("CLIENT APPLICATION");
			
			System.out.println("Specify server port");
			int p_n = in.nextInt();
			
			SimpleClient c = new SimpleClient(p_n);
			c.initClient();
			c.sendServer("Hello Server!");			
			c.closeClinet();
		}
		
		System.out.println("!");
		
	}

}