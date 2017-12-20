import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

class Fichero{
	private String proceso;
	private int prioridad;
	private int tamanyo;
	private int tiempo;
	// Getters
	public String getProceso() {
		return proceso;
	}
	public int getPrioridad() {
		return prioridad;
	}
	public int getTamanyo() {
		return tamanyo;
	}
	public int getTiempo() {
		return tiempo;
	}
	// Fin Getters
	// Setters
	public void setProceso(String proceso) {
		this.proceso = proceso;
	}
	public void setTiempo(int tiempo) {
		this.tiempo = tiempo;
	}
	public void setPrioridad(int prioridad) {
		this.prioridad = prioridad;
	}
	public void setTamanyo(int tamanyo) {
		this.tamanyo = tamanyo;
	}
	// Fin Setters
	public Fichero(String proceso, int prioridad, int tamanyo,
			int tiempo) {
		super();
		this.proceso = proceso;
		this.prioridad = prioridad;
		this.tamanyo = tamanyo;
		this.tiempo = tiempo;
	}
	public Fichero(String linea){
		String [] campos = null;
		campos = linea.split(" ");
		proceso = campos[0];
		prioridad = Integer.parseInt(campos[1]);
		tamanyo = Integer.parseInt(campos[2]);
		tiempo = Integer.parseInt(campos[3]);
	}
}

public class gestormemoria{
	public static ArrayList<Fichero> procesos_a_evaluar = new ArrayList<Fichero>();  // lista de procesos esperando a memoria
	public static int opcion;
	public static Procesos memoria = new Procesos();
	
	public static void consola(String cmd) {
		if(cmd == "limpiar") {
			System.out.print("\033[H\033[2J");
			System.out.flush();
		}else {
			try {
				Runtime.getRuntime().exec(cmd);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}
	
	public static void leerFichero(String nombreFichero) throws FileNotFoundException{
		FileReader fichero = null;
		BufferedReader buffer = null;
		String linea = null;
		Fichero datos;
		
		fichero = new FileReader(nombreFichero);
		buffer = new BufferedReader(fichero);
		try{
			linea = buffer.readLine();
			while(linea != null && !linea.equals("")){
				datos = new Fichero(linea);
				if(datos.getTamanyo() <= 2000)
					procesos_a_evaluar.add(datos); 
				linea = buffer.readLine();
			}
			try{
				fichero.close();
				buffer.close();
			}catch(IOException e){ }
		}
		catch(IOException e){ }
		
	}
	
	public static void menu(){
		StringBuilder sb = new StringBuilder();
		sb.append("******************************************************************\n");
		sb.append("*                 MENU DEL GESTOR DE MEMORIA                     *\n");
		sb.append("*   Selecciona a continuación el algoritmo que deseas utilizar   *\n");
		sb.append("******************************************************************\n");
		sb.append("*   1. Primer Hueco                                              *\n");	
		sb.append("*   2. Mejor Hueco                                               *\n");
		sb.append("******************************************************************\n");
		sb.append("Introduce la opcion deseada: ");
		System.out.print(sb);
	}
	
	public static void ErrorArgumentos() {
		StringBuilder sb = new StringBuilder();
		sb.append("Error: fallo en el numero de argumetos\n");
		sb.append("Por favor, ejecute el programa como se muestra a continuacion\n");
		sb.append("gestormemoria.jar ficheroprocesos.txt\n");
		System.out.print(sb);
	}
	
	public static void gestionDeProcesos(){
		int instante;
		boolean evaluar;
		File archivo = new File("particiones.txt");
		PrintWriter escribir = null;
		try{
			escribir = new PrintWriter(archivo);
			instante = 1;
			while(!procesos_a_evaluar.isEmpty() || !memoria.isEmpty()){
				for(int i = 0; i < procesos_a_evaluar.size(); i++){
					if(procesos_a_evaluar.get(i).getPrioridad() <= instante){
						if(opcion == 1){
							evaluar = memoria.primerHueco(procesos_a_evaluar.get(i));
						}
						else{
							evaluar = memoria.mejorHueco(procesos_a_evaluar.get(i));
						}
						if(evaluar){
							procesos_a_evaluar.remove(i);
							i--;
						}
					}
				}
				StringBuilder sb = new StringBuilder();
				sb.append(instante);
				sb.append(" ");
				sb.append(memoria.toString());
				escribir.print(sb);
				escribir.flush();
				instante++;
				memoria.reducir();
			}
			System.out.print("Ya se ha generado el fichero correctamente :)\n");
			consola("subl particiones.txt");
		}catch(FileNotFoundException e) {}
		escribir.close();
	}

	public static void main(String [] args){
		@SuppressWarnings("resource")
		Scanner sc = new Scanner(System.in);
		if(args.length == 1){
			do{
				consola("limpiar");
				menu();
				opcion = sc.nextInt();
				if(opcion < 1 || opcion > 2) {
					System.out.println("Error: seleccione una de la opcion 1 o 3");               
				}
			}while(opcion < 1 || opcion > 2);
			try{
				leerFichero(args[0]);
				gestionDeProcesos();
			}
			catch(FileNotFoundException e){
				System.out.println("Error al abrir el fichero");
			}
		}else {
			ErrorArgumentos();
		}
	}
}
