package com.keksovmen.Server.Web;

public enum WebResponses {
    NO_SUCH_ID,
    OK,
    FAILED;

    public static int toInt(WebResponses responses){
        switch (responses){
            case NO_SUCH_ID:
                return 0;
            case OK:
                return 1;
            case FAILED:
                return 2;
        }

        return -1;
    }
}
