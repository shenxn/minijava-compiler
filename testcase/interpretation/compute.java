class compute {
    public static void main (String [] s) {{
        System.out.println(1 + 2 - 4 + 2); // 1
        System.out.println(1 + 3 * 5 / 2 + 4 / 3); // 9
        System.out.println(1 + 3 * 5 / (2 + 4) / 3); // 1
        System.out.println(+ 1 + 3 * 5 / -2 + + 4 / 3); // -5
        if (true && false) {
            System.out.println("true");
        } else {
            System.out.println("false");
        }
        if (!false && true || (false == true)) {
            System.out.println("true");
        } else {
            System.out.println("false");
        }
        if (10 > 5 || 3 == 5) {
            System.out.println("true");
        } else {
            System.out.println("false");
        }
        System.out.println(new classA().hello());
        System.out.print("Hello \tWorld\n\"Hi\\\"");
    }}
}

class classA {
    int a;
    int[] x;

    public int hello() {
        a = 4;
        x = new int[10];
        x[5] = 3;
        System.out.println("hi");
        System.out.println(new classB().test(a + x[5], x));
        System.out.print("Returns ");
        return 0;
    }
}

class classB {
    public int test(int a, int[] x) {
        int b;
        System.out.print("a = ");
        System.out.println(a);
        System.out.print("x.length = ");
        System.out.println(x.length);
        b = 3;
        while (b > 0) {
            if (b == 1) {
                System.out.println("b == 1!!");
            } else {
                System.out.println("b != 1");
            }
            b = b - 1;
        }

        if (this.b()) {
            System.out.println("true");
        } else {
            System.out.println("false");
        }

        System.out.print("Returns ");
        return 10;
    }

    public boolean b() {
        System.out.println("I'm B!");
        return true;
    }
}
