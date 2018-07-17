import java.util.Scanner;
import java.util.Arrays;
import java.io.File;
import java.io.FileReader;
import java.io.FileInputStream;


public class Test {

    public static void printUsageAndExit(String[] args) throws InterruptedException {
        System.out.println("usage: " + new java.io.File(Test.class.getProtectionDomain()
                    .getCodeSource()
                    .getLocation()
                    .getPath())
                .getName() + " /path/to/filter_in_file /path/to/filter_out_golden");
        System.exit(-1);
    }

    public static boolean files_equal(String filename1, String filename2) {
        boolean areFilesIdentical = true;
        File file1 = new File(filename1);
        File file2 = new File(filename2);

        if (!file1.exists() || !file2.exists()) {
            System.out.println("One or both files do not exist");
            System.out.println(false);
        }

        System.out.println("length:" + file1.length());
        if (file1.length() != file2.length()) {
            System.out.println("lengths not equal");
            System.out.println(false);
        }

        try {
            FileInputStream fis1 = new FileInputStream(file1);
            FileInputStream fis2 = new FileInputStream(file2);
            int i1 = fis1.read();
            int i2 = fis2.read();
            while (i1 != -1) {
                if (i1 != i2) {
                    areFilesIdentical = false;
                    break;
                }
                i1 = fis1.read();
                i2 = fis2.read();
            }
            fis1.close();
            fis2.close();
        } catch (java.io.IOException e) {
            System.out.println("IO exception");
            areFilesIdentical = false;
        }

        return areFilesIdentical;
    }

    public static void main(String[] args) throws InterruptedException, java.io.FileNotFoundException, java.io.IOException {
        firjava.Filter_LP filter = new firjava.Filter_LP(51, 1000, 7);

        if (args.length != 2)
        {
            printUsageAndExit(args);
        }

        FileReader fin = new FileReader(args[0]);
        Scanner src = new Scanner(fin);

        java.io.File tf = java.io.File.createTempFile("java", "filtered");

        java.io.BufferedWriter bw = new java.io.BufferedWriter(new java.io.FileWriter(tf));

        while (src.hasNext()) {
            if (src.hasNextDouble()) {
                bw.append(String.format("%.8f\n", filter.do_sample(src.nextDouble())));
            } else {
                String str = src.next();
            }
        }

        fin.close();
        bw.close();

        /* Now, compare the results */
        if (!files_equal(args[1], tf.getAbsolutePath()))
        {
            System.out.println("Files are not equal: " + args[1] + " " + tf.getAbsolutePath());
            System.exit(-1);
        }

        tf.deleteOnExit();
    }
}
