
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.InputStreamReader;

public class Utils {

    public static double[][] matriz(int rows, int columns, String path){
        double[][] dat = new double[rows][columns];
        FileReader file = null;
        BufferedReader bf = null;

        try {
            file = new FileReader(path);
            //FileReader file = new FileReader("C:\\Users\\Usuario-Pc\\Desktop\\jacob.txt");
            bf = new BufferedReader(file);                        
            int j = 0;

            while(bf.ready()){
                String[] filas = bf.readLine().split("\t");
                String aux = "";

                for(int i = 0; i < columns; i++){
                    for(int h = 0; h < filas[i].length(); h++){
                        if(filas[i].charAt(h) == ','){
                            aux += "." + filas[i].substring(h+1, filas[i].length());
                            break;
                        }
                        aux += filas[i].charAt(h);
                    }
                    dat[j][i] = Double.parseDouble(aux);
                    aux = "";
                }
                j++;
            }            

            for(int i = 0; i < rows; i++){
                System.out.print("{");
                for(int k = 0; k < columns; k++){
                    System.out.print(dat[i][k]);
                    if(k < columns-1)
                        System.out.print(",");
                }
                System.out.println("},\r");
            }

        } catch (Exception e) {
            System.out.println("Error " + e);
        }
        finally{
            try {
                bf.close();
                file.close();
            } catch (Exception e) {
            }
            return dat;
        }
    }
}
