package com.keksovmen;

import com.keksovmen.Server.Arduino.ArduinoServer;
import com.keksovmen.Server.Web.WebServer;

import java.io.IOException;

public class Main {

    private static ArduinoServer arduinoServer;
    private static WebServer webServer;


    public static void main(String[] args) {
        if (args.length < 2 || args.length > 2) {
            System.err.println("Error wrong amount of arguments");
            return;
        }

        try {
            arduinoServer = new ArduinoServer(Integer.valueOf(args[0]));
        } catch (IOException e) {
            System.err.println("Can't create server for arduino");
            return;
        }

        try {
            webServer = new WebServer(Integer.valueOf(args[1]), arduinoServer.createTransferHandler());
        } catch (IOException e) {
            System.err.println("Can't create server for web");
            return;
        }


        arduinoServer.run();
        webServer.run();

    }


}
