class MainClass {
	public static void main (String [] id) {
        {
            System.out.println("Hello world");
            System.out.print("3 + 4 = ");
            System.out.println(3 + 4);
            System.out.print("3 - 4 * 2 = ");
            System.out.println(3 - 4 * 2);
            System.out.print("(3 - 4) * 2 = ");
            System.out.println((3 - 4) * 2);
            if (true) {
                System.out.print("true - ");
                if (false) {
                    System.out.println("true");
                } else {
                    System.out.println("false");
                }
                System.out.println("End true if");
            } else {
                System.out.println("false");
            }
        }
	}

}
