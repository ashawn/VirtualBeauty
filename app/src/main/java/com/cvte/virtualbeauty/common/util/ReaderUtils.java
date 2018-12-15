/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.common.util;

import android.support.annotation.Nullable;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.UnsupportedEncodingException;

/**
 * 读取工具类
 *
 * @author laizhenqi
 * @since 2017/2/9
 */
public class ReaderUtils {

    private static final String TAG = ReaderUtils.class.getSimpleName();

    /**
     * <p>从InputStream中读取字节流转换成UTF-8字符串，如果失败，则返回null</p>
     * <p>读取完成后关闭InputStream</p>
     *
     * @param is
     * @return 读取结果，如果失败则返回null
     */
    @Nullable
    public static String InputStreamToString(InputStream is) {
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
