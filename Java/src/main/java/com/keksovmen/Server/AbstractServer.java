package com.keksovmen.Server;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

public abstract class AbstractServer implements Runnable {

    protected final ServerSocket serverSocket;

    public AbstractServer(int port) throws IOException {
        serverSocket = new ServerSocket(port);
    }


    /**
     * Starts new thread for {@link ServerSocket#accept()) if possible
     */

    @Override
    public void run() {
        new Thread(() -> {
            while (true) {
                try {
                    Socket socket = serverSocket.accept();
                    ThreadManager.getInstance().execute(() -> {
                        if (!acceptConnection(socket)) {
                            System.out.println("Couldn't accept " + getServerName().toLowerCase() + " connection");
                        }
                    });
                } catch (IOException e) {
                    e.printStackTrace();
                    System.out.println(getServerName() + " server died");
                    break;
                }
            }

        },  getServerName() + "-Server").start();
    }


    /**
     * Do something useful here
     *
     * @param socket valid not closed
     * @return true if accepted to the server
     */

    protected abstract boolean acceptConnection(Socket socket);


    /**
     * Start with capital just for some beauty
     *
     * @return server name such as Arduino or PHP
     */

    protected abstract String getServerName();
}
