/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.ui.main;

import javax.inject.Inject;

/**
 * MainPresenter实现
 *
 * @author laizhenqi
 * @since 2017/1/21
 */
public class MainPresenterImpl implements MainPresenter {

    private MainView mView;

    @Inject
    MainPresenterImpl(MainView view) {
        this.mView = view;
    }

    @Inject
    void init() {
        mView.setPresenter(this);
    }

    @Override
    public void start() {
    }

    @Override
    public void resume() {
    }

    @Override
    public void pause() {
    }

    @Override
    public void stop() {

    }
}
