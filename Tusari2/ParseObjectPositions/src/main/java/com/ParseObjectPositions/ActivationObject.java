package com.ParseObjectPositions;

public class ActivationObject {
    String name;

    public int getXpos() {
        return xpos;
    }

    public int getYpos() {
        return ypos;
    }

    int xpos , ypos;

    public ActivationObject(String name, double xpos, double ypos) {
        this.name = name;
        this.xpos = (int)Math.round(xpos);
        this.ypos = (int)Math.round(ypos);
    }
}
