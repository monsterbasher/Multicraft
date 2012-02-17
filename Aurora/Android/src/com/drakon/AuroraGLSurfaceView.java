package com.drakon;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.view.MotionEvent;
import android.view.View;

public class AuroraGLSurfaceView extends GLSurfaceView
{
	private AuroraRenderer _renderer;
	private boolean _customEventListener = false;

	public AuroraGLSurfaceView(Context context)
	{
		super(context);
		
		_renderer = new AuroraRenderer(context);
		setRenderer(_renderer);
		
		if (Build.VERSION.SDK_INT > Build.VERSION_CODES.ECLAIR_MR1)
		{
			_customEventListener = true;
			
			this.setOnTouchListener(new OnTouchListener()
			{
				public boolean onTouch(View v, MotionEvent event)
				{
					nativeTouchEvent(event.getAction(),(int)event.getX(),(int)event.getY());
					return true;
				}
			});
		}
	}
	
	public boolean onTouchEvent(MotionEvent event)
	{
		//ndk
		if(!_customEventListener)
			nativeTouchEvent(event.getAction(),(int)event.getX(),(int)event.getY());
		
		return super.onTouchEvent(event);
	}
	

	private static native void nativeTouchEvent(int action,int x,int y);
}
