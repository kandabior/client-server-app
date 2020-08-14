package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.Frame;
import bgu.spl.net.srv.Header;

import java.sql.Connection;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class StompMessagingProtocolImp<T> implements StompMessagingProtocol<Frame> {
    private boolean shouldTerminate = false;
    private ConnectionsImp connections;
    private int connectionId;
    private static AtomicInteger messageId=new AtomicInteger(1);

    @Override
    public void start(int connectionId, Connections<Frame> connections) {
        this.connections=(ConnectionsImp) connections;
        this.connectionId=connectionId;


    }

    public void Terminate(){
        shouldTerminate=true;
    }
    @Override
    public void process(Frame message) {
        System.out.println("server get: \n" + message.toString());
        if (message.getName().equals("CONNECT")) {
            String isOk = message.Check(new String[]{"accept-version", "host", "login", "passcode"});
            if (isOk.equals("ok")) {
                if (connections.Connect(message.getValueHeader("login"), message.getValueHeader("passcode"), connectionId)) {
                    Header header1 = new Header("version", message.getValueHeader("accept-version"));
                    List<Header> headers = new LinkedList<>();
                    headers.add(header1);
                    if (message.Check(new String[]{"receipt"}).equals("ok")) {
                        Header header2 = new Header("receipt-id", message.getValueHeader("receipt"));
                        headers.add(header2);
                    }
                    Frame frame = new Frame("CONNECTED", "", headers);
                    connections.send(connectionId, frame);
                }
            } else {
                SendErrorMessage(isOk, message);
            }
        } else if (message.getName().equals("SUBSCRIBE")) {
            String isOk = message.Check(new String[]{"destination", "id", "receipt"});
            if (isOk.equals("ok")) {
                connections.Subscribe(message.getValueHeader("destination"), Integer.parseInt(message.getValueHeader("id")), connectionId);
                Header header1 = new Header("receipt-id", message.getValueHeader("receipt"));
                List<Header> headers = new LinkedList<>();
                headers.add(header1);
                Frame frame = new Frame("RECEIPT", "", headers);
                connections.send(connectionId, frame);
            } else {
                SendErrorMessage(isOk, message);
            }
        } else if (message.getName().equals("UNSUBSCRIBE")) {
            String isOk = message.Check(new String[]{"id",});
            if (isOk.equals("ok")) {
                connections.UnSubscribe(Integer.parseInt(message.getValueHeader("id")), connectionId);
                Header header1 = new Header("receipt-id", message.getValueHeader("receipt"));
                List<Header> headers = new LinkedList<>();
                headers.add(header1);
                Frame frame = new Frame("RECEIPT", "", headers);
                connections.send(connectionId, frame);
            } else {
                SendErrorMessage(isOk, message);
            }
        } else if (message.getName().equals("SEND")) {
            String isOk = message.Check(new String[]{"destination"});
            if (isOk.equals("ok")) {
                Header header1 = new Header("subscription", "0");
                int messageId = StompMessagingProtocolImp.messageId.getAndIncrement();
                Header header2 = new Header("Message-id", String.valueOf(messageId));
                Header header3 = new Header("destination", message.getValueHeader("destination"));
                List<Header> headers = new LinkedList<>();
                headers.add(header1);
                headers.add(header2);
                headers.add(header3);
                Frame frame = new Frame("MESSAGE", message.getBody(), headers);
                connections.send(message.getValueHeader("destination"), frame);
                if (message.Check(new String[]{"receipt"}).equals("ok")) {
                    List<Header> headers2 = new LinkedList<>();
                    Header header4 = new Header("receipt-id", message.getValueHeader("receipt"));
                    headers2.add(header4);
                    Frame frame2=new Frame("RECEIPT","", headers2);
                    connections.send(connectionId,frame2);
                }
            }
            else {
                SendErrorMessage(isOk, message);
            }
        } else if (message.getName().equals("DISCONNECT")) {
            String isOk = message.Check(new String[]{"receipt"});
            if (isOk.equals("ok")) {
                Header header1 = new Header("receipt-id", message.getValueHeader("receipt"));
                List<Header> headers = new LinkedList<>();
                headers.add(header1);
                Frame frame = new Frame("RECEIPT", "", headers);
                connections.send(connectionId, frame);
                shouldTerminate=true;
                connections.disconnect(connectionId);

            } else {
                SendErrorMessage(isOk, message);
            }

        }
    }


    public void SendErrorMessage(String isOk,Frame message) {
        Header header1 = new Header("message", "malformed frame received");
        List<Header> headers = new LinkedList<>();
        headers.add(header1);
        Frame frame = new Frame("Error", "The message: \n ----- \n" + message.toStringError() + " \n ----- \n" +
                " Did not contain a " + isOk + " header, which is REQUIRED for message propagation", headers);
        connections.send(connectionId, frame);
        connections.disconnect(connectionId);
    }




    @Override
    public boolean shouldTerminate() {
        return shouldTerminate;
    }
}
