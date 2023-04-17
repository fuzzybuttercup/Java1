// Clay Molitor
// Lion Critter
// The lion randomly flashes between red, green, and blue. 
// Displays as an "L"
// Lion will infect critters in front and will avoid walls and other lions.


import java.awt.*;
import java.util.Random;


public class Lion extends Critter{
    static private Random rand = new Random();


    @Override
    public Action getMove(CritterInfo info) {
        
        //if 
        if(info.getFront() == Neighbor.OTHER)
        {
            return Action.INFECT;
        }
        // Wall in front or to right
        else if(info.getFront() == Neighbor.WALL || info.getRight() == Neighbor.WALL)
        {
            return Action.LEFT;
        }
        else if(info.getFront() == Neighbor.SAME)
        {
            return Action.RIGHT;
        }
        // else
        return Action.HOP;
    }

    // Randomly select from Red, Green, Blue;
    @Override
    public Color getColor() {
        int r = rand.nextInt(3);
        Color color = null;

        switch(r)
        {
            case 0:
            color = Color.RED;
            break;
            case 1:
            color = Color.GREEN;
            break;
            case 2:
            color = Color.BLUE;
            break;
            default:
            color = Color.BLUE;
            break;
        }
        return color;
    }

    // Display as an L
    @Override
    public String toString() {
        return "L";
    }
}
