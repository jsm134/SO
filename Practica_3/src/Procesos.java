import java.util.ArrayList;

class datosPorProceso{
	private String nombre;
	private int direccion_memoria;
	private int tam;
	private int queda;
	// Getters
	public String getNombre() {
		return nombre;
	}
	public int getDireccion() {
		return direccion_memoria;
	}
	public int getTam() {
		return tam;
	}
	public int getQueda() {
		return queda;
	}
	// Fin Getters
	// Setters
	public void setNombre(String nombre) {
		this.nombre = nombre;
	}
	public void setDireccion(int direccion_memoria) {
		this.direccion_memoria = direccion_memoria;
	}
	public void setTam(int tam) {
		this.tam = tam;
	}
	public void setQueda(int queda) {
		this.queda = queda;
	}
	// Fin Setters
	public datosPorProceso(String nombre, int direccion_memoria, int tam, int queda) {
		super();
		this.nombre = nombre;
		this.direccion_memoria = direccion_memoria;
		this.tam = tam;
		this.queda = queda;
	}
	public void reducir(){
		queda--;
		if(queda == 0)
			nombre = "hueco";
	}
	public datosPorProceso(Fichero file){
		nombre = file.getProceso();
		tam = file.getTamanyo();
		queda = file.getTiempo();
		direccion_memoria = 0;
	}
	public String toString(){
		StringBuilder sb = new StringBuilder();
		sb.append("[");
		sb.append(direccion_memoria);
		sb.append(" ");
		sb.append(nombre);
		sb.append(" ");
		sb.append(tam);
		sb.append("]");
		return sb.toString();
	}
}

public class Procesos{
	private ArrayList<datosPorProceso> datos;
	private static final int tam = 2000;
	
	public ArrayList<datosPorProceso> getMemoria(){
		return datos;
	}
	
	public boolean isEmpty(){
		return datos.size() == 1 && datos.get(0).getNombre() == "hueco";
	}

	public Procesos(){
		datos = new ArrayList<datosPorProceso>();
		datos.add(new datosPorProceso("hueco", 0, tam, 0));
	}
	
	public void reducir(){
		for(int i = 0; i < datos.size(); i++){
			if(datos.get(i).getNombre() != "hueco"){
				datos.get(i).reducir();
			}
		}
		for(int i = 0; i < datos.size(); i++){
			if(datos.get(i).getNombre() == "hueco"){
				int proceso_antiguo = i + 1;
				while(proceso_antiguo < datos.size() && datos.get(proceso_antiguo).getNombre() == "hueco"){
					datos.get(i).setTam(datos.get(i).getTam() + datos.get(proceso_antiguo).getTam());
					datos.remove(proceso_antiguo);
				}
			}
		}
	}
	
	public boolean mejorHueco(Fichero dato){
		int mejorPosicion;
		int diferenciaActual, diferenciaPeor;
		mejorPosicion = -1;
		for(int i = 0; i < datos.size(); i++){
			if(datos.get(i).getNombre() == "hueco" && datos.get(i).getTam() >= dato.getTamanyo()){
				if(mejorPosicion == -1){
					mejorPosicion = i;
				}
				else{
					diferenciaPeor = datos.get(mejorPosicion).getTam() - dato.getTamanyo();
					diferenciaActual = datos.get(i).getTam() - dato.getTamanyo();
					if(diferenciaActual < diferenciaPeor)
						mejorPosicion = i;
				}
			}
		}
		if(mejorPosicion != -1){
			datosPorProceso procesoNuevo = new datosPorProceso(dato);
			datos.get(mejorPosicion).setTam(datos.get(mejorPosicion).getTam() - dato.getTamanyo());
			procesoNuevo.setDireccion(datos.get(mejorPosicion).getDireccion());
			datos.get(mejorPosicion).setDireccion(datos.get(mejorPosicion).getDireccion() + dato.getTamanyo());
			if(datos.get(mejorPosicion).getTam() == 0) {
				datos.remove(mejorPosicion);
			}
			datos.add(mejorPosicion, procesoNuevo);
			return true;
		}else {
			return false;
		}
	}
	
	public String toString(){
		StringBuilder sb = new StringBuilder();
		int i;
		for(i = 0; i < datos.size(); i++){
			sb.append(datos.get(i).toString());
			if(i != datos.size() - 1){
				sb.append(" ");
			}
		}
		sb.append("\n");
		return sb.toString();
	}

	public boolean primerHueco(Fichero nuevo){
		int i = 0;
		boolean hayhueco;
		hayhueco = false;
		while(i < datos.size() && !hayhueco){
			if(datos.get(i).getNombre() == "hueco" && datos.get(i).getTam() >= nuevo.getTamanyo()){
				hayhueco = true;
			}
			else{
				i++;
			}
		}
		if(hayhueco){
			datosPorProceso nuevoMemoria = new datosPorProceso(nuevo);
			datos.get(i).setTam(datos.get(i).getTam() - nuevo.getTamanyo());
			nuevoMemoria.setDireccion(datos.get(i).getDireccion());
			datos.get(i).setDireccion(datos.get(i).getDireccion() + nuevo.getTamanyo());
			if(datos.get(i).getTam() == 0)
				datos.remove(i);
			datos.add(i, nuevoMemoria);
		}
		return hayhueco;
	}
}