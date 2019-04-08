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
            if (!true || 3 == 3) {
                System.out.print("true - ");
                if ((false || (false != false)) && true) {
                    System.out.println("true");
                } else {
                    System.out.println("false");
                }
                System.out.println("End true if");
            } else {
                System.out.println("false");
            }
            System.out.print("1 < 3: ");
            if (1 < 3) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("5 < -7: ");
            if (5 < -7) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("-5 > -7: ");
            if (-5 > -7) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("-7 < -7: ");
            if (-7 < -7) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("-7 < -5: ");
            if (-7 < -5) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("3 > 1: ");
            if (3 > 1) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("3 >= 1: ");
            if (3 >= 1) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("3 >= 3: ");
            if (3 >= 3) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("3 <= 1: ");
            if (3 <= 1) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("3 >= 1: ");
            if (1 <= 1) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
        }
	}

}
