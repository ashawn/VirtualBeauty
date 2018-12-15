/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.gles;

/**
 * 渲染组件所属部位
 *
 * @author laizhenqi
 * @since 2017/2/9
 */
public class GlComponent {

    public enum Type {
        COLOR, TEXTURE
    }

    private String position;

    private String type;

    private String value;

    public String getPosition() {
        return position;
    }

    public String getType() {
        return type;
    }

    public String getValue() {
        return value;
    }

    public void setPosition(String position) {
        this.position = position;
    }

    public void setType(String type) {
        this.type = type;
    }

    public void setValue(String value) {
        this.value = value;
    }
}
