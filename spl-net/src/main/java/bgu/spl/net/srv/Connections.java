package bgu.spl.net.srv;

import java.io.IOException;
import java.util.concurrent.ConcurrentHashMap;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

//    boolean Connect(String name, String passcode,int id);
//
//    void Subscribe(String channel, int id_Sub, int id_client);
//    Integer getIdSub(String channel, int IdConnection);
//
//    void setId_CH(Integer ID, ConnectionHandler id_CH);
//    void UnSubscribe(int id_Sub, int connectionId);
}
