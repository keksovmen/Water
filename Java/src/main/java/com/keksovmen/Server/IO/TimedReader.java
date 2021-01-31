package com.keksovmen.Server.IO;

import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.Charset;
import java.util.Scanner;

public class TimedReader {


    private final InputStream inputStream;
    private final Scanner scanner;
    private final Charset defaultCharset;

    private int available = 0;

    public TimedReader(InputStream inputStream, Charset charset) {
        this.inputStream = inputStream;
        this.scanner = new Scanner(inputStream, charset);
        defaultCharset = charset;
    }


    public String readString(long maxWaitTimeMillis) throws IOException {
        long before = System.currentTimeMillis();

        if (available <= 0) {
            while ((available = inputStream.available()) == 0) {
                if (System.currentTimeMillis() - before > maxWaitTimeMillis) {
                    System.out.println("Time expired");
                    throw new IOException();
                }
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException ignored) {
                }
            }
        }
//        System.out.println("AVAILABLE = " + available);
        String s = scanner.nextLine();
//        scanner.nextLine();
        available -= (s.getBytes(defaultCharset).length + 1);
        System.out.println("Line is: " + s);
        return s;
    }
}
