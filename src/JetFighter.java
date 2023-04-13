// CIS 145
// Lab 2: Critters

// Clay Molitor

import java.awt.*;
import java.lang.System;

import sun.misc.Unsafe;
import java.lang.reflect.Field;
import java.util.Map;

// The JetFighter class returns values that are called from CritterPanel in order to run the Critter Game.
// Aditionally, Hack can be used to edit private CritterPanel values in order to cheat.
// Hack.add() adds critters of the passed type and Hack.genocide() replaces all species with the passed critter.
public class JetFighter extends Critter {

	static int count = 0;
	static boolean first = true;
	// i used for calculating when to calling garbage collection.
	static int iGC = 0;
	static boolean enableCheats = true;


	public JetFighter() {
		count++;

		if (first) {
			first = false;

			Hack.init();

			try {
				if(enableCheats)
				{
					Hack.addAnimals(100, this.getClass());
				}
			} catch (NoSuchFieldException | SecurityException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}
	}

	// Runs about once every step.
	// Must be run to send Jet's to GC so that finalize is called.
	private static void periodicGC() {
		iGC++;
		if (iGC % (count) == 0) {
			System.gc();

		}
	}




	public Action getMove(CritterInfo info) {

		periodicGC();

		// Calls hacks 
		try {
			// Replace all critters with specified critter
			if(enableCheats)
			{
				Hack.genocide(JetFighter.class);
			}
		} catch (Exception e) {
			System.out.println(e);
		}

		// Moving and attacking loginc
		if (info.getFront() == Neighbor.OTHER) {
			return Action.INFECT;
		} else if (info.getFront() == Neighbor.WALL) {
			return Action.LEFT;
		} else if (info.getFront() == Neighbor.SAME) {
			return Action.LEFT;
		} else {

			return Action.HOP;
		}
	}

	@Override
	protected void finalize() {

		count--;
	}

	public Color getColor() {
		return Color.BLACK;
	}

	public String toString() {
		return "✈️";
		// return Integer.toString(count);
	}
}


class Hack {

	// Used to write to protected fields
	public static Unsafe unsafe;

	static CritterFrame frame; // CritterFrame object;
	static CritterModel model; // CritterFrame.myModel;
	public static Map<Critter, Object> info; // CritterFrame.myModel.info
	public static Critter[][] grid; // CritterFrame.myModel.grid


	// Initilize the 
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


	// Kills all but the passed species
	public static void genocide(Class survivorSpecies)
			throws IllegalArgumentException, IllegalAccessException, NoSuchFieldException, SecurityException {
		

		//Class privateInfoField;
		//Field pointField;
		//Field directionField;

		//privateInfoField = CritterModel.class.getDeclaredClasses()[1];

		

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
				else if(c.getClass() == Food.class)
				{
				}
				// All other critters
				else {
					System.out.println("Killed " + c.toString() + " at: \t" + x + ": " + y);
					Object privateData = info.get(c);
					
					// TODO: create a new survivorSpecies instead of a hardcoded type.
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
