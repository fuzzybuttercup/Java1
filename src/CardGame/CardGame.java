package CardGame;
// Clay Molitor
// 4-24-2023
// CS 145
// https://github.com/fuzzybuttercup/Java1/tree/master/src/CardGame
/*  
 *  My card game is slap jack, where players take turns placing cards faceup in a pile. When a jack is drawn all players try and slap it.
 *  The winner takes the stack, players lose a card if they slap incorrectly. If you run out of cards you lose.
 *  
 *  There is very little strategy in playing slap jack, especially on the computer, However it is a good game for implementing Lists.
 *  I used a stack for the center deck, however this stack is never used as a stack because cards are never drawn from the center.
 *      The center stack is shuffled into the successful slappers hand.
 *  Each player has a Queue for their cards. Won cards are added to the bottom and played cards are taken from the top
 *  Also each player has a pointer to the next player, forming a circular, non indexed, linked list. Losing players are removed from the link.
 */

import java.util.*;
import java.util.stream.Collectors;

// Plays slap jack
public class CardGame {

    // Setting, disable if emojis are not available on your platform.
    static boolean enableEmojis = true;

    public static void main(String[] args) throws Exception {

        // Debug
        // String[] names= {"Fred", "Gob", "Hilly"};
        // String[] keys = {"F", "G", "H"};
        // CardGame game = new CardGame(names, keys, scanner);

        Scanner scanner = new Scanner(System.in);

        int players = 0;
        while (players <= 1) {
            System.out.print("How many players are playing? ");
            players = scanner.nextInt();

            scanner.nextLine(); // Clear scanner buffer

        }

        CardGame game = new CardGame(players, scanner);

        game.playSlapJack();

    }

    Player startingPlayer;
    // The cards in the center of the table.
    Stack<Card> cardPile;

    Scanner inputScanner;

    // ------------- Setup game -------------

    // Prompt the players to enter names and choose a "slap" key.
    CardGame(int players, Scanner input) {
        String[] names = new String[players];
        String[] keys = new String[players];

        for (int i = 0; i < players; i++) {
            System.out.print("Player " + (i + 1) + ", Enter your name and press enter: ");
            names[i] = input.nextLine().trim();
            System.out.print("Now press a single key followed by enter. \nThis will be the key you use to \"slap\": ");
            keys[i] = input.nextLine().trim().toUpperCase();
            System.out.println(names[i] + ", Your Slap key is \"" + keys[i] + "\". Press this key to SLAP!\n\n");
        }
        init(names, keys, input);
    }

    // Pass an array of names and slap keys, these arrays must be the same length.
    CardGame(String[] names, String[] keys, Scanner input) throws Exception {
        if (names.length != keys.length) {
            throw new Exception("Error");
        }

        init(names, keys, input);
    }

    // Called by constructor
    // Sets up the Players list
    private void init(String[] names, String[] keys, Scanner input) {
        this.inputScanner = input;
        cardPile = new Stack<Card>();

        // First player.
        Player first = new Player(names[0], keys[0]);
        Player prev = first;

        // Subsequent Players.
        for (int i = 1; i < names.length; i++) {
            Player p = new Player(names[i], keys[i]);

            prev.add(p);
            prev = p;
        }
        // Linked the last player to the first, closing the loop
        prev.add(first);

        startingPlayer = first;
        dealCards(first);
        System.out.print(rules());
    }

    // Shuffles and deals standard 52 card deck to players
    private void dealCards(Player player) {
        // Creates deck
        ArrayList<Card> cards = new ArrayList<Card>();
        // 1 to 13
        for (int face = 1; face <= 13; face++) {
            for (Card.Suit suit : Card.Suit.values()) {
                cards.add(new Card(face, suit));
            }
        }
        // Shuffle deck
        Collections.shuffle(cards);

        // Deals cards
        for (Card card : cards) {

            player.giveCard(card);
            player = player.next();
        }
    }

    // ------------- Play slap jack -------------
    public void playSlapJack() {
        Player p = startingPlayer;
        String in;

        while (p.next() != p) {

            // Drawing a card from the player and adding it to the pile
            Card card = p.takeCard();
            cardPile.add(card);

            System.out.print(card.display());

            System.out.println(
                    p.getName() + ", you have " + p.deckSize() + " cards left, Press ENTER to Draw a card.");

            // Slapping
            System.out.println("Slap? If not, press enter to continue.");
            in = inputScanner.nextLine();

            System.out.print("\n\n\n\n\n\n\n\n\n\n\n\n\n");

            // Removes duplicate characters, eg: cleanup if a player hit their key twice.
            String cleanedString = Arrays.asList(in.toUpperCase().split(""))
                    .stream().distinct().collect(Collectors.joining());

            // Check if any players guessed correctly, or incorrectly.
            // Will loop through all players

            Player i = p;
            for (char c : cleanedString.toCharArray()) {

                do // Loop all players once
                {
                    // If the players key char was pressed
                    if (i.getKey() == c) {
                        if (card.face == 11) // Jack
                        {
                            i.giveCard(cardPile);
                            System.out.println(i.name + " Wins the trick, gaining "
                                    + cardPile.size() + " cards.");
                            cardPile.clear();
                        } else // wrong card
                        {
                            // move a card from the player to the pile
                            cardPile.add(i.takeCard());
                            System.out.println(i.getName() + ", lost a card");
                        }
                    }
                    i = i.next();
                } while (i != p);

            }

            // eliminate losers
            if (p.next().deckSize() == 0) {
                System.out.println(p.next().getName() + " has been removed from the game.");
                // p.remove removes the NEXT player
                p.remove();
            }
            // Ending round
            p = p.next();
            playHand(p);
        }

        System.out.println(p.getName() + " has Won!");

    }

    public String playHand(Player p) {
        return "";
    }

    public String rules() {
        return "Welcome to Clay's Slap Jack!\n" +
                "The goal of slap jack is to slap the jacks.\n" +
                "Each player will choose a key on the keyboard as their slap key.\n" +
                "When a Jack is drawn, be the first to press your slap key and then ENTER to win. \n" +
                "The winner gets all the card in the center stack and adds them to their deck.\n" +
                "Once you are out of cards you are eliminated and can no longer play.\n" +
                "The last player left is the winner.\n" +
                "On your turn press enter to flip up a card when prompter. If it's a jack, Slap!\n" +
                "Make sure all players are ready before drawing a card as anyone can slap.\n" +
                "Incorrect slaps will cost you a card.\n\n";
    }

    // Acts as a linked list.
    // Inherits Iterator because it has the least abstract methods to overwrite.
    // Each player has a stack of cards, a name, and a key that they use to "Slap".
    private class Player implements Iterator<Player> {

        private Player nextPlayer; // Pointer to next Player

        private Queue<Card> cards;
        public String name;
        public char key;

        Player(String name, String key) {
            this.name = name;
            this.key = key.toUpperCase().trim().charAt(0);

            cards = new LinkedList<Card>();
        }
        // -------------- Linked List Logic--------------

        // Adds the next player. Called during setup.
        public void add(Player p) {
            nextPlayer = p;
        }

        // Return the player whom (who?) goes next.
        public Player next() {
            return nextPlayer;
        }

        // Removes the NEXT player.
        public void remove() {
            nextPlayer = next().next();
        }

        public boolean hasNext() {
            return nextPlayer != null;
        }

        // -------------- Player Logic --------------

        public String getName() {
            return name;
        }

        public char getKey() {
            return this.key;
        }

        // -------------- Card Logic --------------

        public void giveCard(Card card) {
            cards.add(card);
        }

        // Shuffles and adds cards to player's deck
        public void giveCard(List<Card> card) {
            Collections.shuffle(card);

            cards.addAll(card);
        }

        public Card takeCard() {
            return cards.poll();
        }

        public int deckSize() {
            return cards.size();
        }

    }

    // A stack of cards for each player.
    // Cards may be added to the bottom and removed from the top
    class Hand {

    }

    class Card {

        public enum Suit {
            HEARTS, DIAMONDS, CLUBS, SPADES;
        }

        Suit suit;
        int face; // 1:Ace, 2:2 ... 10:10, 11:Jack, 12:Queen, 13:King,

        Card(int face, Suit suit) {
            this.face = face;
            this.suit = suit;
        }

        // Returns an emoji for the cards suit.
        public String suitString() {

            switch (suit) {
                case CLUBS:
                    return enableEmojis ? "♣" : "C";
                case DIAMONDS:
                    return enableEmojis ? "♦" : "D";
                case HEARTS:
                    return enableEmojis ? "♥" : "H";
                case SPADES:
                    return enableEmojis ? "♠" : "S";
                default: // Should never be called, add breakpoint here
                    return enableEmojis ? "X" : "X";
            }
        }

        // Returns the face as a string, may be 1 or 2 length.
        public String faceString() {
            String f;
            switch (face) {
                case 1:
                    f = "A";
                    break;
                case 11:
                    f = "J";
                    break;
                case 12:
                    f = "Q";
                    break;
                case 13:
                    f = "K";
                    break;
                default:
                    f = Integer.toString(face);
            }
            // If length 1 front pad with a space
            return f;
        }

        // Returns a stylized card for printing to console
        public String display() {
            String s = suitString();
            String f = faceString();
            String f1 = (f.length() == 1) ? f + " " : f; // back pad if length 1
            String f2 = (f.length() == 1) ? " " + f : f; // front pad if length 1

            return "┌─────┐\n" +
                    "│" + f1 + "   │\n" +
                    "│" + s + "    │\n" +
                    "│    " + s + "│\n" +
                    "│   " + f2 + "│\n" +
                    "└─────┘\n";
        }

        public String toString() {
            return suit + ": " + faceString();
        }
    }
}
