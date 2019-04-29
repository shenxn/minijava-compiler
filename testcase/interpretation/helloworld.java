class helloworld {
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
                System.out.println("params is false");
            }
            if (new C().test()) {
                System.out.println("test is true");
            } else {
                System.out.println("test is false");
            }
            if (new C().array()) {
                System.out.println("array is true");
            } else {
                System.out.println("array is false");
            }
            System.out.print("whileTest is ");
            System.out.println(new C().whileTest());
            System.out.print("vtable is ");
            System.out.println(new C().vtable());
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
        int v1;
        int v2;
        int v3;
        int x1;
        v1 = 16;
        v2 = 17;
        v3 = 18;
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
        System.out.print("v1 = ");
        System.out.println(v1);
        System.out.print("v2 = ");
        System.out.println(v2);
        x1 = 5;
        System.out.print("v3 = ");
        System.out.println(v3);
        System.out.print("x1 = ");
        System.out.println(x1);
        System.out.print("v2 + v3 = ");
        System.out.println(v2 + v3);
        return true;
    }
    public boolean test() {
        C1 b;
        b = new C1();
        System.out.println(b.init(11));
        System.out.println(b.initv2(10));
        System.out.print("v1 = ");
        System.out.println(b.rv1());
        System.out.print("v1 + v2 = ");
        System.out.println(b.rv12());
        return true;
    }
    public boolean array() {
        int[][] a;
        a = new int[new C().v5()][10];
        System.out.print("a.length = ");
        System.out.println(a.length);
        System.out.print("a[3].length = ");
        System.out.println(a[3].length);
        a[3][new C().v5()] = -10;
        System.out.print("a[3][5] = ");
        System.out.println(a[3][5]);
        return true;
    }
    public int whileTest() {
        int p;
        p = 5;
        while (p > 0) {
            System.out.print(p);
            p = p - 1;
        }
        return p;
    }
    public int vtable() {
        Base b;
        b = new C1();
        System.out.println(b.hello(99));
        return 10;
    }
}

class Base {
    int v1;

    public int init(int p1) {
        v1 = p1;
        return v1;
    }

    public int rv1() {
        return v1;
    }

    public int hello(int a) {
        System.out.print("Hello from Base ");
        System.out.println(a);
        return 5;
    }
}

class C1 extends Base {
    int v2;

    public int initv2(int p2) {
        v2 = p2;
        return p2;
    }

    public int rv12() {
        return v1 + v2;
    }

    public int hello(int a) {
        System.out.print("Hello from C1 ");
        System.out.println(a);
        return 8;
    }
}
