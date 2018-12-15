/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.common.view;

import android.content.Context;
import android.support.annotation.Nullable;
import android.support.v7.widget.RecyclerView;
import android.util.AttributeSet;
import android.util.SparseBooleanArray;

/**
 * 增强型recyclerview，支持item选中和item点击操作
 *
 * @author laizhenqi
 * @since 2017/2/9
 */
public class ExtRecyclerView extends RecyclerView {

    public static final int MODE_SINGLE = 0x00;
    public static final int MODE_MULTIPLE = 0x01;

    public ExtRecyclerView(Context context) {
        super(context);
    }

    public ExtRecyclerView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public ExtRecyclerView(Context context, @Nullable AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public void setExtAdapter(ExtAdapter adapter) {
        setAdapter(adapter);
    }

    public void setSelectMode(int mode) {
        if (getAdapter() == null)
            throw new NullPointerException("please set ExtAdapter before set SelectMode");
        ((ExtAdapter) getAdapter()).setSelectMode(mode);
    }

    @Override
    public void setAdapter(Adapter adapter) {
        if (adapter instanceof ExtAdapter) {
            super.setAdapter(adapter);
        } else {
            throw new IllegalArgumentException("please use ExtAdapter not RecyclerView.Adapter");
        }
    }

    public void setOnItemClicked(OnItemClicked onItemClicked) {
        ((ExtAdapter) getAdapter()).setOnItemClicked(onItemClicked);
    }

    public static abstract class ExtAdapter<VH extends ViewHolder> extends Adapter<VH> {

        protected OnItemClicked mOnItemClicked;

        private final SparseBooleanArray mSelectedArray = new SparseBooleanArray();
        private int mSelectMode = MODE_SINGLE;
        protected int mSelectedPosition = 0;

        void setOnItemClicked(OnItemClicked onItemClicked) {
            mOnItemClicked = onItemClicked;
        }

        void setSelectMode(int mode) {
            mSelectMode = mode;
            synchronized (mSelectedArray) {
                mSelectedArray.clear();
            }
        }

        public abstract Object getItemAt(int position);

        /**
         * 只有在单选模式下该方法返回才有意义
         *
         * @return 当前选中对象的位置
         */
        public int getSelectedPosition() {
            return mSelectedPosition;
        }

        /**
         * 选中某个位置
         *
         * @param position
         * @return 如果选中状态设置成功返回true，反之false
         */
        public boolean selected(int position) {
            return selected(position, true);
        }

        /**
         * 将position处的Item设置为flag标识的状态
         *
         * @param position
         * @param flag
         * @return 如果设置成功返回true，反之false
         */
        public boolean selected(int position, boolean flag) {
            boolean ret = false;
            switch (mSelectMode) {
                case MODE_SINGLE:
                    synchronized (mSelectedArray) {
                        mSelectedArray.clear();
                        if (flag) {
                            mSelectedArray.put(position, flag);
                            mSelectedPosition = position;
                            ret = true;
                        }
                    }
                    break;
                case MODE_MULTIPLE:
                    synchronized (mSelectedArray) {
                        if (mSelectedArray.get(position) != flag) {
                            mSelectedArray.put(position, flag);
                            mSelectedPosition = position;
                            ret = true;
                        }
                    }
                    break;
            }
            if (ret) notifyDataSetChanged();
            return ret;
        }
    }

    public interface OnItemClicked {
        void onClicked(int position);
    }

}
