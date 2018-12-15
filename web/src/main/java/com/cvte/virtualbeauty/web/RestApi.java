/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.web;

import com.cvte.virtualbeauty.web.domain.model.RenderSchemeModel;

import java.util.List;

import io.reactivex.Observable;
import okhttp3.ResponseBody;

/**
 * rest api接口
 * @author laizhenqi
 * @since 2017/2/10
 */
public interface RestApi {

    Observable<List<RenderSchemeModel>> getRenderSchemes();

    Observable<ResponseBody> downloadFileWithDynamicSync(String url);

}
