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
            System.out.println(new HelloClass().hello(1, 2));
        }
	}

}

class HelloClass {
    int v;

    public int hello(int p1, int p2) {
        int x;
        System.out.println("Hello from HelloClass::hello()");
        x = new HelloClass().hello2(1, 2, 3, 4, 5, false);
        System.out.println(x);
        System.out.print("Return value is ");
        return -1;
    }

    public int hello2(int p1, int p2, int p3, int p4, int p5, boolean p6) {
        int x;
        int y;
        HelloClass hc;
        System.out.println("Hello from HelloClass::hello2()");
        hc = new NewHelloClass();
        System.out.println(hc.hello(1, 2));
        x = 100;
        y = 8;
        System.out.print("x * (x + y) - y = (10792) ");
        System.out.println(x * (x + y) - y);
        System.out.print("p1 = ");
        System.out.println(p1);
        System.out.print("p2 = ");
        System.out.println(p2);
        System.out.print("p3 = ");
        System.out.println(p3);
        System.out.print("p4 = ");
        System.out.println(p4);
        System.out.print("p5 = ");
        System.out.println(p5);
        System.out.print("p6 = ");
        if (p6) {
            System.out.println("true");
        } else {
            System.out.println("false");
        }

        x = 2;
        while (x > 5) {
            System.out.println("Should not print");
        }
        while (x > -3) {
            System.out.print("while loop x = ");
            System.out.println(x);
            x = x - 1;
        }


        System.out.print("Return value is ");
        return y;
    }

    public int printV() {
        v = 8;
        System.out.print("HelloClass::v = ");
        System.out.println(v);
        return this.hi();
    }

    public int hi() {
        System.out.println("Hi from HelloClass");
        return 0;
    }
}

class NewHelloClass extends HelloClass {
    int v;

    public int hello(int p1, int p2) {
        v = 5;
        System.out.println("Hello from NewHelloClass::hello()");
        if (this.printV() == 1) {} else {}
        System.out.print("NewHelloClass::v = ");
        System.out.println(v);
        System.out.print("Return value is ");
        return -1;
    }

    public int hi() {
        System.out.println("Hi from NewHelloClass");
        return 0;
    }
}

