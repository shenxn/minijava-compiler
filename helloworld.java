class MainClass {
	public static void main (String [] id) {
        {
            System.out.println("Hello world");
            System.out.println(5 + 3);
            if (true) {
                System.out.print("Should print with num 5 / 2 = ");
                System.out.println(5 / 2);
            } else {
                System.out.println("Should not print");
            }
            System.out.println(new HelloClass().hello(-9));
            System.out.print("5 / 3 = ");
            System.out.println(new HelloClass().v5() / new HelloClass().v3());
            System.out.print("5 + 6 = ");
            System.out.println(new HelloClass().v5() + 6);
            System.out.print("true && true = ");
            if (new HelloClass().vtrue() && new HelloClass().vtrue()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("true || false = ");
            if (new HelloClass().vtrue() || new HelloClass().vfalse()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("1 > 5 = ");
            if (1 > new HelloClass().v5()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("5 >= 3 = ");
            if (new HelloClass().v5() > new HelloClass().v3()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("5 == 3 = ");
            if (new HelloClass().v5() == new HelloClass().v3()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("2 != 2 = ");
            if (2 != 2) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("-5 = ");
            System.out.println(-new HelloClass().v5());
            System.out.print("+3 = ");
            System.out.println(+new HelloClass().v3());
            System.out.print("!true = ");
            if (!new HelloClass().vtrue()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("-8 = ");
            System.out.println(-8);
        }
	}

}

class HelloClass {
    public int hello(int x) {
        System.out.println("Hello from hello()");
        System.out.print("param x = ");
        System.out.println(x);
        System.out.print("Return value is ");
        return -1;
    }
    public int v5() {
        return 5;
    }
    public int v3() {
        return 3;
    }
    public boolean vtrue() {
        return true;
    }
    public boolean vfalse() {
        return false;
    }
}
