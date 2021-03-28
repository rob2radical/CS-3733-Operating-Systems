import java.util.*;

class ThreadMerge implements Runnable 
{ 
   private double[] out;
   private double[] first;
   private double[] second;
   private int endIndex;

   public ThreadMerge(double[] FirstHalf, double[] SecondHalf, double[] C, int arrSize) 
   { 
      out = C;
      first = FirstHalf;
      second = SecondHalf;
      endIndex = arrSize;
   } 
   public void run() 
   { 
      int i, j; 
      for(i = 0; i < endIndex / 2; i++) 
      { 
         out[i] = first[i]; 
      }
      for(i = 0, j = endIndex / 2; j < endIndex && i < endIndex / 2; i++, j++) 
      { 
         out[j] = second[i]; 
      } 
   } 
}

class SelectionSort implements Runnable 
{ 
   private double[] arrSort;
   private int ArraySize, start;

   public SelectionSort(double[] Array, int startIndex, int endIndex) 
   { 
      arrSort = Array;
      ArraySize = endIndex;
      start = startIndex;
   }

   public void run() 
   { 
      int i, j;
      double temp;
      for(i = start; i < ArraySize; i++) 
      { 
         temp = arrSort[i];
         j = i - 1;
         while(j >= 0 && arrSort[j] > temp)
          { 
            arrSort[j + 1] = arrSort[j];
            j = j - 1;
         } 
         arrSort[j + 1] = temp;
      }
   }
}
public class part2 
{
   public static void main(String args[]) 
   { 
      if(args.length != 1) 
      { 
         System.err.println("ERROR: Please provide the correct number of arguments (file, size of array)\n");
         System.exit(-1);
      }
      int ArraySize = Integer.parseInt(args[0]); 
      int ArrayHalfSize = ArraySize / 2; 
      int i; 
      double[] A = new double[ArraySize];
      double[] B = new double[ArraySize];
      double[] C = new double[ArraySize];
      double[] A_First_Half = new double[ArraySize / 2];
      double[] A_Second_Half = new double[ArraySize / 2]; 

      Random randomDoubles = new Random();
      double start_t, start_t1, end_t, end_t1;

      for(i = 0; i < ArraySize; i++) 
      { 
         A[i] = randomDoubles.nextDouble() * 1000.0;
      }
      for(i = 0; i < ArraySize; i++) 
      { 
         B[i] = A[i];
      } 
      for(i = 0; i < ArrayHalfSize; i++) 
      { 
         A_First_Half[i] = A[i];
      } 
      for(i = ArrayHalfSize; i < ArraySize; i++) 
      { 
         A_Second_Half[i - ArrayHalfSize] = A[i]; 
      }
      SelectionSort r1 = new SelectionSort(B, 0, ArraySize);
      Thread THB = new Thread(r1); 
      start_t = System.nanoTime();
      THB.start(); 

      try 
      { 
         THB.join(); 
      } catch (InterruptedException e) 
      { 
         e.printStackTrace();
      } 
      end_t = System.nanoTime() - start_t;

      System.out.printf("Sorting is done in %.2f ms when one thread is used\n", end_t / 1000000.0);
      SelectionSort r2 = new SelectionSort(A_First_Half, 0, ArrayHalfSize); 
      SelectionSort r3 = new SelectionSort(A_Second_Half, 0, ArrayHalfSize);
      ThreadMerge r4 = new ThreadMerge(A_First_Half, A_Second_Half, C, ArraySize); 
      Thread MRG = new Thread(r4);
      Thread ArrayHalf1 = new Thread(r2);
      Thread ArrayHalf2 = new Thread(r3);

      start_t1 = System.nanoTime();
      ArrayHalf1.start();
      ArrayHalf2.start();

      try 
      {
         ArrayHalf1.join(); 
         ArrayHalf2.join();
      } catch (InterruptedException e) 
      {
         e.printStackTrace();
      } 
      MRG.start(); 
      
      try 
      { 
         MRG.join(); 
      } catch (InterruptedException e) 
      { 
         e.printStackTrace();
      } 
      end_t1 = System.nanoTime() - start_t1; 
      System.out.printf("Sorting is done in %.2f ms when two threads are used\n", end_t1 / 1000000.0);
   }
}