package com.nuclearinstruments.jscisdk;

public class Ref<T> {
    public T value;
    
    public Ref(T value){
        this.value = value;
    }
    
    public Ref(){
        this.value = null;
    }
    
    public void set(T value){
        this.value = value;
    }
    
    public T get(){
        return value;
    }
}
