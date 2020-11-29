
import java.util.concurrent.RecursiveAction;
import java.util.concurrent.ForkJoinPool;
import java.math.BigInteger;
import java.math.BigInteger; 
import java.security.MessageDigest; 
import java.security.NoSuchAlgorithmException; 
import  java.util.Arrays;
import java.nio.* ;
public class MD5Cracker extends RecursiveAction  {
	private static final long SIZE = 630_188_182;
	private static final long MAX_LENGTH =  7;
	private static final long CHARSETLEN =  26;
	private static final long MIN = 100_000_000;
	private static final String charset = "abcdefghijklmnopqrstuvwxyz";
	private static String prefixes[];
	private  static long length;
	private long start, end, prefix_to_calc, off_end, off_start;
	private static BigInteger hash = new BigInteger("c6178e7799a03b5db95ce4be48ad8c64",16);
	private static String password = "";
	public  BigInteger getMd5(String input) 
    { 
        try { 
  
            // Static getInstance method is called with hashing MD5 
            MessageDigest md = MessageDigest.getInstance("MD5"); 
  
            // digest() method is called to calculate message digest 
            //  of an input digest() return array of byte 
            byte[] messageDigest = md.digest(input.getBytes()); 
            // Convert byte array into signum representation 
            BigInteger no = new BigInteger(1, messageDigest); 
  
        
            return no; 
        }  
  
        // For specifying wrong message digest algorithms 
        catch (NoSuchAlgorithmException e) { 
            throw new RuntimeException(e); 
        } 
	}

	public MD5Cracker(String prefixes[], long length, long off_start, long off_end, long prefix_to_calc,   String password,long start, long end) {
		this.prefixes = prefixes;
		this.length = length;
		this.off_start = off_start;
		this.off_end = off_end;
		this.prefix_to_calc = prefix_to_calc;
		this.password = password;
		this.start = start;
		this.end = end;
	}
	
	public void computeDirectly() {


			for(long i = start; i<end; i++){
				
			prefixes[ (int)(off_end + i) ]  =  prefixes[ (int)(off_start + i/26) ] +  charset.charAt((int)(i % 26));
			if(password.length() == 0){
				if (getMd5(	prefixes[(int)(off_end + i) ]).equals( hash))
				{
					password = prefixes[(int)(off_end + i) ];
					break;
				}
			}
			else{
				break;
			}
		}
	}

	@Override 
	protected void compute() {
		if(password.length() == 0){
		if ( (end - start) <= MIN ) {
			computeDirectly();
		} else {
			long mid = start + ((end - start) / 2);
			invokeAll(new MD5Cracker(prefixes, length,  off_start,  off_end,  prefix_to_calc,    password, start,  mid),
					  new MD5Cracker(prefixes, length,  off_start,  off_end,  prefix_to_calc,   password, mid,  end));
		}}
	}
	
	public static void main(String args[]) {
		int array[], block;
		MD5Cracker threads[];
		double ms;
		long result = 0;
		String prefixes[];
		long startTime, stopTime;
		ForkJoinPool pool;

		prefixes = new String[(int)SIZE];
		
		for (int i = 0; i < 26; i++) prefixes[i] = ""+charset.charAt(i);
	
		
		System.out.printf("Starting with %d threads...\n", Utils.MAXTHREADS);
		ms = 0;
		
		for (int i = 0; i < Utils.N; i++) {
			 long prefix_n = 26;
    		int length = 5;
    		long offset_start = 0;
    		long offset_end = 26;
			password = "";
			startTime = System.currentTimeMillis();
			for(int j = 2; j<=length; j++){
			prefix_n = prefix_n* CHARSETLEN;
			pool = new ForkJoinPool(Utils.MAXTHREADS);
			pool.invoke( new MD5Cracker(prefixes, j,  offset_start,  offset_end,  prefix_n,   password, 0,  prefix_n));
			if(!password.equals("")){
				System.out.printf("password found: " +password +"\n" );
			}
			offset_start = offset_end;
		    offset_end = offset_start + prefix_n;

			}
			stopTime = System.currentTimeMillis();
			ms += (stopTime - startTime);
		}
		System.out.printf("avg time = %.5f\n", (ms / Utils.N));
	}
}
			