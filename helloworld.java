class MainClass {
	public static void main (String [] id) {
        {
            System.out.println("Hello world");
            System.out.println(5 + 3);
            if (true) {
                System.out.print("Should print with num ");
                System.out.println(5 / 2);
            } else {
                System.out.println("Should not print");
            }
            System.out.println(new HelloClass().hello());
        }
	}

}

class HelloClass {
    public int hello() {
        System.out.println("Hello from hello()");
        System.out.print("Return value is ");
        return -1;
    }
}
