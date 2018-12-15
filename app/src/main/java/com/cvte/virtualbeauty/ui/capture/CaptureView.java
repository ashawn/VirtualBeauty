/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.ui.capture;

import com.cvte.virtualbeauty.base.BaseView;
import com.cvte.virtualbeauty.data.render.RenderScheme;

import java.util.List;

/**
 * capture界面view，负责加载相关ui布局
 *
 * @author laizhenqi
 * @since 2017/2/6
 */
public interface CaptureView extends BaseView<CapturePresenter> {

    /**
     * <p>更新渲染方案列表</>
     * <p>一旦更新列表，需将之前加载列表清空</p>
     *
     * @param list 待更新的列表
     */
    void updateRenderSchemeList(List<RenderScheme> list);
}
