package com.keksovmen.Server.Arduino;

public enum ArduinoResponses {
    PONG,
    OK,
    FAIL;

    public static ArduinoResponses fromInt(int i) {
        switch (i) {
            case 0:
                return PONG;
            case 1:
                return OK;
            case 2:
                return FAIL;
        }

        return FAIL;
    }
}
