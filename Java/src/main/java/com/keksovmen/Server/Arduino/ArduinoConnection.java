package com.keksovmen.Server.Arduino;

import com.keksovmen.Server.IO.TimedReader;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;


public class ArduinoConnection {

    public static final Charset DEFAULT_CHARSET = StandardCharsets.US_ASCII;


    private final Socket socket;
    private final TimedReader reader;
    private final PrintWriter writer;

    private int id = -1;
    private String imei = "";

    /**
     * Im millis acquired from {@link System#currentTimeMillis()}
     */
    private long lastPingTime;

    /**
     * True means i need to ping
     * False means time has not come yet
     */

    private boolean readyFoPing = false;


    public ArduinoConnection(Socket socket) throws IOException {
        this.socket = socket;
        try {
            reader = new TimedReader(new BufferedInputStream(socket.getInputStream()), DEFAULT_CHARSET);
            writer = new PrintWriter(socket.getOutputStream(), true, DEFAULT_CHARSET);
            defineId();
            defineIMEI();
            lastPingTime = System.currentTimeMillis();
        } catch (IOException e) {
            close();
            throw e;
        }
    }


    public synchronized boolean handlePing() {
        System.out.println("Handling ping");
        if (!isAlive()) {
            return false;
        }

        try {
            if (!tryToWrite("#PING$")) {
                return false;
            }

            if (readResponse() != ArduinoResponses.PONG) {
                return false;
            }
        } catch (IOException e) {
            close();
            return false;
        }

        dropReadyForPing();
        return true;
    }

    public synchronized boolean transferData(String data) {
        if (!isAlive()) {
            return false;
        }

        try {
            if (!tryToWrite(data)) {
                return false;
            }

            if (readResponse() != ArduinoResponses.OK) {
                return false;
            }
        } catch (IOException e) {
            close();
            return false;
        }

        return true;
    }

    public void close() {
        try {
            socket.close();
        } catch (IOException ignored) {
        }
    }

    public boolean isAlive() {
        return !socket.isClosed();
    }

    public boolean isTimerForPing(long expectedTimePassed, long currentTime) {
        if (currentTime - lastPingTime > expectedTimePassed) {
            readyFoPing = true;
            return true;
        }

        return false;
    }

    public int getId() {
        return id;
    }

    public String getImei() {
        return imei;
    }

    public boolean isReadyFoPing() {
        return readyFoPing;
    }

    private void defineId() throws IOException {
        String s = reader.readString(20_000);
        if (!s.matches("^-?\\d+")) {
            throw new IOException();
        }
        id = Integer.parseInt(s);
    }

    private void defineIMEI() throws IOException {
        String s = reader.readString(20_000);
        if (!s.matches("^\\d{15,20}")) {
            throw new IOException();
        }
        imei = s;
    }

    private short readShort() throws IOException {
        return Short.parseShort(reader.readString(20_000));
    }

    private ArduinoResponses readResponse() throws IOException {
        return ArduinoResponses.fromInt(readShort());
    }

    private boolean tryToWrite(String str) {
        writer.println(str);
        if (writer.checkError()) {
            close();
            return false;
        }

        return true;
    }

    private void dropReadyForPing() {
        readyFoPing = false;
        //TODO: probably is the worst case when you will have to zoom amount of threads
        lastPingTime = System.currentTimeMillis();
    }
}
