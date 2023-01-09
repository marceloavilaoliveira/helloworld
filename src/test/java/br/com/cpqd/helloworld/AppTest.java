package br.com.cpqd.helloworld;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Unit test for simple App.
 */
public class AppTest
    extends TestCase
{
    /**
     * Create the test case
     *
     * Alguma coisa aqui
     *
     * @param testName name of the test case
     */
    public AppTest( String testName )
    {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     *
     * This is a test
     *
     * b + a = ba
     * It's a kind of magic
     * Dont't stop me now
     * To be or not to be?
     *
     */
    public static Test suite()
    {
        return new TestSuite( AppTest.class );
    }

     public void testIsOk()
     {
         assertEquals(true, App.isOk());
     }
}
