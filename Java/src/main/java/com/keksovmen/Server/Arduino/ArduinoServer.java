package com.keksovmen.Server.Arduino;

import com.keksovmen.Server.AbstractServer;
import com.keksovmen.Server.SQL.ArduinoMessanger;
import com.keksovmen.Server.ThreadManager;
import com.keksovmen.Server.Web.WebConnection;
import com.keksovmen.Server.Web.WebResponses;

import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;
import java.util.function.Consumer;

public class ArduinoServer extends AbstractServer {

    /**
     * In millis
     */

    private static final long TIME_BETWEEN_PINGS = 30_000;

    private final ConcurrentHashMap<Integer, ArduinoConnection> connectionMap;
    private final ScheduledThreadPoolExecutor pingExecutor;


    public ArduinoServer(int port) throws IOException {
        super(port);
        connectionMap = new ConcurrentHashMap<>();
        pingExecutor = new ScheduledThreadPoolExecutor(1);
        pingExecutor.scheduleAtFixedRate(this::pingScheduler, 0, 1, TimeUnit.SECONDS);
    }


    @Override
    protected String getServerName() {
        return "Arduino";
    }


    /**
     * If server already contains unique key
     * it will not accept incoming connection
     *
     * @param socket new born connection
     * @return true if added to {@link #connectionMap}
     */

    @Override
    protected boolean acceptConnection(Socket socket) {
        ArduinoConnection connection = null;
        try {
            connection = new ArduinoConnection(socket);
        } catch (IOException e) {
            return false;
        }

        if (connectionMap.containsKey(connection.getId()) &&
                connectionMap.get(connection.getId()).isAlive()) {
            connection.close();
            return false;
        }

        connectionMap.put(connection.getId(), connection);
        if (!ArduinoMessanger.connectArduino(connection.getId(), connection.getImei())) {
            System.out.println("Database, connect arduino failed, disconnecting id - " + connection.getId());
            disconnectConnection(connection);
            return false;
        }

        return true;
    }


    public Consumer<WebConnection> createTransferHandler() {
        return webConnection -> {
            if (!connectionMap.containsKey(webConnection.getDestinationId())) {
                webConnection.finishWitCode(WebResponses.NO_SUCH_ID);
                System.out.println("NO SUCH ID");
                return;
            }
            ArduinoConnection connection = connectionMap.get(webConnection.getDestinationId());
            if (connection.transferData(webConnection.getData())) {
                webConnection.finishWitCode(WebResponses.OK);
            } else {
                webConnection.finishWitCode(WebResponses.FAILED);
            }
        };
    }


    protected void pingScheduler() {
        if (connectionMap.size() == 0)
            return;

        long currentTimeMillis = System.currentTimeMillis();
        //TODO: it seems like it won't throw concurrentModificationException but I am not sure
        connectionMap.forEach((integer, arduinoConnection) -> {
            //check if ping handling in progress
            if (arduinoConnection.isReadyFoPing())
                return;

            //check if time has come for ping handling procedure
            if (arduinoConnection.isTimerForPing(TIME_BETWEEN_PINGS, currentTimeMillis)) {
                //schedule ping action
                ThreadManager.getInstance().execute(createPingHandler(arduinoConnection));
            }
        });
    }


    protected Runnable createPingHandler(ArduinoConnection connection) {
        return () -> {
            if (connection.handlePing())
                return;

            disconnectConnection(connection);


        };
    }


    private void disconnectConnection(ArduinoConnection connection) {
        connectionMap.remove(connection.getId());
        ArduinoMessanger.updateArduinoState(connection.getId(), false);
        connection.close();

        //TODO: checking for some unexpected null references, later remove and handle somehow
        System.out.println("Finished - " + connection.getId());
    }


}
