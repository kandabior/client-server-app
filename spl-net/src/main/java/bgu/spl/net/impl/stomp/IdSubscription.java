package bgu.spl.net.impl.stomp;

public class IdSubscription {
    private Integer IdSubscription;
    private Integer id_client;

    public IdSubscription(Integer id_client, Integer idSubscription) {
        this.id_client = id_client;
        this.IdSubscription=idSubscription;
    }

    public Integer getId_client() {
        return id_client;
    }

    public Integer getIdSubscription() {
        return IdSubscription;
    }
}

