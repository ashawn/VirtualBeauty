/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.web.domain.model;

/**
 * web端获取的component信息
 *
 * @author laizhenqi
 * @since 2017/2/10
 */
public class ComponentModel {

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

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("{position=").append(position)
                .append(" type=").append(type)
                .append(" value=").append(value)
                .append("}");
        return sb.toString();
    }
}
