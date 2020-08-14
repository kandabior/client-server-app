package bgu.spl.net.impl.stomp;
import bgu.spl.net.srv.Server;




public class StompServer {


    public static void main(String[] args) {

        String server=args[1];
        int port=Integer.parseInt(args[0]);
        if(server.equals("reactor")){
            Server.reactor(
                    10,
                    port, //port
                    () ->  new StompMessagingProtocolImp(), //protocol factory
                    MessageEncoderDecoderImp::new //message encoder decoder factory
            ).serve();
        }
        else if(server.equals("tpc")){
            Server.threadPerClient(
                    port, //port
                    () -> new StompMessagingProtocolImp(), //protocol factory
                    MessageEncoderDecoderImp::new //message encoder decoder factory
            ).serve();
        }
        System.out.println("It was a pleasure to serve you!");
    }
}
