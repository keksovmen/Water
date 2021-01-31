package com.keksovmen.Server.SQL;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class ArduinoMessanger {

    private static final String DRIVER_NAME = "com.mysql.cj.jdbc.Driver";
    private static final String URI = "jdbc:mysql://localhost/arduino?";
    private static final String USER_NAME = "java";
    private static final String PASSWORD = "1234";

    public static boolean isInitOkay = false;


    static {
        try {
            Class.forName(DRIVER_NAME);
            isInitOkay = true;
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            isInitOkay = false;
        }
    }


    public static boolean connectArduino(int id, String imei) {
        dieIfNotInitialised();

        try (Connection connection = getConnection()) {
            PreparedStatement statement = connection.prepareStatement("INSERT INTO arduino " +
                    "(arduino_id, imei, is_active) " +
                    "VALUES(?, ?, 1) ON DUPLICATE KEY UPDATE arduino_id = VALUES(arduino_id), " +
                    "imei = VALUES(imei), is_active = VALUES(is_active);");
            statement.setInt(1, id);
            statement.setString(2, imei);

            int rowAffected = statement.executeUpdate();
            if(rowAffected == 0){
                System.err.println("Zero rows affected!");
                return false;
            }

        } catch (SQLException e) {
            for (Throwable throwable : e) {
                throwable.printStackTrace();
            }

            return false;
        }

        return true;
    }


    public static boolean updateArduinoState(int id, boolean isActive){
        dieIfNotInitialised();

        try (Connection connection = getConnection()) {
            PreparedStatement statement = connection.prepareStatement("UPDATE arduino SET is_active = ? " +
                    "WHERE arduino_id = ?");
            statement.setInt(1, isActive ? 1 : 0);
            statement.setInt(2, id);

            int rowAffected = statement.executeUpdate();
            if(rowAffected == 0){
                System.err.println("Zero rows affected!");
                return false;
            }

        } catch (SQLException e) {
            for (Throwable throwable : e) {
                throwable.printStackTrace();
            }

            return false;
        }

        return true;
    }


    private static Connection getConnection() throws SQLException {
        return DriverManager.getConnection(URI, USER_NAME, PASSWORD);
    }

    private static void dieIfNotInitialised(){
        if (!isInitOkay)
            throw new IllegalStateException("JDBC driver is not loaded!");
    }
}
