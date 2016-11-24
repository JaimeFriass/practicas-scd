
/*
Se escribirá una clase hebra Estanquero y otra Fumador . De esta
última habrá tres instancias, cada una almacenará el número de
ingrediente que necesita (o lo que es equivalente: el número de
fumador), que se proporcionará en el constructor.

La interacción entre los fumadores y el estanquero será resuelta
mediante un monitor Estanco basado en el paquete monitor .
*/

import java.util.ArrayList;
import java.util.Random;
import monitor.* ;

class MonitorFum extends AbstractMonitor {

	public static int ing;
	private boolean consumir;
	private Condition[] fumar;
	private Condition poner;

	public MonitorFum() {
		ing = -1;
		consumir = false;
		fumar = new Condition[3];
		poner = makeCondition();

		for (int i = 0; i < 3; i++) {
			fumar[i] = makeCondition();
		}
	}

	// invocado por cada fumador, indicando su ingrediente o numero
	public void obtenerIngrediente (int miIngrediente) {
		enter();
			// Si el ingrediente no está en la mesa aun no puede fumar
			if (ing != miIngrediente)
				fumar[miIngrediente].await();

			System.out.println("\t\t\t\tFumador" + miIngrediente + " comienza a fumar.");
			consumir = false;
			poner.signal();
		leave();
	}

	// invocado por el estanquero, indicando el ingrediente que pone
	public void ponerIngrediente (int ingrediente) {
		enter();
			//if (ing != -1)
			//	poner.await();
			ing = ingrediente;
			consumir = true;
			System.out.println("Estanquero pone el ingrediente " + ing + ".");
			fumar[ingrediente].signal();
		leave();
	}

	// invocado por el estanquero
	public void esperarRecogidaIngrediente() {
		enter();
			if (consumir)
				poner.await();
				//System.out.println("\t\t\t\tFumador" + ing + " termina de fumar.");
		leave();
	}
}

// Cada instancia de la hebra de fumador guarda su número de fumador
// (el número de ingrediente que necesita).
class Fumador implements Runnable {
	private MonitorFum fumadores;
	private int miIngrediente;
	private int nveces;
	public Thread thr;
	
	public Fumador (MonitorFum p_fumadores, int num_veces, int ingrediente, String nombre) {
		fumadores = p_fumadores;
		nveces = num_veces;
		miIngrediente = ingrediente;
		thr = new Thread(this, "Fumador" + nombre);
	}

	public void run() {
		/*
		while (true) {
			estanco.obtenerIngrediente (miIngrediente);
			aux.dormir_max(200);
		}
		*/
	
		for (int i = 0; i < nveces; i++) {
			fumadores.obtenerIngrediente(miIngrediente);
			aux.dormir_max(200);
			System.out.println("\t\t\t\t" + thr.getName() + " ha terminado de fumar.");
		}
	}
}

// El estanquero continuamente produce ingredientes y espera a que se recojan.
class Estanquero implements Runnable {
	public Thread thr;
	private MonitorFum estanco;
	int nveces, ingrediente, nfumadores = 3;
	static Random genAlea = new Random();

	public Estanquero (MonitorFum p_estanco, int num_veces) {
		estanco = p_estanco;
		nveces = num_veces;
		thr = new Thread(this, "Estanquero");
	}

	public void run() {
		int ingrediente;
			for (int i = 0; i < nveces; i++) {
				ingrediente = (int) (Math.random() * 3.0); // 0,1 o 2
				estanco.ponerIngrediente(ingrediente);
				estanco.esperarRecogidaIngrediente();
			}
	}
}

class aux
{
  static Random genAlea = new Random() ;

  static void dormir_max( int milisecsMax )
  { 
    try
    { 
      Thread.sleep( genAlea.nextInt( milisecsMax ) ) ;
    } 
    catch( InterruptedException e )
    { 
      System.err.println("sleep interumpido en 'aux.dormir_max()'");
    }
  }
}

// ****************************************************************************


class Main {
	public static void main(String[] args) {

		// Si no se escriben los argumentos correctos se finaliza la ejecución.
		if (args.length != 1) {
			System.err.println("Uso: numero_iteraciones");
			return;
		}

		// Vectores de hebras:
		Fumador[] fumador = new Fumador[3];
		Estanquero estanquero;

		// Monitor:
		MonitorFum monitor = new MonitorFum();

		// Numero de iteraciones:
		int iteraciones = Integer.parseInt(args[0]); // pasamos a entero el argumento

		// Creación de hebras:
		for (int j = 0; j < 3; j++)
			fumador[j] = new Fumador(monitor, iteraciones, j, Integer.toString(j));
		estanquero = new Estanquero(monitor, iteraciones);

		// Lanzamiento de hebras:
		for (int j = 0; j < 3; j++) 
			fumador[j].thr.start();
		estanquero.thr.start();

	}
}
	
