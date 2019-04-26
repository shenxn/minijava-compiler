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
            System.out.println(new HelloClass().hello());
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
        }
	}

}

class HelloClass {
    public int hello() {
        System.out.println("Hello from hello()");
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
