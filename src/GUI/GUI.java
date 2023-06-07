/*
 * Clay Molitor
 * CS 145 Assignment 3
 * 
 * For this assignment I chose to make a simple graphical user interface and use events.
 * My program's background color changes to the color of the pressed button.
 * 
 * 
 * 
 */


import java.awt.Color;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.LinkedList;
import javax.swing.*;


public class GUI {
	public static void main(String[] args) {
		
		JFrame frame = new JFrame("CM GUI Demo");
		frame.setSize(320, 240);

		buildGui(frame);

		frame.setVisible(true);

		
	}

	// Adds Button with "Green", "Orange", and "Yellow"
	// When clicked these buttons will change the windows background to their color 

	public static void buildGui(JFrame frame)
	{
		JPanel buttonsPanel = new JPanel();

		// List of buttons with colors
		LinkedList<ColorButton> buttons = new LinkedList<>();
		buttons.add(new ColorButton("Green",  Color.GREEN,  buttonsPanel));
		buttons.add(new ColorButton("Orange", Color.ORANGE, buttonsPanel));
		buttons.add(new ColorButton("Yellow", Color.YELLOW, buttonsPanel));

		for (JButton b : buttons) {
			buttonsPanel.add(b);
		}

		buttonsPanel.setBackground(Color.gray);

		frame.add(buttonsPanel);
		frame.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
	}

	// Button with a color and a target
	// On click recolor target to color
	private static class ColorButton extends JButton implements ActionListener 
	{
		Color color;
		Container target;

		ColorButton(String name, Color color, Container target)
		{
			super(name);
			this.color = color;
			this.target = target;
			this.setBackground(color); // Sets button to it's color
			this.addActionListener(this); // onclick run actionPerformed
		}
		public void actionPerformed(ActionEvent a)
		{
			// Set the frame's background to the color passed on construction
			target.setBackground(color);
		}
	}
}
