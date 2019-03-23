class p_4 {

	public static void main (String [] id) {
            System.out.println(new Cat().woof(false));
	}

}


class Cat {
	int lmao;

    public int woof(boolean x){ //It's Daniel H's fault that the cat says woof
		boolean local;
                int i;
		local = x;
                System.out.println(this.blub(true));
                if(x)
		return 0;
                else {}
                i = new Fish().blub(true);
                return 0;
	}

    public int blub(boolean y){
        System.out.println("Test\n");
        return 0;
    }

}

class Fish {
    Cat c;

    public int blub(boolean y){
        int x;
        c = new Cat();
        x = c.woof(y);

        return 0;
    }

}

class Tuna extends Fish {

    public int blubb(boolean y, boolean x){
        if(x)
            return this.blub(y);
        else {}
        //return -1;

        return 0;
    }
}
