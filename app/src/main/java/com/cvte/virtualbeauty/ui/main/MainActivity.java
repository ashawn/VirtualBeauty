package com.cvte.virtualbeauty.ui.main;

import android.app.Fragment;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;

import com.cvte.virtualbeauty.App;
import com.cvte.virtualbeauty.R;
import com.cvte.virtualbeauty.common.util.ActivityUtils;
import com.cvte.virtualbeauty.internal.di.component.DaggerMainComponent;
import com.cvte.virtualbeauty.internal.di.module.MainModule;

import javax.inject.Inject;

public class MainActivity extends AppCompatActivity {

    @Inject
    MainPresenter mPresenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        MainFragment fragment = (MainFragment) getFragmentManager().findFragmentById(R.id.main_container);
        if(fragment==null) {
            fragment = (MainFragment) Fragment.instantiate(this,MainFragment.class.getName());
            ActivityUtils.replaceFragment(getFragmentManager(),fragment,R.id.main_container);
        }

        DaggerMainComponent.builder()
                .appComponent(App.getInstance().getComponent())
                .mainModule(new MainModule(fragment))
                .build()
                .inject(this);
    }
}
