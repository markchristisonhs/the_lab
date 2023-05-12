package com.hillandsmith.GPIO_Trials;


import ioio.lib.api.DigitalOutput;
import ioio.lib.api.DigitalInput;
import ioio.lib.api.IOIO;
import ioio.lib.api.exception.ConnectionLostException;
import ioio.lib.util.BaseIOIOLooper;
import ioio.lib.util.IOIOLooper;
import ioio.lib.util.android.IOIOService;

import android.content.Intent;
import android.os.IBinder;
import android.os.SystemClock;

import android.util.Log;

public class MyTest extends IOIOService {
    private static native void inputChanged(int port, int val);

    private static void safeInputChanged(int port, int val) {
        try{
            inputChanged(port,val);
        }
        catch (java.lang.UnsatisfiedLinkError e){
            Log.d("MyTest","inputChanged is not implemented");
        }
    }

    private static boolean m_value_11 = false;
    private static boolean m_value_12 = false;
    private static boolean m_value_13 = false;
    private static boolean m_value_14 = false;

    private static boolean m_value_43 = false;
    private static boolean m_value_44 = false;
    private static boolean m_value_45 = false;
    private static boolean m_value_46 = false;

    public static void setOutput(int port, int val)
    {
        Log.d("MyTest","setOutput is being run with port = " + String.valueOf(port) + " and val of " + String.valueOf(val) );
        if(port == 43)
        {
            m_value_43 = (val != 0);
        }
        if(port == 44)
        {
            m_value_44 = (val != 0);
        }
        if(port == 45)
        {
            m_value_45 = (val != 0);
        }
        if(port == 46)
        {
            m_value_46 = (val != 0);
        }
    }

    public static int getInput(int port)
    {
        int result = 0;
        if(port == 11)
        {
            result = m_value_11 ? 1 : 0;
        }
        if(port == 12)
        {
            result = m_value_12 ? 1 : 0;
        }
        if(port == 13)
        {
            result = m_value_13 ? 1 : 0;
        }
        if(port == 14)
        {
            result = m_value_14 ? 1 : 0;
        }
        return result;
    }

    @Override
    protected IOIOLooper createIOIOLooper() {
        return new BaseIOIOLooper() {
            private DigitalOutput led_, mPin_43, mPin_44, mPin_45, mPin_46;
            private DigitalInput mPin_11, mPin_12, mPin_13, mPin_14;
            private long last_blink = 0;
            private long blink_interval = 500;
            private boolean blink_state = false;

            @Override
            protected void setup() throws ConnectionLostException {
                led_ = ioio_.openDigitalOutput(IOIO.LED_PIN);
                mPin_43 = ioio_.openDigitalOutput(43);
                mPin_44 = ioio_.openDigitalOutput(44);
                mPin_45 = ioio_.openDigitalOutput(45);
                mPin_46 = ioio_.openDigitalOutput(46);

                mPin_11 = ioio_.openDigitalInput(11, DigitalInput.Spec.Mode.PULL_UP);
                mPin_12 = ioio_.openDigitalInput(12, DigitalInput.Spec.Mode.PULL_UP);
                mPin_13 = ioio_.openDigitalInput(13, DigitalInput.Spec.Mode.PULL_UP);
                mPin_14 = ioio_.openDigitalInput(14, DigitalInput.Spec.Mode.PULL_UP);
            }

            @Override
            public void loop() throws ConnectionLostException,
                                      InterruptedException {
                if((last_blink == 0) || ((SystemClock.elapsedRealtime() - last_blink) > blink_interval))
                {
                    blink_state = !blink_state;
                    led_.write(blink_state);
                    last_blink = SystemClock.elapsedRealtime();
                }
                mPin_43.write(m_value_43);
                mPin_44.write(m_value_44);
                mPin_45.write(m_value_45);
                mPin_46.write(m_value_46);

                boolean btemp;

                btemp = mPin_11.read();
                if(btemp != m_value_11)
                {
                    safeInputChanged(11, btemp ? 1 : 0);
                    m_value_11 = btemp;
                }

                btemp = mPin_12.read();
                if(btemp != m_value_12)
                {
                    safeInputChanged(12, btemp ? 1 : 0);
                    m_value_12 = btemp;
                }

                btemp = mPin_13.read();
                if(btemp != m_value_13)
                {
                    safeInputChanged(13, btemp ? 1 : 0);
                    m_value_13 = btemp;
                }

                btemp = mPin_14.read();
                if(btemp != m_value_14)
                {
                    safeInputChanged(14, btemp ? 1 : 0);
                    m_value_14 = btemp;
                }
            }
        };
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        int result = super.onStartCommand(intent, flags, startId);

        return result;
    }

    @Override
    public IBinder onBind(Intent arg0) { return null; }
}
