
/*
Se escribirá una clase hebra Estanquero y otra Fumador . De esta
última habrá tres instancias, cada una almacenará el número de
ingrediente que necesita (o lo que es equivalente: el número de
fumador), que se proporcionará en el constructor.

La interacción entre los fumadores y el estanquero será resuelta
mediante un monitor Estanco basado en el paquete monitor .
*/

import java.util.Random;
import monitor.* ;

class MonitorFum extends AbstractMonitor {

	public static int ing = -1;
	private Condition[] fumar;
	private Condition poner;

	public MonitorFum() {
		fumar = new Condition[3];
		poner = makeCondition();

		for (int i = 0; i < 3; i++) {
			fumar[i] = makeCondition();
		}
	}

	// invocado por cada fumador, indicando su ingrediente o numero
	public void obtenerIngrediente (int miIngrediente) {
		enter();
			if (ing != miIngrediente) {
				System.out.println("Fumador " + miIngrediente + " no puede fumar aún.");
				fumar.get(miIngrediente).await();
			}

			System.out.println("Fumador " + miIngrediente + " comienza a fumar.");
			ing = -1;
			poner.signal();
		leave();
	}

	// invocado por el estanquero, indicando el ingrediente que pone
	public void ponerIngrediente (int ingrediente) {
		enter();
			if (ing != -1)
				poner.wait();
			ing = ingrediente;
			System.out.println("Estanquero pone el ingrediente " + ing + ".");
			fumar.get(ingrediente).signal();
		leave();
	}

	// invocado por el estanquero
	public void esperarRecogidaIngrediente() {
		enter();
			System.out.println("Fumador " + ingrediente + " termina de fumar.")
		leave();
	}
}

// Cada instancia de la hebra de fumador guarda su número de fumador
// (el número de ingrediente que necesita).
class Fumador implements Runnable {
	private MonitorFum fumadores;
	int miIngrediente;
	int nveces;
	public Thread thr;
	
	public Fumador (MonitorFum p_fumadores, int num_veces; int ingrediente, String nombre) {
		fumadores = p_fumadores;
		nveces = num_veces;
		miIngrediente = ingrediente;
		thr = new Thread(this, nombre);
	}

	public void run() {
		/*
		while (true) {
			estanco.obtenerIngrediente (miIngrediente);
			aux.dormir_max(200);
		}
		*/
	
		for (int i = 0; i < nveces; i++) {
			System.out.println(thr.getName() + " solicita fumar.");
			fumadores.obtenerIngrediente(miIngrediente);
			System.out.println(thr.getName() + " fumando.");
			aux.dormir_max(200);
			fumadores.esperarRecogidaIngrediente();
		}
	}
}

// El estanquero continuamente produce ingredientes y espera a que se recojan.
class Estanquero implements Runnable {
	public Thread thr;
	private Estanquero estanco;
	int nveces, ingrediente, nfumadores;ç
	static Random genAlea = new Random();

	public Estanquero (MonitorFum p_estanco, int num_veces, int num_fumadores, String nombre) {
		estanco = p_estanco;
		nveces = num_veces;
		nfumadores = num_fumadores;
		thr = new Thread(this, nombre);
	}

	public void run() {
		int ingrediente;
		while (true) {
			for (int i = 0; i < nveces; i++) {
				ingrediente = (int) (Math.random() * 3.0); // 0,1 o 2
				estanco.ponerIngrediente(ingrediente);
				estanco.esperarRecogidaIngrediente();
			}
		}
	}
}

// ****************************************************************************


class Main {
	public static void main(String[] args) {

		// Si no se escriben los argumentos correctos se finaliza la ejecución.
		if (args.length != 4) {
			System.err.println("Uso: numero_iteraciones");
			return;
		}

		// Vectores de hebras:
		Fumadores[] fumador = new Fumador[3];
		Estanquero estanquero = new Estanquero();

		// Monitor:
		MonitorFum monitor = new MonitorFum();

		// Numero de iteraciones:
		int iteraciones = Integer.parseInt(args[0]); // pasamos a entero el argumento

		// Creación de hebras:
		for (int j = 0; j < 3; j++)
			fumador[i] = new Fumador(monitor, iteraciones, j, "Fumador" + 1);
		estanquero = new Estanquero(monitor, iteraciones, fumador.length, "Estanquero");

		// Lanzamiento de hebras:
		for (int j = 0; j < 3; j++) 
			fumador[j].thr.start();
		estanquero.thr.start();

	}
}
	
