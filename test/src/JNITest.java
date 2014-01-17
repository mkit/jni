import cz.cuni.mff.d3s.deeco.simulation.Simulation;


public class JNITest {

	public static void main(String[] args) throws Exception {
		System.out.println(System.getProperty("java.library.path"));
		
		Simulation sim = new Simulation();
		
		sim.initialize();
		
		System.out.println(sim.getCurrentTime());
		
		System.loadLibrary("libmodel");
		System.loadLibrary("liboppcmdenvd");
		
		sim.run("Cmdenv");
		
		System.out.println("Simulation finished.");
		
	}

}
