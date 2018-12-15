/**
 * Copyright 2016 CVTE All Rights Reserved.
 */
package com.cvte.virtualbeauty.ui.capture;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.cvte.virtualbeauty.R;
import com.cvte.virtualbeauty.common.view.ExtRecyclerView;
import com.cvte.virtualbeauty.data.render.RenderScheme;
import com.squareup.picasso.Picasso;

import java.util.ArrayList;
import java.util.List;

import butterknife.BindView;
import butterknife.ButterKnife;

/**
 * shader列表适配器，使用一个cardView显示当前shader基本信息(预览效果图和名字)
 *
 * @author laizhenqi
 * @since 2017/2/8
 */
class RenderSchemeAdapter extends ExtRecyclerView.ExtAdapter<RenderSchemeAdapter.ViewHolder> {

    private List<RenderScheme> mData = new ArrayList<>();

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return new ViewHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.item_render_scheme_list, parent, false));
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, int position) {
        RenderScheme renderScheme = mData.get(position);
        Picasso.with(holder.itemView.getContext())
                .load(renderScheme.getPreviewUri())
                .into(holder.background);
        if (renderScheme.isCached()) {
            holder.title.setText(renderScheme.getName());
        } else {
            holder.title.setText("download\n" + renderScheme.getName());
        }
        if (mSelectedPosition == position) {
            holder.itemView.setScaleX(1.2f);
        } else {
            holder.itemView.setScaleX(1.0f);
        }
        if (mOnItemClicked != null) {
            holder.itemView.setOnClickListener(v ->
                    mOnItemClicked.onClicked(((RecyclerView.LayoutParams) v.getLayoutParams()).getViewAdapterPosition())
            );
        }
    }

    @Override
    public int getItemCount() {
        return mData.size();
    }

    void refreshList(List<RenderScheme> list) {
        mData.clear();
        mData.addAll(list);
        notifyDataSetChanged();
    }

    @Override
    public RenderScheme getItemAt(int position) {
        return mData.get(position);
    }

    static final class ViewHolder extends RecyclerView.ViewHolder {

        @BindView(R.id.imageView_background_item_render_scheme_list)
        ImageView background;

        @BindView(R.id.textView_title_item_render_scheme_list)
        TextView title;

        ViewHolder(View itemView) {
            super(itemView);
            ButterKnife.bind(this, itemView);
        }
    }
}
