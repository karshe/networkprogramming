import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class JavaHttpGetConnection {

	private final String USER_AGENT = "Mozilla/5.0";

	public static void main(String[] args) throws Exception {

		JavaHttpGetConnection http = new JavaHttpGetConnection();

		System.out.println("Send HTTP GET request.");
		http.sendGet();

	}

	// HTTP GET request
	private void sendGet() throws Exception {

		String url = "http://127.0.0.1:8080/host/php/christmas_calculator.php";
		
		URL obj = new URL(url);
		HttpURLConnection con = (HttpURLConnection) obj.openConnection();

		// optional default is GET
		con.setRequestMethod("GET");

		//add request header
		con.setRequestProperty("User-Agent", USER_AGENT);

		int responseCode = con.getResponseCode();
		System.out.println("\nENDING 'GET' REQUEST TO CHRISTMAS CALCULATOR : " + url);
		System.out.println("RESPONSE CODE : " + responseCode);

		BufferedReader in = new BufferedReader(
		        new InputStreamReader(con.getInputStream()));
		String inputLine;
		StringBuffer response = new StringBuffer();

		while ((inputLine = in.readLine()) != null) {
			response.append(inputLine);
		}
		in.close();

		//print result
		System.out.println(response.toString());

	}

}