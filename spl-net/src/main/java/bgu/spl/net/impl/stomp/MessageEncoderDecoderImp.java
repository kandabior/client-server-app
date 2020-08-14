package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.srv.Frame;
import bgu.spl.net.srv.Header;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

public class MessageEncoderDecoderImp implements MessageEncoderDecoder<Frame> {
    private List<Byte> bytes = new LinkedList<>();
    private int len=0;


    @Override
    public Frame decodeNextByte(byte nextByte) {
        if(nextByte== '\u0000'){
            String str= popString();
            return StringToFrame(str);

        }

        pushByte(nextByte);
        return null; //not a line yet
    }

    private void pushByte(byte nextByte) {
        bytes.add(nextByte);
        len++;
    }

    private String popString() {
        byte[] arr = new byte[bytes.size()];
        for(int i=0; i<bytes.size(); i++){
          arr[i] =  bytes.get(i);
        }
        String result = new String(arr, 0, len, StandardCharsets.UTF_8);
        len = 0;
        bytes.clear();
        return result;
    }

    private Frame StringToFrame(String str){
        String [] arr = str.split("\n");
        int i=0;
        String name = arr[i++];
        List<Header> headers = new LinkedList<>();
        for(i=1; i<arr.length && arr[i].length() != 0; i++) {
            headers.add(StringToHeader(arr[i]));
        }
        i++;
        String body="";
        if(i<arr.length && arr[i]!= "^@" ){
            body = arr[i++];
        }
        return new Frame(name,body,headers);
    }

    private Header StringToHeader(String header){
        String [] arr = header.split(":");
        return new Header(arr[0],arr[1]);
    }

    @Override
    public byte[] encode(Frame message) {
        return (message.toString()).getBytes();//  \u0000
    }
}
