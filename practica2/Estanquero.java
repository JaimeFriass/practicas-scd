
/*
Se escribirá una clase hebra Estanquero y otra Fumador . De esta
última habrá tres instancias, cada una almacenará el número de
ingrediente que necesita (o lo que es equivalente: el número de
fumador), que se proporcionará en el constructor.

La interacción entre los fumadores y el estanquero será resuelta
mediante un monitor Estanco basado en el paquete monitor .
*/

import monitor.* ;

class Estanco extends AbstractMonitor {
	private int numero_ingrediente;
	public Boolean puede_consumir;

	public Boolean 

	// invocado por cada fumador, indicando su ingrediente o numero
	public void obtenerIngrediente (int miIngrediente) {
		enter();
		if (puede_consumir)
			
		leave();
	}

	// invocado por el estanquero, indicando el ingrediente que pone
	public void ponerIngrediente (int ingrediente) {
		enter();

		leave();
	}

	// invocado por el estanquero
	public void esperarRecogidaIngrediente() {
		enter();

		leave();
	}
}

// Cada instancia de la hebra de fumador guarda su número de fumador
// (el número de ingrediente que necesita).
class Fumador implements Runnable {
	int miIngrediente;
	public Thread thr;
	...
	public Fumador (int p_miIngrediente, ... ) {
		miIngrediente = p_miIngrediente;
		thr = new Thread(this, "fumador" + miIngrediente);
	}

	public void run() {
		while (true) {
			estanco.obtenerIngrediente (miIngrediente);
			aux.dormir_max(200);
		}
	}
}

// El estanquero continuamente produce ingredientes y espera a que se recojan.
class Estanquero implements Runnable {
	public Thread thr;
	...
	public void run() {
		int ingrediente;
		while (true) {
			ingrediente = (int) (Math.random() * 3.0); // 0,1 o 2
			estanco.ponerIngrediente (ingrediente);
			estanco.esperarRecogidaIngrediente();
		}
	}
}


class Lector implements Runnable 
{
  private  MonitorLE monitorLE ; // objeto monitor l.e. compartido
  private int        nveces ;    // numero de veces que lee
  public  Thread     thr   ;     // objeto hebra encapsulado
  
  public Lector( MonitorLE p_monitorLE, int p_nveces, String nombre ) 
  { 
    monitorLE  = p_monitorLE  ;    
    nveces     = p_nveces ;
    thr        = new Thread(this,nombre);
  }
  public void run() 
  { 
    for( int i = 0 ; i < nveces ; i++ ) 
    { 
      System.out.println( thr.getName()+": solicita lectura.");
      monitorLE.inicio_lectura();
        System.out.println( thr.getName()+": leyendo.");
        aux.dormir_max( 1000 ) ;
      monitorLE.fin_lectura();
    }
  }
}

// ****************************************************************************

class Escritor implements Runnable 
{
  private MonitorLE monitorLE ; // objeto monitor l.e. compartido
  private int                   nveces    ; // numero de veces que lee
  public  Thread                thr       ; // objeto hebra encapsulado
   
  public Escritor( MonitorLE p_monitorLE, int p_nveces, String nombre ) 
  { 
    monitorLE  = p_monitorLE  ;    
    nveces     = p_nveces ;
    thr        = new Thread(this,nombre);
  }
  public void run() 
  { 
    for( int i = 0 ; i < nveces ; i++ ) 
    { 
      System.out.println( thr.getName()+": solicita escritura.");
      monitorLE.inicio_escritura();
        System.out.println( thr.getName()+": escribiendo.");
        aux.dormir_max( 1000 );
      monitorLE.fin_escritura ();
    }
  }
}

// ****************************************************************************


class Main
{
	public static void main(String[] args) {
		

class Main
{ 
  public static void main(String[] args) 
  { 
    if ( args.length != 4 ) 
    { 
      System.err.println("Uso: num_lectores num_escritores num_iters_lector num_iters_escritor");
      return ;
    }
    
    // crear los dos vectores de hebras:
    Lector[]    vlec          = new Lector[ Integer.parseInt(args[0]) ];
    Escritor[]  vesc          = new Escritor[ Integer.parseInt(args[1]) ];

    // leer numero de iteraciones de lector y de escritor
    int   iter_lector   = Integer.parseInt(args[2]);
    int   iter_escritor = Integer.parseInt(args[3]);

    // crear monitor
    MonitorLE monitor = new MonitorLE();
    
    // crear hebras
    for( int i = 0; i < vlec.length; i++) 
      vlec[i] = new Lector(monitor, iter_lector,"lector"+(i+1));
    for( int i = 0; i < vesc.length; i++) 
      vesc[i] = new Escritor(monitor, iter_lector,"escritor"+(i+1));
    
    // lanzar hebras
    for( int i = 0; i < vlec.length ; i++) 
      vlec[i].thr.start();
    for( int i = 0; i < vesc.length ; i++) 
      vesc[i].thr.start();
  }
}
// ****************************************************************************

