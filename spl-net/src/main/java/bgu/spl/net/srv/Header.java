package bgu.spl.net.srv;

public class Header {

    private String name;
    private String value;

    public Header(String name, String value){
        this.name=name;
        this.value=value;
    }

    public String getName() {
        return name;
    }

    public String getValue() {
        return value;
    }

    @Override
    public String toString() {
        return name+":"+value;
    }

    public void setValue(Integer idSubscription) {
        value=idSubscription.toString();
    }
}
