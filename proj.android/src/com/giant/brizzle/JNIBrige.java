package com.giant.brizzle;

import org.cocos2dx.plugin.PluginWrapper;

import android.widget.Toast;

public class JNIBrige {

	public static void toast(final String str) {
		PluginWrapper.runOnMainThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(PluginWrapper.getContext(), str,
						Toast.LENGTH_LONG).show();
			}
		});
	}
}
