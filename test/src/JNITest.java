import cz.cuni.mff.d3s.deeco.simulation.Simulation;


public class JNITest {

	public static void main(String[] args) throws Exception {
		System.out.println(System.getProperty("java.library.path"));
		Simulation sim = new Simulation();
		
		sim.initialize();
		
		sim.run("Cmdenv");
		
		System.gc();
		System.out.println("Simulation finished.");
		
	}

}
