public class Test {

    public static void main(String[] args) throws InterruptedException {
        firjava.Filter_LP filter = new firjava.Filter_LP(51, 1000, 10);

        System.out.println("filter " + filter.do_sample(10));
    }
}
