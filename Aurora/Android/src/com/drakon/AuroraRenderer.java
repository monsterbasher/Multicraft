package com.drakon;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.view.Display;
import android.view.WindowManager;
import android.opengl.GLSurfaceView;

public class AuroraRenderer implements GLSurfaceView.Renderer
{
	String apkFilePath = null;
	static boolean s_firstCreate = true;
	private Context context;
	
	public AuroraRenderer(Context context)
	{	
		this.context = context;
	}
	
	public void onDrawFrame(GL10 gl)
	{
		nativeRender();
	}

	public void onSurfaceChanged(GL10 gl, int width, int height)
	{
		nativeResize(width, height);
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config)
	{
		if( s_firstCreate )
		{
			s_firstCreate = false;
			
			//init
			ApplicationInfo appInfo = null;
			PackageManager packMgmr = context.getPackageManager();
			try 
			{
				appInfo = packMgmr.getApplicationInfo("com.drakon", 0);
			} 
			catch (NameNotFoundException e) 
			{
				e.printStackTrace();
				throw new RuntimeException("Unable to locate assets, aborting...");
			}

			apkFilePath = appInfo.sourceDir;
			String savePath = context.getFilesDir().getAbsolutePath();

			Display display = ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay(); 

			nativeInit(apkFilePath, savePath, display.getWidth(), display.getHeight());
		}
	}

	
	private static native void nativeInit(String apkPath, String savePath, int width, int height);
    private static native void nativeResize(int w, int h);
    private static native void nativeRender();
}
