/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package com.giant.brizzle;

import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import org.cocos2dx.plugin.PluginWrapper;

import android.content.Intent;
import android.os.Bundle;
import android.os.Parcelable;
import android.view.KeyEvent;
import android.view.WindowManager;

public class Brizzle extends Cocos2dxActivity {

	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		PluginWrapper.init(this);
		// If you want your callback function can be invoked in GL thread, add
		// this line:
		PluginWrapper.setGLSurfaceView(Cocos2dxGLSurfaceView.getInstance());
		//createShortCut();
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	}

	
	
	public Cocos2dxGLSurfaceView onCreateView() {
		Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
		// Brizzle should create stencil buffer
		glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);

		return glSurfaceView;
	}

	public static native void exitGame();

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK)
			exitGame();
		return super.onKeyUp(keyCode, event);
	}

	static {
		System.loadLibrary("cocos2dcpp");
	}
	
	public void createShortCut(){   
	    //创建快捷方式的Intent                   
	    Intent shortcutintent = new Intent("com.android.launcher.action.INSTALL_SHORTCUT");                   
	    //不允许重复创建                   
	    shortcutintent.putExtra("duplicate", false);                   
	    //需要现实的名称                   
	    shortcutintent.putExtra(Intent.EXTRA_SHORTCUT_NAME, getString(R.string.app_name));   
	    //快捷图片                  
	    Parcelable icon = Intent.ShortcutIconResource.fromContext(getApplicationContext(), R.drawable.icon);   
	    shortcutintent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, icon);   
	    //点击快捷图片，运行的程序主入口                   
	    shortcutintent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, new Intent(getApplicationContext() , Brizzle.class));                   
	    //发送广播。OK                   
	    sendBroadcast(shortcutintent);   
	}
	
}
