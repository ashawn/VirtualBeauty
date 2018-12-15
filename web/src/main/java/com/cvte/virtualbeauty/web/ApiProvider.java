/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.web;

import android.content.Context;

import com.cvte.virtualbeauty.web.domain.model.RenderSchemeModel;

import java.util.List;

import io.reactivex.Observable;
import okhttp3.ResponseBody;

/**
 * web请求封装类
 *
 * @author laizhenqi
 * @since 2017/2/10
 */
public class ApiProvider {

    private static ApiProvider sInstance;

    public static ApiProvider getInstance() {
        if (sInstance == null) sInstance = new ApiProvider();
        return sInstance;
    }

    private RestApi mRestApi;

    /**
     * <p>获取所有远程渲染方案</p>
     */
    public Observable<List<RenderSchemeModel>> getRenderSchemes(Context context) {
        if (mRestApi == null) {
            mRestApi = new RestApiImpl(context);
        }
        return mRestApi.getRenderSchemes();
    }

    public Observable<ResponseBody> downloadFileWithDynamicUrlAsync(Context context, String url) {
        if (mRestApi == null) {
            mRestApi = new RestApiImpl(context);
        }
        return mRestApi.downloadFileWithDynamicSync(url);
    }

}
