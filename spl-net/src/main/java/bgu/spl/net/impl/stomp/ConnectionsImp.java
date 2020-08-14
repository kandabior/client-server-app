package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.ConnectionHandler;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.Frame;
import bgu.spl.net.srv.Header;



import java.io.IOException;
import java.util.LinkedList;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

public class ConnectionsImp implements Connections<Frame> {

    private static ConnectionsImp instance = new ConnectionsImp();


    private ConcurrentLinkedQueue<Client> Clients;
    private ConcurrentHashMap<Integer, Client> Active_Clients_Id;
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<IdSubscription>> Channels_Id;
    private ConcurrentHashMap<Integer, ConnectionHandler> Id_CH;


    private ConnectionsImp() {
        Clients = new ConcurrentLinkedQueue<>();
        Active_Clients_Id = new ConcurrentHashMap<>();
        Channels_Id = new ConcurrentHashMap<>();
        Id_CH = new ConcurrentHashMap<>();

    }

    public static ConnectionsImp getInstance() {
        return instance;
    }

    @Override
    public boolean send(int connectionId, Frame msg) {
        Client client=Active_Clients_Id.get(connectionId);
        if(client!=null) {
            synchronized (client) {
                ConnectionHandler connectionHandler = Id_CH.get(connectionId);
                if (connectionHandler != null) {
                    connectionHandler.send(msg);

                    return true;
                } else {
                    return false;
                }
            }
        }
        else{
            ConnectionHandler connectionHandler = Id_CH.get(connectionId);
            if (connectionHandler != null) {
                connectionHandler.send(msg);
                return true;
            } else {
                return false;
            }
        }

    }

    @Override
    public void send(String channel, Frame msg) {
        ConcurrentLinkedQueue<IdSubscription> queue = Channels_Id.get(channel);
        if (queue != null) {
            for (IdSubscription sub : queue) {
                msg.setSubscriptionId(sub.getIdSubscription());
                send(sub.getId_client(), msg);
            }
        }
    }


    @Override
    public void disconnect(int connectionId) {
        Client client= Active_Clients_Id.remove(connectionId);
        for (ConcurrentLinkedQueue<IdSubscription> queue : Channels_Id.values()) {
            queue.removeIf(sub -> sub.getId_client() == connectionId);
        }
        if(client!=null) {
            synchronized (client) {
                Id_CH.get(connectionId);
                Id_CH.remove(connectionId);
            }
        }
    }


    public boolean Connect(String name, String passcode, int connectId) {
        if (Active_Clients_Id.get(connectId) != null) {
            Frame frame = new Frame("ERROR", "User already logged in", new LinkedList<Header>());
            send(connectId, frame);
            disconnect(connectId);
            return false;
        }
        for (Client client : Clients) {
            if (client.getUserName().equals(name)) {
                if (checkConteinsName(name)) {
                    Frame frame = new Frame("ERROR", "User already logged in", new LinkedList<Header>());
                    send(connectId, frame);
                    disconnect(connectId);
                    return false;
                }
                if (client.getPassCode().equals(passcode)) {
                    Active_Clients_Id.put(connectId, client);
                    return true; //User exist

                } else {
                    Frame frame = new Frame("ERROR", "Wrong password", new LinkedList<Header>());
                    send(connectId, frame);
                    disconnect(connectId);
                    return false;
                }
            }
        }
        Client newClient = new Client(name, passcode);
        Active_Clients_Id.put(connectId, newClient);
        Clients.add(newClient);
        return true;
    }

    private boolean checkConteinsName(String name) {
        for (Client c : Active_Clients_Id.values()) {
            if (c.getUserName().equals(name)) {
                return true;
            }
        }
        return false;
    }


    public void Subscribe(String channel, int id_Sub, int connectionId) {
        if (Channels_Id.get(channel) == null) {
            synchronized (Channels_Id) {
                if (Channels_Id.get(channel) == null) {
                    Channels_Id.put(channel, new ConcurrentLinkedQueue());
                }
            }
        }

        Channels_Id.get(channel).add(new IdSubscription(connectionId, id_Sub));

    }



    public void UnSubscribe(int id_Sub, int connectionId) {
        for (ConcurrentLinkedQueue<IdSubscription> queue : Channels_Id.values()) {

            for (IdSubscription idSubscription : queue) {
                if (idSubscription.getIdSubscription() == id_Sub && idSubscription.getId_client() == connectionId) {
                    queue.remove(idSubscription);
                    return;
                }
            }

        }
    }

//    public Integer getIdSub(String channel, int IdConnection){
//        ConcurrentLinkedQueue<IdSubscription> q= Channels_Id.get(channel);
//        for(IdSubscription id : q){
//            if(id.getId_client()==IdConnection){
//                return id.getIdSubscription();
//            }
//        }
//        return null;
//    }

    public void setId_CH(Integer id, ConnectionHandler id_CH) {
            Id_CH.put(id, id_CH);
    }
}
