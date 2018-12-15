/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.data.render;

import android.net.Uri;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.cvte.virtualbeauty.gles.GlComponent;
import com.cvte.virtualbeauty.web.domain.model.ComponentModel;
import com.cvte.virtualbeauty.web.domain.model.RenderSchemeModel;
import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;
import java.util.List;

/**
 * 渲染方案的基本信息，包含预览图、名称、包含的shader信息
 *
 * @author laizhenqi
 * @since 2017/2/8
 */
public final class RenderScheme {

    private static final String TAG = RenderScheme.class.getSimpleName();

    static RenderScheme empty() {
        return new RenderScheme();
    }

    /**
     *
     */
    @Nullable
    static RenderScheme parse(RenderSchemeModel model) {
        return parse(model, false);
    }

    /**
     * <p>将{@link RenderSchemeModel}转化为RenderSchemeModel,如转化失败，则返回null</p>
     *
     * @param model
     * @param flag  是否置为已经缓存，如设为已经缓存，则内部所有uri会校验是否为网络资源，如是网络资源则抛出异常
     * @return 返回
     */
    @Nullable
    static RenderScheme parse(@Nullable RenderSchemeModel model, boolean flag) {
        if (model == null) return null;
        RenderScheme rs = new RenderScheme();
        rs.isCached = flag;
        rs.id = model.getId();
        rs.name = model.getName();
        rs.previewUri = Uri.parse(model.getPreviewUri());
        rs.mComponentList = new ArrayList<>();
        for (ComponentModel cm : model.getList()) {
            rs.mComponentList.add(parse(cm));
        }
        return rs;
    }

    private static GlComponent parse(@NonNull ComponentModel model) {
        GlComponent glComponent = new GlComponent();
        glComponent.setPosition(model.getPosition());
        glComponent.setType(model.getType());
        glComponent.setValue(model.getValue());
        return glComponent;
    }

    /**
     * 渲染方案id，理论上不可重复
     */
    private long id;

    /**
     * 预览图uri，可以是本地文件、资源文件、网络图片
     */
    private Uri previewUri;

    /**
     * 该着色方案的名字
     */
    private String name;

    /**
     * 是否已经缓存了渲染方案
     */
    private boolean isCached;

    /**
     * shader列表，可能为复数个
     */
    @SerializedName("components")
    private List<GlComponent> mComponentList;

    private RenderScheme() {
    }

    public Uri getPreviewUri() {
        return previewUri;
    }

    public String getName() {
        return name;
    }

    public boolean isCached() {
        return isCached;
    }

    public List<GlComponent> getComponentList() {
        return mComponentList;
    }

    @Override
    public String toString() {
        return "{id:" + id + " name:" + name + " preview:" + previewUri + " isCached:" + isCached + "}";
    }

}
