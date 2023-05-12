package com.hillandsmith.GPIO_Trials;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;

import android.util.Log;

public class GPIO_Activity extends Activity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("GPIO_Activity","Running GPIO_Activity");
        super.onCreate(savedInstanceState);
        startService(new Intent(this, MyTest.class));
        finish();
    }
}
