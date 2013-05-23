package com.yoctopuce.examples.yocto_relay;

import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.view.Menu;

public class MainActivity extends FragmentActivity {

	private android.support.v4.app.Fragment mFragment;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_fragment);
		FragmentManager fm = getSupportFragmentManager();
		mFragment = fm.findFragmentById(R.id.fragmentContainter);
		if(mFragment == null){
			mFragment = PebbleTestFragment.getInstance();
			fm.beginTransaction().add(R.id.fragmentContainter,mFragment).commit();
		}
		
	}

	
	
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
