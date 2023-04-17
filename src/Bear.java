// Clay Molitor
// Bear Critter
// The Bear will be set to be white or black on creation. 
// Displays alternating "/" and  "\".
// Lion will infect critters in front and will turn left to avoid objects while moving.


import java.awt.*;


public class Bear extends Critter{

    int i = 0;

    boolean polar;
    public Bear(boolean polar)
    {
       this.polar = polar;
    }
    public Bear()
    {
       
    }
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

    @Override
    public Color getColor() {
       
        if(polar)
        {
        return Color.WHITE;
        }
        else 
        {
            return Color.BLACK;
        }
    }

    // Display as an L
    @Override
    public String toString() {
        i++;
        return (i%2 == 0) ? "/": "\\";
    }
}
