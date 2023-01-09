package br.com.cpqd.helloworld;

/**
 * Bla File
 */
public class Bla {

    public static final int AAA = 111;
    public static final int BBB = 222;
    public static final int CCC = 333;
    public static final int DDD = 4;
    public static final int EEE = 555;
    public static final int FFF = 6;
    public static final int GGG = 7;
    public static final int ZZZ = 147;

    /**
     * This is a test file
     */
    static void bla()
    {
        int var1 = AAA;
        int iii = BBB;
        int this_var_is_not_used = 147741;
        int not_used_too = 741;

        if (var1 == AAA)  {
            var1 = BBB;
        } else {
            iii = AAA;
        }

        if (iii == BBB)  {
            var1 = AAA;
        } else {
            iii = BBB;
        }

        return;
    }
}
