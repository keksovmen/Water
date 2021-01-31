package com.keksovmen.Server.Web;

import com.keksovmen.Server.IO.TimedReader;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;

public class WebConnection {

    private static final Charset DEFAULT_CHARSET = StandardCharsets.US_ASCII;
    private static final long MAX_WAIT_TIME = 10_000;

    private final Socket socket;
    private final TimedReader reader;
    private final PrintWriter writer;

    private final int destinationId;
    private final String data;


    public WebConnection(Socket socket) throws IOException {
        this.socket = socket;
        try {
            reader = new TimedReader(socket.getInputStream(), DEFAULT_CHARSET);
            writer = new PrintWriter(socket.getOutputStream(), true, DEFAULT_CHARSET);
            destinationId = fetchId();
            data = fetchData();
        }catch (IOException e){
            close();
            throw e;
        }
    }


    public void finishWitCode(WebResponses responsesCode){
        writer.println(WebResponses.toInt(responsesCode));
        try {
            socket.close();
        } catch (IOException ignored) {}
    }

    public void close(){
        try {
            socket.close();
        } catch (IOException ignored) {}
    }


    protected int fetchId() throws IOException {
        return Integer.parseInt(reader.readString(MAX_WAIT_TIME));
    }

    protected String fetchData() throws IOException {
        return reader.readString(MAX_WAIT_TIME);
    }


    public int getDestinationId() {
        return destinationId;
    }

    public String getData() {
        return data;
    }
}
