/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.web.domain.model;

import android.support.annotation.Nullable;
import android.text.TextUtils;

import com.google.gson.Gson;
import com.google.gson.JsonSyntaxException;
import com.google.gson.annotations.SerializedName;

import java.util.List;

/**
 * web端获取的渲染方案类型
 *
 * @author laizhenqi
 * @since 2017/2/10
 */
public class RenderSchemeModel {


    private static final String TAG = RenderSchemeModel.class.getSimpleName();

    /**
     *
     * 从json字符串中读取生成RenderScheme对象
     *
     * @param jsonString 读取的字符串
     * @return RenderScheme对象
     * @throws JsonSyntaxException 如果不能正确序列化，则跑出异常，此异常可能是缺少必要字段
     */
    @Nullable
    public static RenderSchemeModel create(String jsonString) throws JsonSyntaxException {
        if (TextUtils.isEmpty(jsonString)) {
            return null;
        }
        Gson gson = new Gson();
        RenderSchemeModel rs = gson.fromJson(jsonString, RenderSchemeModel.class);
        return rs;
    }

    /**
     * 渲染方案id，理论上不可重复
     */
    private long id;

    /**
     * 预览图uri，可以是本地文件、资源文件、网络图片
     */
    @SerializedName("preview")
    private String previewUri;

    /**
     * 该着色方案的名字
     */
    private String name;

    /**
     * shader列表，可能为复数个
     */
    @SerializedName("components")
    private List<ComponentModel> list;

    public long getId() {
        return id;
    }

    public String getPreviewUri() {
        return previewUri;
    }

    public String getName() {
        return name;
    }

    public List<ComponentModel> getList() {
        return list;
    }

    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();
        sb.append("{id=").append(id)
                .append(" ,name=").append(name)
                .append(" ,preview=").append(previewUri)
                .append(" ,component=[");
        for (ComponentModel c : list) {
            sb.append(c.toString()).append(" ,");
        }
        sb.append("]}");
        return sb.toString();
    }
}
