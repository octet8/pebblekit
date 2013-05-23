package com.yoctopuce.examples.yocto_relay;

import java.util.Random;
import java.util.UUID;

import com.getpebble.android.kit.PebbleKit;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

public class PebbleTestFragment extends Fragment {
	private static final String TAG = "PebbleTestFragment";
	private static final String PREF_SMS_ACTIVITY = "pref_sms";
	private static final String PREF_NUMBER = "phone_number";

	private final static UUID PEBBLE_APP_UUID = UUID
			.fromString("7D93A5D5-EA73-4192-B389-01A94242CA82");
	private final static int CMD_KEY = 0x00;
	private final static int CMD_UP = 0x01;

	private PebbleKit.PebbleDataReceiver dataReceiver;

	private StringBuilder mLogs;
	private TextView mLogsView;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		mLogs = new StringBuilder();
		mLogs.append("start\n");
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container,
			Bundle savedInstanceState) {
		View v = inflater.inflate(R.layout.fragment_pebbletest, container,
				false);
		mLogsView = (TextView) v.findViewById(R.id.pebble_messages);
		mLogsView.setText(mLogs);
		return v;
	}

	public static PebbleTestFragment getInstance() {
		PebbleTestFragment fragment = new PebbleTestFragment();
		return fragment;
	}

}
