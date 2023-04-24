package Critters;
// CS 145
// Lab 2: Critters

// Clay Molitor

// Summary of critter movment
// 1 Attack when able
// 2 Go right as far as possible
// 3 If all the way to the right, go down
// 4 otherwise go up
// 5 attack anything that gets close
// 6 If nothing is happening, stampede to the far side

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import sun.misc.Unsafe;
import java.lang.reflect.Field;
import java.util.Map;

import javax.swing.JButton;

// The Sheep class returns values that are called from CritterPanel in order to run the Critter Game.
// Aditionally, Hacks can be used to edit private CritterPanel values in order to cheat.
// Hacks.add() adds critters of the passed type and Hacks.killOthers() replaces all living critters with the passed critter.
public class Sheep extends Critter {


	static boolean enableCheats = true;
	static final String DISPLAY_STRING = "🐑";


	// How far the sheep will shift south at the end of the stampede.
	static final int STAMPEDE_SHIFT = 7;
	static boolean stampede = false;
	static int endingStampede = 0;

	// How long all sheep have been in position, resets when a sheep is out of poition
	static int positionTime = 0;

	static int count = 0;
	static boolean firstInit = true;
	static boolean firstMove = true;
	// i incrimented every time any sheep's move method is called.
	static int i = 0;
	
	
	// Once the jet is against a wall and next to another jet it will be "In Position"
	boolean inPosition = false;
	boolean turnRightNext = false;

	public Sheep() {
		count++;

		// Could use a Random here instead
		if (firstInit) {
			firstInit = false;

			Hacks.init();

			try {
				Hacks.addCheatButton();
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

			Hacks.init();

			try {
				Hacks.addCheatButton();

				if(enableCheats)
				{
					Hacks.addAnimals(300, this.getClass());
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
				Hacks.killOthers(Sheep.class);
				
			} catch (Exception e) {
				System.out.println(e);
			}
		}

		if (inPosition)
		{
			positionTime++;
		}
		else
		{
			positionTime = 0;
		}
		// Stampede after 20 steps without any sheep action
		if(positionTime > (count* 20))
		{
			stampede = true;
		}

		return move(info);
		
	}
	// The main move method, calls other move methods.
	protected Action move(CritterInfo info)
	{
		// ====== Moving and attacking logic ======
		inPosition = false; // Sets draw color to default
		Direction dir = info.getDirection() ;

		
		if (info.getFront() == Neighbor.OTHER) { // Enemy in front
			return Action.INFECT;
		} // Stamede makes all sheep move to the far left
		else if(stampede)
		{
			// Once at the far left end the stampede
			if(dir == Direction.WEST && info.getFront() == Neighbor.WALL)
			{
				stampede = false;
				endingStampede = 1;
			}

			if(dir == Direction.WEST) {
				return attackMove(info);
			} else {
				return Action.LEFT;
			}
		}  
		else if(endingStampede > 0) // When the stamped is ending shift down for a few rounds
		{
			// Moves South for STAMPEDE_SHIFT rounds after stampede ends

			endingStampede++;
			if((endingStampede/count) >= STAMPEDE_SHIFT) {
				endingStampede = 0;
			}

			if(dir != Direction.SOUTH) {
				return Action.LEFT;
			}
			else {
				return attackMove(info);
			}

		}
		// If against the east wall move down
		else if (dir == Direction.SOUTH && info.getLeft() == Neighbor.WALL)
		{
			// If another sheep is bellow turn outwards
			if(info.getFront() == Neighbor.SAME || info.getFront() == Neighbor.WALL)
			{
				return Action.RIGHT;
			}
			else {
				return attackMove(info);
			}
		} // If facing the east wall turn down
		else if(dir == Direction.EAST && info.getFront() == Neighbor.WALL ) {
			return Action.RIGHT;
		} // If against Sheep move up
		else if (dir == Direction.NORTH && info.getRight() == Neighbor.SAME)
		{
			// If another sheep is above, turn outwards
			if(info.getFront() == Neighbor.SAME || info.getFront() == Neighbor.WALL)
			{
				return Action.LEFT;
			}
			else {
				return attackMove(info);
			}
		} // If facing sheep turn up
		else if( info.getFront() == Neighbor.SAME && dir == Direction.EAST) {
			return Action.LEFT;
		} // If back is to the wall or sheep, attack
		else if(dir == Direction.WEST &&  (
			info.getBack() == Neighbor.WALL && // Back to wall and can't move
				(info.getLeft() == Neighbor.SAME || info.getLeft() == Neighbor.WALL) ||
			info.getBack() == Neighbor.SAME && // Back to sheep and can't move
				(info.getRight() == Neighbor.SAME || info.getRight() == Neighbor.WALL)))
		{
			inPosition = true;
			return attack(info);
		}
		
		else {
			return moveEast(info);
		}

	}
	// Should be called instead of Action.HOP;
	// Will turn to face the enemy instead of moving when advantagous
	protected Action attackMove(CritterInfo info) {
		Direction dir = info.getDirection();


		// If there is an enemy that is not looking at me then attack
		if( // If going north and there is an enemy that is not facing the Sheep then attack
			(dir == Direction.NORTH  && (
			(info.getLeft()  == Neighbor.OTHER && info.getLeftDirection()  != Direction.EAST ) ||
			(info.getRight() == Neighbor.OTHER && info.getRightDirection() != Direction.WEST ))) 
				|| // If headed south..
			(dir == Direction.SOUTH  && ( 
			(info.getLeft()  == Neighbor.OTHER && info.getLeftDirection()  != Direction.WEST ) ||
			(info.getRight() == Neighbor.OTHER && info.getRightDirection() != Direction.EAST ))) 
				|| // If headed east...
			(dir == Direction.EAST  && ( 
			(info.getLeft()  == Neighbor.OTHER && info.getLeftDirection()  != Direction.SOUTH) ||
			(info.getRight() == Neighbor.OTHER && info.getRightDirection() != Direction.NORTH))) 
				|| // If headed west...
			(dir == Direction.WEST  && ( 
			(info.getLeft()  == Neighbor.OTHER && info.getLeftDirection()  != Direction.NORTH) ||
			(info.getRight() == Neighbor.OTHER && info.getRightDirection() != Direction.SOUTH)))  )
		{
			return attack(info);
		}

		return Action.HOP;
	}
	// Should be called instead of Action.INFECT;
	// Will turn to face enemy when no enemy is infront.
	protected Action attack(CritterInfo info)
	{
		if (info.getFront() == Neighbor.OTHER) {
			return Action.INFECT;
		} else if(info.getLeft() == Neighbor.OTHER) {
			return Action.RIGHT;
		} else if(info.getRight() == Neighbor.OTHER) {
			return Action.RIGHT;
		}
		return Action.INFECT;

	}
	protected Action moveEast(CritterInfo info)
	{
		Direction dir = info.getDirection() ;
		
		if(dir == Direction.EAST) {
			return Action.HOP;
		} else if (dir == Direction.NORTH){
			return Action.RIGHT;
		} else if (dir == Direction.SOUTH || dir == Direction.WEST ) {
			return Action.LEFT;
		} 

		return attackMove(info);
	}
	protected Action eastWall(CritterInfo info)
	{
		if(info.getDirection() == Direction.EAST){
			return Action.RIGHT;
		}
		else{
			
			return Action.HOP;
		}

	}
	@Override
	protected void finalize() {

		count--;
	}

	@Override
	// Red if stampeding, grey if moving, black if in poition
	public Color getColor() {
		if(stampede)
			return Color.RED;
		else
			return (inPosition)? Color.BLACK : Color.WHITE;
	}

	@Override
	public String toString() {
		return DISPLAY_STRING;
	}
}

// Access protected and private methods from CritterModel and CritterFrame
class Hacks {

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
		JButton b = new JButton("Diasable Cheats");
        
		b.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                Sheep.enableCheats = false;
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
					// For each of the passed species
				}
				// All other critters
				else {
					System.out.println("Killed " + c.toString() + " at: \t" + x + ": " + y);
					Object privateData = info.get(c);
					
					// TODO: Replace Sheep with type of survivorSpecies.
					grid[x][y] = new Sheep();
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
