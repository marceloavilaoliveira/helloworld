package br.com.cpqd.helloworld;

/**
 * Hello World!
 */
public class App {
    /**
     * Let me add a comment here
     */
    public static void main( String[] args ) {
        // Create some variables
        int not_used_1 = 741;
        int not_used_2;
        String hellotxt = "Hello"; // Set hello text

        // Print the Hello message
        System.out.println();
        System.out.print(hellotxt);

        // Checks if there is any argument
        if (args.length > 0)  {
            // Print the arguments
            for (int i=0; i<args.length; i++)
                System.out.print(" " + args[i]);
        } else {
            // Print the word "World"
            System.out.print("World");
        }

        System.out.println();
        System.out.println();

        return;
    }

    public static boolean isOk()
    {
        /**
         * This function don't do anything useful
         */
        return true;
    }
}
