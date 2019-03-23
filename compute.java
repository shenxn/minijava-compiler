class mainClass {
    public static void main (String [] s) {{
        System.out.println(1 + 2 - 4 + 2); // 1
        System.out.println(1 + 3 * 5 / 2 + 4 / 3); // 9
        System.out.println(1 + 3 * 5 / (2 + 4) / 3); // 1
        System.out.println(+ 1 + 3 * 5 / -2 + + 4 / 3); // -5
        System.out.println(true && false); // false
        System.out.println(!false && true || (false == true)); // true
        System.out.println(10 > 5 || 3 == 5); // true
    }}
}

class classA {
    public int hello() {
        System.out.println("hi");
        return 0;
    }
}
