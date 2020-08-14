package bgu.spl.net.srv;

import java.io.Serializable;
import java.lang.reflect.Array;
import java.util.List;

public class Frame implements Serializable {

    private String name;
    private List<Header> headers;
    private String body;

    public Frame(String name,String body, List<Header> headers ){
        this.name=name;
        this.body=body;
        this.headers=headers;
    }

    public String getName() {
        return name;
    }

    public String getBody() {
        return body;
    }

    public List<Header> getHeaders() {
        return headers;
    }

    public String getValueHeader(String name){
        for( Header h: headers){
            if(h.getName().equals(name)){
                return h.getValue();
            }
        }
        return null;
    }

    public String Check(String[] str){
        for(String s: str){
            boolean ok = false;
            for(Header header: headers){
               if(header.getName().equals(s)){
                   ok=true;
               }
            }
            if(!ok){
                return s;
            }
        }
        return "ok";
    }

    @Override
    public String toString() {
        String output="";
        output=output+name+'\n';
        for( Header header : headers){
            output=output+header.toString()+'\n';
        }
        output=output+'\n';
        output=output+body+'\n';
        output=output+'\u0000';
        return output;

    }

    public String toStringError() {
        String output="";
        output=output+name+'\n';
        for( Header header : headers){
            output=output+header.toString()+'\n';
        }
        output=output+'\n';
        output=output+body+'\n';
        return output;

    }

    public void setSubscriptionId(Integer idSubscription) {
        for(Header h : headers){
            if(h.getName().equals("subscription")){
                h.setValue(idSubscription);
                return;
            }

        }
    }
}
