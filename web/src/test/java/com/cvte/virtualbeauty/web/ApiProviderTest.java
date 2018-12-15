package com.cvte.virtualbeauty.web;

import android.content.Context;
import android.content.res.AssetManager;

import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.runners.MockitoJUnitRunner;

import java.io.FileInputStream;
import java.io.InputStream;

import io.reactivex.Observable;

import static org.mockito.Matchers.anyString;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.when;

/**
 * TODO Description
 *
 * @author laizhenqi
 * @since 2017/2/10
 */
@RunWith(MockitoJUnitRunner.class)
public class ApiProviderTest {

    @Mock
    Context mContext;

    @Mock
    AssetManager mAssetManager;

    @Before
    public void setUp() throws Exception {
        assert mContext != null;
        when(mContext.getString(R.string.test_end_point))
                .thenReturn("http://127.0.0.1");
        doReturn(mAssetManager)
                .when(mContext).getAssets();
        InputStream inputStream = new FileInputStream("../assets/test/test.json");
        doReturn(inputStream)
                .when(mAssetManager).open(anyString());
    }

    @Test
    public void getRenderSchemes() {
        ApiProvider.getInstance().getRenderSchemes(mContext)
                .flatMap(Observable::fromIterable)
                .subscribe(rsm -> System.out.print(rsm.toString() + "\n"));
    }
}