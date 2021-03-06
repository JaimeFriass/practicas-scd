import monitor.*;
import java.util.Random;

class Barberia extends AbstractMonitor {
	
	public static int cola;
	public static boolean cortando;
	private Condition barbero = makeCondition();
	private Condition clientes = makeCondition();

	public Barberia() {
		cola = 0;
		cortando = false;
	}

	// invocado por los clientes para cortarse el pelo
	public void cortarPelo(int n_cliente) {
		enter();
			cola++;
			if (cortando) {
				System.out.println("Cliente " + n_cliente + " espera.");
				clientes.await();
			}
			cortando = true;
			System.out.println("Cliente " + n_cliente + " empieza a cortarse el pelo.");
			barbero.signal();
		leave();
	}
	// invocado por el barbero para esperar (si procede) a un nuevo
	// cliente y sentarlo para el corte
	public void siguienteCliente() {
		enter();
			// Si no hay nadie esperando, barbero espera
			if (cola == 0)
				barbero.await();
			cola--;
			cortando = true;
		leave();
	}
	// invocado por el barbero para indicar que ha terminado de cortar
	// el pelo
	public void finCliente() {
		enter();
			System.out.println("Cliente ha terminado de cortarse el pelo.");
			cortando = false;
			clientes.signal();
		leave();
	}
}

class aux{
	static Random genAlea = new Random() ;

	static void dormir_max(int milisecsMax){
    	try{
			Thread.sleep( genAlea.nextInt( milisecsMax ) ) ;
    	}

		catch(InterruptedException e){
			System.err.println("sleep interumpido en 'aux.dormir_max()'");
		}
	}
}

class Cliente implements Runnable {
	public Thread thr;
	private int n_cliente;
	private Barberia barberia;

	public Cliente (Barberia p_barberia, int p_cliente) {
		barberia = p_barberia;
		n_cliente = p_cliente;
		thr = new Thread (this, "Cliente" + p_cliente);
	}

	public void run () {
		while (true) {
			barberia.cortarPelo(n_cliente); // el cliente espera (si procede) y se corta el pelo
			aux.dormir_max(2000); // el cliente está fuera de la barberia un tiempo
		}
	}
}

class Barbero implements Runnable {
	public Thread thr;
	private Barberia barberia;

	public Barbero (Barberia p_barberia) {
		barberia = p_barberia;
		thr = new Thread(this, "Barbero");
	}

	public void run () {
		while (true) {
			barberia.siguienteCliente();
			aux.dormir_max(2500); // el barbero está cortando el pelo
			barberia.finCliente();
		}
	}
}

class Main {
	public static void main (String[] args) {
		if (args.length != 1) {
			System.err.println("Uso correcto: numero_clientes");
			return;
		}

		int numero_clientes = Integer.parseInt(args[0]);
		Cliente clientes[] = new Cliente[numero_clientes];
		Barberia barberia = new Barberia();
		Barbero barbero = new Barbero(barberia);

		for (int i = 0; i < numero_clientes; i++)
			clientes[i] = new Cliente(barberia, i);

		barbero.thr.start();
		for (int i = 0; i < numero_clientes; i++)
			clientes[i].thr.start();
	}
}
