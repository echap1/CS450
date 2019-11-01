package com.ethanchapman.arduinocontroller;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.Bundle;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ScrollView;
import android.widget.TextView;

import java.text.DecimalFormat;

import app.akexorcist.bluetotohspp.library.BluetoothSPP;
import app.akexorcist.bluetotohspp.library.BluetoothState;

@SuppressLint("SetTextI18n")
public class MainActivity extends AppCompatActivity {
    TextView console = null;
    TextView info = null;
    ScrollView scrollView = null;

    long startTime = 0;
    DecimalFormat logTimeFmt = new DecimalFormat("#0.00");
    String address = "00:18:E4:34:D4:DE";
    Context context = null;
    BluetoothSPP bt = null;
    
    int[] pos = new int[8];

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        context = getApplicationContext();

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        startTime = System.currentTimeMillis();

        console = findViewById(R.id.console);
        console.setText("");

        scrollView = findViewById(R.id.scroll);

        info = findViewById(R.id.info);

        pos = new int[] {128, 128, 128, 128, 128, 128, 0, 0};

        setInfo();

        log("Connecting to device " + address);

        bt = new BluetoothSPP(context);
    }

    @Override
    protected void onStart() {
        super.onStart();

        if (!bt.isBluetoothAvailable()) {
            log("Bluetooth Unavailable");
        } else {
            log("Bluetooth Available");
            bt.setupService();
            bt.startService(BluetoothState.DEVICE_OTHER);

            bt.connect(address);

            bt.setOnDataReceivedListener(new BluetoothSPP.OnDataReceivedListener() {
                public void onDataReceived(byte[] data, String message) {
                    log("> " + message);
                }
            });

            bt.setBluetoothConnectionListener(new BluetoothSPP.BluetoothConnectionListener() {
                public void onDeviceDisconnected() {
                    log("Disconnected, Reconnecting...");
                    bt.connect(address);
                }

                public void onDeviceConnectionFailed() {
                    log("Connection Failed, Retrying...");
                    bt.connect(address);
                }

                public void onDeviceConnected(String name, String address) {
                    log("Connected to " + name + " (" + address + ")");
                    bt.send("connected", true);
                }
            });
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (Controller.keyDown(keyCode)) sendData();
        return true;
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        if (Controller.keyUp(keyCode)) sendData();
        return true;
    }

    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {

        // Check that the event came from a game controller
        if ((event.getSource() & InputDevice.SOURCE_JOYSTICK) ==
                InputDevice.SOURCE_JOYSTICK &&
                event.getAction() == MotionEvent.ACTION_MOVE) {

            // Process the current movement sample in the batch (position -1)
            pos = Controller.processJoystickInput(event, -1);

            sendData();

            return true;
        }
        return super.onGenericMotionEvent(event);
    }

    private void sendData() {
        bt.send(new byte[] {'C',
                (byte) pos[0], (byte) pos[1],
                (byte) pos[2], (byte) pos[3],
                (byte) pos[4], (byte) pos[5],
                (byte) pos[6], (byte) pos[7],
                (byte) Controller.buttons}, true);

        setInfo();
    }

    private void setInfo() {
        info.setText("Current Vals:\n" + logText(" " + pos[0] +
                ", " + pos[1] +
                ", " + pos[2] +
                ", " + pos[3] +
                ", " + pos[4] +
                ", " + pos[5] +
                ", " + pos[6] +
                ", " + pos[7] + "\n" + Controller.buttonString()));
    }

    private void log(String text) {
        double displayNum = (System.currentTimeMillis() - startTime) / 1000f;

        console.append("+" + logTimeFmt.format(displayNum) + ":" + text + "\n");

        scrollView.post(new Runnable() {
            public void run() {
                scrollView.fullScroll(View.FOCUS_DOWN);
            }
        });
    }

    private String logText(String text) {
        double displayNum = (System.currentTimeMillis() - startTime) / 1000f;

        return "+" + logTimeFmt.format(displayNum) + ":" + text + "\n";
    }
}
