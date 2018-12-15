/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.web;

import android.content.Context;
import android.util.Log;

import com.cvte.virtualbeauty.web.domain.model.RenderSchemeModel;
import com.cvte.virtualbeauty.web.service.MyService;
import com.google.gson.Gson;
import com.jakewharton.retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory;

import org.json.JSONArray;
import org.json.JSONObject;

import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;

import io.reactivex.Observable;
import okhttp3.ResponseBody;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

/**
 * @author laizhenqi
 * @since 2017/2/10
 */
public class RestApiImpl implements RestApi {

    private static final String TAG = RestApiImpl.class.getSimpleName();
    private Context mContext;

    private MyService mMyService;

    public RestApiImpl(Context context) {
        mContext = context;
        mMyService = new Retrofit.Builder()
                .baseUrl(context.getString(R.string.test_end_point))
                .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
                .addConverterFactory(
                        GsonConverterFactory.create(new Gson()))
                .build()
                .create(MyService.class);
    }

    @Override
    public Observable<List<RenderSchemeModel>> getRenderSchemes() {
        //测试版本从assets中读取渲染方案列表
        return Observable.just("test/test.json")
                .map(s -> {
                    List<RenderSchemeModel> list = new ArrayList<>();
                    try {
                        String jsonStr = InputStreamToString(mContext.getAssets().open(s));
                        JSONObject json = new JSONObject(jsonStr);
                        Gson gson = new Gson();
                        JSONArray array = json.getJSONArray("renderschemes");
                        for (int i = 0; i < array.length(); i++) {
                            list.add(gson.fromJson(array.getJSONObject(i).toString(), RenderSchemeModel.class));
                        }
                    } catch (Exception e) {
                        Log.w(TAG, "open test.json failed", e);
                    }
                    return list;
                });
    }

    @Override
    public Observable<ResponseBody> downloadFileWithDynamicSync(String url) {
        return mMyService.downloadFileWithDynamicUrlSync(url);
    }

    private static String InputStreamToString(InputStream is) {
        int len = 0;
        int r = 0;//总计读取字节数
        byte[] data = new byte[1024];//每次最多读取1024字节
        byte[] result = new byte[1024];//结果字符串，最多存取1024字节
        try {
            while ((len = is.read(data)) > 0) {
                r += len;
                if (r > 1024) {
                    //将之前result拷贝到newbuff中再重新引用
                    byte[] newBuff = new byte[(r / 1024 + 1) * 1024];
                    System.arraycopy(result, 0, newBuff, 0, result.length);
                    result = newBuff;
                }
                System.arraycopy(data, 0, result, r - len, len);
            }
        } catch (Exception e) {
            Log.e(TAG, "convert InputStream to String failed", e);
        } finally {
            if (is != null) try {
                is.close();
            } catch (IOException e1) {
                e1.printStackTrace();
            }
        }
        String s = null;
        try {
            s = new String(result, 0, r, "UTF-8");
        } catch (UnsupportedEncodingException e) {
            Log.e(TAG, "UnsupportedEncodingException", e);
        }
        return s;
    }
}
