// Clay Molitor
// Giant Critter
// The giant is grey 
// Displays alternating "Fee", "fie", "foe",  and "fum"
// Lion will infect critters in front and will avoid walls and other lions.


import java.awt.*;


public class Giant extends Critter{
    
    
    int i = 0;


    public Giant()
    {

    }
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
    public Color getColor() {
        
        return Color.GRAY;
    }

    // Display as an L
    public String toString() {

        i++;

        String name = "";
        
        switch((i/6) % 4)
        {
            case 0:
            name = "fee";
            break;
            case 1:
            name = "fie";
            break;
            case 2:
            name = "foe";
            break;
            case 3:
            name = "fum";
            break;
            default:
            name = "fum"; // default is never run
        }

        return name;
    }
}
