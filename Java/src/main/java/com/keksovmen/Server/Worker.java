package com.keksovmen.Server;

import com.keksovmen.Server.Arduino.ArduinoConnection;

import java.io.IOException;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;

public class Worker implements Runnable {

    public static final Charset DEFAULT_CHARSET = Charset.forName("UTF-8");

//    private final Timer timer = new Timer();
    private final Runnable removeMe;
    private final Thread me;
    private ArduinoConnection connection;

//    private boolean pingSent = false;
//    private long timePingSend;
//    CharBuffer buffer = CharBuffer.allocate(32);


    public Worker(Socket socket, Runnable removeMe) throws IOException {
        connection = new ArduinoConnection(socket);
        this.removeMe = removeMe;
        me = Thread.currentThread();

        updateTime();
    }


    @Override
    public void run() {
        while (connection.isAlive()) {
            synchronized (this) {
                if (!connection.handlePing()) {
                    break;
                }
            }

//                readIncoming();
//            sendPing();
//                if (checkFileExistence()) {
//                    sendData();
//                } else {
            try {
                Thread.sleep(30000);
            } catch (InterruptedException ignored) {
            }
//                }

        }


        removeMe.run();
        System.out.println("Finished");
    }


//    private void readIncoming() throws IOException {
//        while (input.available() > 0) {
//            char tmp = (char) input.read();
//            buffer.append(tmp);
//            System.out.print(tmp);
//        }
//
//        if (String.valueOf(buffer.array(), 0, buffer.position()).contains("PONG\n")) {
//            System.out.println("PONG received");
//            buffer.clear();
//            pingSent = false;
//        }
//
//    }


    public synchronized boolean sendData(ByteBuffer data) throws IOException {
//        boolean result = connection.transferData(data);
//        if(!result && !connection.isAlive()){
//            me.interrupt();
//        }
//
//        return result;
//        Scanner scanner = null;
//
//        try {
//            scanner = new Scanner(Files.newInputStream(
//                    filePath,
//                    StandardOpenOption.READ,
//                    StandardOpenOption.DELETE_ON_CLOSE),
//                    DEFAULT_CHARSET);
//        } catch (IOException ignored) {
//        }
//
//        if (scanner == null) {
//            System.err.println("Can't open file to read");
//            return;
//        }
//
//
//        while (scanner.hasNext()) {
//            String data = scanner.nextLine() + "\n";
//            System.out.println("Sending: " + data);
//            if (data.length() == 0) {
//                continue;
//            }
//
//            output.write(data.getBytes(DEFAULT_CHARSET));
//        }
//
//        scanner.close();
        return true;
    }


//    private boolean checkFileExistence() {
//        return Files.exists(filePath);
//    }

    //TODO: if no answer received for some time disconnect
    private void sendPing() {
//        if (!pingSent && isTimePassed()) {
//            output.write("#PING$".getBytes(DEFAULT_CHARSET));
//            pingSent = true;
//            updateTime();
//        }
//        if (pingSent && isTimePassed()) {
//            socket.close();
//        }
//        if (isTimePassed()) {
//            connection.handlePing();
//        }
    }


    private void updateTime() {
//        timer.schedule(new TimerTask() {
//            @Override
//            public void run() {
//
//            }
//        }, 5000, 30000);
//        timePingSend = System.currentTimeMillis();
    }


//    private boolean isTimePassed() {
//        return System.currentTimeMillis() - timePingSend > 5000;
//    }
}
