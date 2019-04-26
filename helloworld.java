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
            System.out.println(new C().hello(-9));
            System.out.print("5 / 3 = ");
            System.out.println(new C().v5() / new C().v3());
            System.out.print("6 + 5 = ");
            System.out.println(6 + new C().v5());
            System.out.print("true && true = ");
            if (new C().vtrue() && new C().vtrue()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("true || false = ");
            if (new C().vtrue() || new C().vfalse()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("1 > 5 = ");
            if (1 > new C().v5()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("5 >= 3 = ");
            if (new C().v5() > new C().v3()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("5 == 3 = ");
            if (new C().v5() == new C().v3()) {
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
            System.out.println(-new C().v5());
            System.out.print("+3 = ");
            System.out.println(+new C().v3());
            System.out.print("!true = ");
            if (!new C().vtrue()) {
                System.out.println("true");
            } else {
                System.out.println("false");
            }
            System.out.print("-8 = ");
            System.out.println(-8);
            if (new C().params(1, 2, 3, 4, 5, 6)) {
                System.out.println("params is true");
            } else {
                System.out.println("param is false");
            }
        }
	}

}

class C {
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
    public boolean params(int a, int b, int c, int d, int e, int f) {
        f = 10;
        System.out.print("a = ");
        System.out.println(a);
        System.out.print("b = ");
        System.out.println(b);
        System.out.print("c = ");
        System.out.println(c);
        System.out.print("d = ");
        System.out.println(d);
        System.out.print("e = ");
        System.out.println(e);
        System.out.print("f = ");
        System.out.println(f);
        System.out.print("e + f = ");
        System.out.println(e + f);
        System.out.print("f / c = ");
        System.out.println(f / c);
        return true;
    }
}
