// CS 145
// Lab 2: Critters

// Clay Molitor

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import sun.misc.Unsafe;
import java.lang.reflect.Field;
import java.util.Map;

import javax.swing.JButton;

// The JetFighter class returns values that are called from CritterPanel in order to run the Critter Game.
// Aditionally, Hack can be used to edit private CritterPanel values in order to cheat.
// Hack.add() adds critters of the passed type and Hack.genocide() replaces all living critters with the passed critter.
public class JetFighter extends Critter {

	static int count = 0;
	static boolean firstInit = true;
	static boolean firstMove = true;
	// i incrimented every time any jet's move method is called.
	static int i = 0;
	static boolean enableCheats = false;
	
	// Once the jet is against a wall and next to another jet it will be "In Position"
	boolean inPosition = false;
	boolean turnRightNext = false;
	public JetFighter() {
		count++;

		// Could use a Random here instead
		if (firstInit) {
			firstInit = false;

			Hack.init();

			try {
				Hack.addCheatButton();
			} catch (SecurityException e) {
				e.printStackTrace();
			}
		}
	}

	// Runs about once every step.
	// Must be run to send Jet's to GC so that finalize is called.
	private static void periodicGC() {
		i++;
		if (i % (count) == 0) {
			Runtime.getRuntime().gc();

		}
	}

	// Call every time a Jet moves, only runs on the first call.
	private void firstRun()
	{
		if (firstMove) {
			firstMove = false;

			Hack.init();

			try {
				Hack.addCheatButton();

				if(enableCheats)
				{
					Hack.addAnimals(100, this.getClass());
				}
			} catch (NoSuchFieldException | SecurityException e) {
				e.printStackTrace();
			}

		}
	}

	// Jets will follow walls until they boarder another jet.
	// Once adjacent to a jet they will turn into the center and infect all that pass.
	// Jets will also turn to attack others on their left or right.
	@Override
	public Action getMove(CritterInfo info) {

		
		// Call every time, only runs once.
		firstRun();
		
		// Calls Garbage Collection
		periodicGC();

		// Calls hacks 
		if(enableCheats)
			{
			try {
				// Replace all critters with specified critter
				Hack.killOthers(JetFighter.class);
				
			} catch (Exception e) {
				System.out.println(e);
			}
		}


		// ====== Moving and attacking logic ======
		inPosition = false; // Sets draw color to default
		if (info.getFront() == Neighbor.OTHER) { // Enemy in front
			return Action.INFECT;
		}  else if(info.getFront() == Neighbor.EMPTY) { // Empty in front
			if(turnRightNext && info.getRight() == Neighbor.EMPTY){ // Turns to the right to find gaps in "In Position" Jets.
				turnRightNext = false;
				return Action.RIGHT;
			}
			// Turn to face other
			if(info.getLeft() == Neighbor.OTHER) {
				return Action.LEFT;
			}
			// Turn to face other
			else if(info.getRight() == Neighbor.OTHER) {
				return Action.RIGHT;
			}
			// Walls on left or right, try and circle the walls until finding another jet
			else if(info.getLeft() == Neighbor.WALL || info.getRight() == Neighbor.WALL)
			{
				if(info.getFront() == Neighbor.SAME) { // Turn away from wall if next to another plane
					if (info.getLeft() == Neighbor.WALL){ 
						return Action.RIGHT; 
					}
					else {
						return Action.LEFT;
					}
				}
				else {
					return Action.HOP;
				}
			}
			// if Wall behind, attack front
			else if(info.getBack() == Neighbor.WALL)
			{
				inPosition = true; // Sets draw color to second color.
				return Action.INFECT;
			}
			else{ // Default action, hop around
				// If passing a Jet on the right turn right next turn
				if(info.getRight() == Neighbor.SAME)
				{
					turnRightNext = true;
				}
				return Action.HOP;
			}
		} else if (info.getFront() == Neighbor.SAME) { // Ally in front
			return Action.LEFT;
		} else if (info.getFront() == Neighbor.WALL) { // Wall in front
			return Action.LEFT;
		}else // This should never be called
		{
			return Action.INFECT;
		} // End of moving and attacking
	}

	@Override
	protected void finalize() {

		count--;
	}

	@Override
	public Color getColor() {
		return (inPosition)? Color.WHITE : Color.BLACK;
	}

	@Override
	public String toString() {
		return "✈️";
	}
}


class Hack {

	// Used to write to protected fields
	public static Unsafe unsafe;

	public static CritterFrame frame; // CritterFrame object;
	public static CritterModel model; // CritterFrame.myModel;
	public static Map<Critter, Object> info; // CritterFrame.myModel.info
	public static Critter[][] grid; // CritterFrame.myModel.grid


	// Must be run before calling the other methods
	// This method fetches protected fileds from the CritterFrame and CritterModel objects
	public static void init() {
		try {

			// frame
			frame = (CritterFrame) Frame.getFrames()[0];


			// model
			Field modelField = CritterFrame.class.getDeclaredField("myModel");
			modelField.setAccessible(true);
			model = (CritterModel) modelField.get(frame);


			// info
			Field infoField = CritterModel.class.getDeclaredField("info");
			infoField.setAccessible(true);
			info = (Map<Critter, Object>) infoField.get(model);

			// grid
			Field gridField = CritterModel.class.getDeclaredField("grid");
			gridField.setAccessible(true);
			grid = (Critter[][]) gridField.get(model);

			unsafe = getUnsafe();
		} catch (Exception e) {
			System.out.println("An error occured initializing FighterJet's hacks");
			System.out.println(e);
		}
	}

	private static Unsafe getUnsafe() {
		try {
			Field theUnsafe = Unsafe.class.getDeclaredField("theUnsafe");
			theUnsafe.setAccessible(true);
			return (Unsafe) theUnsafe.get(null);
		} catch (Exception e) {
			throw new AssertionError(e);
		}
	}

	public static void addCheatButton()
	{
		Component root;
		Container southFrame;
		JButton b = new JButton("Disable Cheats");
        
		b.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                JetFighter.enableCheats = false;
            }
        });

		
		root = frame.getRootPane().getComponent(1);
		
		southFrame = (Container)((Container)((Container)root).getComponent(0)).getComponent(1);

		southFrame.add(b);
	}

	// Kills all but survivorSpecies species
	public static void killOthers(Class survivorSpecies)
			throws IllegalArgumentException {

		for (int x = 0; x < grid.length; x++)
		{
			for (int y = 0; y < grid[x].length; y++)
			{
				Critter c = grid[x][y];
				if(c == null)
				{
					//System.out.println(x + "- " + y);
				}
				//If current grid contains the passed critter type
				else if(c.getClass() == survivorSpecies)
				{
				}
				// If critter is Food
				//else if(c.getClass() == Food.class) { }
				// All other critters
				else {
					System.out.println("Killed " + c.toString() + " at: \t" + x + ": " + y);
					Object privateData = info.get(c);
					
					// TODO: Replace JetFighter with type of survivorSpecies.
					grid[x][y] = new JetFighter();
					info.remove(c);
					info.put(grid[x][y], privateData);
				}
			}
		}
	}

	// Adds animals by calling CritterFrame.add
	// Sets CritterFrame.started to false to bypass anti-cheating protections.
	public static void addAnimals(int count, Class type) throws NoSuchFieldException, SecurityException {

		Field startedField = CritterFrame.class.getDeclaredField("started");
		startedField.setAccessible(true);

		// Set "started" to false, allowing frame.add to work.
		unsafe.putInt(frame, unsafe.objectFieldOffset(startedField), 0);
		// add JetFighters
		frame.add(count, type);

	}

}
