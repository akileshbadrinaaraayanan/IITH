
import java.util.Random;

/** 
 Generate pseudo-random floating point values, with an 
 approximately Gaussian (normal) distribution.

 Many physical measurements have an approximately Gaussian 
 distribution; this provides a way of simulating such values. 
*/
public final class RandomGaussian {
  
  public static void main(String... aArgs){
    RandomGaussian gaussian = new RandomGaussian();
    double MEAN = 20.0f; 
    double VARIANCE = 5.0f;
    for (int idx = 1; idx <= 10; ++idx){
      log("Generated : " + gaussian.getGaussian(MEAN, VARIANCE));
    }
  }
    
  private Random fRandom = new Random();
  
  double getGaussian(double aMean, double aVariance){
    return aMean + fRandom.nextGaussian() * aVariance;
  }

  private static void log(Object aMsg){
    System.out.println(String.valueOf(aMsg));
  }
} 


