package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;

public class Client {

    private String UserName;
    private String PassCode;


    public Client(String name, String passCode){
        this.UserName=name;
        this.PassCode=passCode;
    }

    public String getPassCode() {
        return PassCode;
    }

    public String getUserName() {
        return UserName;
    }
}