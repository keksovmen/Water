package com.keksovmen.Server.Web;

import com.keksovmen.Server.AbstractServer;

import java.io.IOException;
import java.net.Socket;
import java.util.function.Consumer;

public class WebServer extends AbstractServer {

    private final Consumer<WebConnection> transferHandler;


    public WebServer(int port, Consumer<WebConnection> transferHandler) throws IOException {
        super(port);
        this.transferHandler = transferHandler;
    }


    @Override
    protected boolean acceptConnection(Socket socket) {
        WebConnection connection = null;
        try {
            connection = new WebConnection(socket);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        transferHandler.accept(connection);
        return true;
    }

    @Override
    protected String getServerName() {
        return "Web";
    }

}
