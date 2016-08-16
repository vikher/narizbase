


public class RedNeuronal {
    private int entradas;
    private int ocultas;
    private int salidas;
    
    private double[][] wh;
    private double[] bh;
    private double[] yh;
    private double[] minh;
    private double[] maxh;
    private double[][] wy;
    private double[] by;
    private double[] y;
    private double miny = -1;
    private double maxy = 1;
    private double minX = 0;
    private double maxX = 1;
    
    public RedNeuronal(int entradas, int ocultas, int salidas){
        this.entradas = entradas;
        this.ocultas = ocultas;
        this.salidas = salidas;
        this.wh = new double[this.ocultas][this.entradas];
        this.bh = new double[this.ocultas];
        this.yh = new double[this.ocultas];
        
        this.wy = new double[this.salidas][this.ocultas];
        this.by = new double[this.salidas];      
        this.y = new double[this.salidas];
    }
    public void setPesosAndBias(double[][] wh, double[] bh, double[][] wy, double[] by, double[] minh, double[] maxh){
        this.wh = wh;
        this.bh = bh;
        this.wy = wy;
        this.by = by;
        this.minh = minh;
        this.maxh = maxh;
    }
    public double[] calcularSalida(double[] valoresEntrada){
        valoresEntrada = procesarEntrada(valoresEntrada);
        System.out.print("Suma [");
        for(int i = 0; i < ocultas; i++){
            double suma = bh[i];
            for(int j = 0; j < entradas; j++){
                suma += (valoresEntrada[j] * wh[i][j]);
            }
            System.out.print(suma + " ");
            yh[i] = (2/(1 + Math.exp(-2*suma)) - 1);
            //yh[i] = Math.tanh(suma);
            //yh[i] = ((Math.exp(suma) - Math.exp(-1*suma))/(Math.exp(suma) + Math.exp(-1*suma)));
        }
        System.out.println("]");
        System.out.print("YH [");
        for(double a : yh){
            System.out.print(a + " ");
        }
        System.out.println("]");
        
        System.out.print("Suma2 [");
        for(int i = 0; i < salidas; i++){
            double suma = by[i];
            for(int j = 0; j < ocultas; j++){
                suma +=  (yh[j] * wy[i][j]);
            }
            System.out.print(suma + " ");
            y[i] = (2/(1 + Math.exp(-2*suma)) - 1);
            //y[i] = Math.tanh(suma);  
            //y[i] = ((Math.exp(suma) - Math.exp(-suma))/(Math.exp(suma) + Math.exp(-suma)));
        }
        System.out.println("]");
        System.out.print("Y [");
        for(double a : y){
            System.out.print(a + " ");
        }
        System.out.println("]");
        return procesarSalida(y);
    }
    private double[] procesarEntrada(double[] valores){
        for(int i = 0; i < valores.length; i++){
            valores[i] = valores[i] - minh[i];
            valores[i] = valores[i] * (1/(maxh[i] - minh[i]));
            valores[i] = valores[i] * (maxy - miny);
            valores[i] += miny;
        }
        return valores;
    }
    private double[] procesarSalida(double[] valores){
        for(int i = 0; i < valores.length; i++){
            valores[i] = valores[i] - miny;
            valores[i] = valores[i] * (1/(maxy - miny));
            valores[i] = valores[i] * (maxX - minX);
            valores[i] += minX;
            valores[i] = Math.round(valores[i]);
        }
        return valores;
    }
}
