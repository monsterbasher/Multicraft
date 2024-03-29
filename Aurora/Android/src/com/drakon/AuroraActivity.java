package com.drakon;

import android.app.Activity;
import android.os.Bundle;

public class AuroraActivity extends Activity
{
	private AuroraGLSurfaceView _surface;	
	
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        
        //requestWindowFeature(android.view.Window.FEATURE_NO_TITLE); 
		//getWindow().setFlags(android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN, android.view.WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        _surface = new AuroraGLSurfaceView(this);
        setContentView(_surface);
    }
    
    @Override
    protected void onPause()
    {
        super.onPause();
        _surface.onPause();
        nativePause();
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        _surface.onResume();
        nativeResume();
    }
    
    //ndk part
    static
	{
		System.loadLibrary("auroraLib");
	}
    
    private static native void nativePause();
    private static native void nativeResume();
}