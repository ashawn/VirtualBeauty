/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.data.render;

import java.util.List;

import io.reactivex.Single;

/**
 * 渲染方案仓库，用来获取当前本机可用的渲染方案
 * @author laizhenqi
 * @since 2017/2/8
 */
public interface RenderSchemeRepository {

    /**
     * 获取本机可用的渲染方案，该方法为异步调用<br/>
     * 分别获取assets中存储方案、本地disk缓存方案、网络远程方案
     * @return 响应式结果列表
     */
    Single<List<RenderScheme>> getRenderSchemeList() ;
}
