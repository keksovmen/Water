import java.sql.*;

public class BDTest {

    public static void main(String[] args) {
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
            return;
        }

        Connection connection;
        try {
            connection = DriverManager.getConnection("jdbc:mysql://localhost/arduino?" +
                    "user=java&password=1234");
        } catch (SQLException ex) {
//            e.printStackTrace();
            System.out.println("SQLException: " + ex.getMessage());
            System.out.println("SQLState: " + ex.getSQLState());
            System.out.println("VendorError: " + ex.getErrorCode());
            return;
        }

        Statement statement = null;
//        ResultSet resultSet = null;
        try {
            statement = connection.createStatement();
            int res = statement.executeUpdate("INSERT INTO arduino (arduino_id, imei, is_active) values(5, 5, 1)");
            System.out.println(res);
        } catch (SQLException e) {
            e.printStackTrace();
        }finally {
//            if(resultSet != null) {
//                try {
//                    resultSet.close();
//                } catch (SQLException ignored) {}
//            }

            if(statement != null) {
                try {
                    statement.close();
                } catch (SQLException ignored) {}
            }

            try {
                connection.close();
            } catch (SQLException ignored) {}
        }

    }
}
