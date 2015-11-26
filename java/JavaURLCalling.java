import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.nio.charset.Charset;
 
/**
 * @author https://github.com/karshe/
 * PROJECT : https://github.com/karshe/networkprogramming
 */
 
public class JavaURLCalling {
 
	public static void main(String[] args) {
		/* Lets poke localhost at port 6001 */
		System.out.println("\nOutput : \n" + callURL("http://127.0.0.1:6001/"));
	}
 
	public static String callURL(String myURL) {
		System.out.println("Requeted URL : " + myURL);
		
		/* Buffer for Output from requested URL */
		StringBuilder sb = new StringBuilder();
		URLConnection urlConn = null;
		InputStreamReader in = null;
		try {
			URL url = new URL(myURL);
			urlConn = url.openConnection();
			if (urlConn != null)
				urlConn.setReadTimeout(60 * 1000); /* 1 MIN TIMEOUT */
			if (urlConn != null && urlConn.getInputStream() != null) {
				in = new InputStreamReader(urlConn.getInputStream(),
						Charset.defaultCharset());
				BufferedReader bufferedReader = new BufferedReader(in);
				if (bufferedReader != null) {
					int cp;
					while ((cp = bufferedReader.read()) != -1) {
						sb.append((char) cp);
					}
					bufferedReader.close();
				}
			}
		in.close();
		} catch (Exception e) {
			throw new RuntimeException("Exception while calling URL:"+ myURL, e);
		} 
		
		/* Return output! */
		return sb.toString();
	}
}