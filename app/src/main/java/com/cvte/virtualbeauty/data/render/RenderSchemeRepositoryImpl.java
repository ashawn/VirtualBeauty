/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.data.render;

import android.content.Context;
import android.support.annotation.NonNull;
import android.util.Log;

import com.cvte.virtualbeauty.common.util.ReaderUtils;
import com.cvte.virtualbeauty.web.ApiProvider;
import com.cvte.virtualbeauty.web.domain.model.RenderSchemeModel;

import java.io.FileInputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import javax.inject.Inject;

import io.reactivex.Observable;
import io.reactivex.Single;

/**
 * @author laizhenqi
 * @since 2017/2/8
 */
public class RenderSchemeRepositoryImpl implements RenderSchemeRepository {

    private static final String TAG = RenderSchemeRepositoryImpl.class.getSimpleName();

    private Context mContext;

    @Inject
    RenderSchemeRepositoryImpl(Context context) {
        mContext = context;
    }

    @Override
    @NonNull
    public Single<List<RenderScheme>> getRenderSchemeList() {
        return Observable.concatDelayError(Arrays.asList(mFromAssets,
                mFromLocal,
                ApiProvider.getInstance().getRenderSchemes(mContext)
                        .flatMap(Observable::fromIterable)
                        .map(rsModel -> {
                            try {
                                RenderScheme rs = RenderScheme.parse(rsModel);
                                return rs == null ? NOT_FOUND : rs;
                            } catch (Exception e) {
                                throw new Exception("parse from remote " + rsModel.toString() + "failed");
                            }
                        })
                        .onErrorReturn(e -> {
                            Log.w(TAG, e.getMessage());
                            return NOT_FOUND;
                        })
                        .filter(target -> NOT_FOUND != target)))
                .collect(ArrayList::new, List::add);
    }

    private RenderScheme NOT_FOUND = RenderScheme.empty();

    private Observable<RenderScheme> mFromAssets = Observable.just("renderschemes")
            .flatMap(path -> Observable.fromArray(mContext.getAssets().list(path)))
            .map((filename) -> {
                String jsonString = ReaderUtils.InputStreamToString(mContext.getAssets().open("renderschemes/" + filename));
                try {
                    RenderScheme rs = RenderScheme.parse(RenderSchemeModel.create(jsonString), true);
                    return rs == null ? NOT_FOUND : rs;
                } catch (Exception e) {
                    throw new Exception("parse file width assets://renderschemes/" + filename + " failed");
                }
            })
            .onErrorReturn(e -> {
                Log.w(TAG, e.getMessage());
                return NOT_FOUND;
            })
            .filter(target -> NOT_FOUND != target);

    private Observable<RenderScheme> mFromLocal = Observable.just("renderschemes")
            .flatMap(path -> Observable.fromArray(mContext.getDir(path, 0).listFiles((dir, filename) -> filename.endsWith(".rs"))))
            .map(file -> {
                FileInputStream fis = new FileInputStream(file);
                String jsonString = ReaderUtils.InputStreamToString(fis);
                try {
                    RenderScheme rs = RenderScheme.parse(RenderSchemeModel.create(jsonString), true);
                    return rs == null ? NOT_FOUND : rs;
                } catch (Exception e) {
                    throw new Exception("parse file width files://" + file.getAbsolutePath() + " failed");
                }
            })
            .onErrorReturn(e -> {
                Log.w(TAG, e.getMessage());
                return NOT_FOUND;
            })
            .filter(target -> NOT_FOUND != target);

}
