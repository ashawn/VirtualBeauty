/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.web.service;

import io.reactivex.Observable;
import okhttp3.ResponseBody;
import retrofit2.http.GET;
import retrofit2.http.Url;

/**
 * 测试用的服务器，主要提供在线渲染方案信息获取
 *
 * @author laizhenqi
 * @since 2017/2/10
 */
public interface MyService {

    @GET
    Observable<ResponseBody> downloadFileWithDynamicUrlSync(@Url String url);

}
