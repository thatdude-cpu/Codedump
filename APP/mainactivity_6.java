package com.mycompany.myapp;
import android.app.Activity
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter

import android.graphics.Color
import android.net.wifi.ScanResult
import android.net.wifi.WifiManager;

import android.os.Bundle;
import android.view.*;
import android.widget.*;

import java.util.ArrayList
import java.util.List
import java.util.Set;


//Bluetooth an und aus schalten
public class Mainactivity extends Acticity

button b1,b2,b3,b4;
 private BluetoothAdapter BA;


//buttons konfigurieren ids festlegen BA als bluetooth adapter festlegen
@Override
public void onCreate(Bundle savedInstanceState)
{
	super.onCreate(savedInstanceState);
	setContentView(R.layout.main);
	b1=(Button) findViewById(R.id.button1);
	b2=(Button) findViewById(R.id.button2);
 	b3=(Button) findViewById(R.id.button3);
 	b4=(Button) findViewById(R.id.button4);

BA=BluetoothAdapter.getDefault.Adapter();



}

public void an(view v)
{
	if(!BA.isEnabled())

{
 Intent einschalten=new Intent BlutoothAdapter.ACTION_REQUEST_ENABLE);
startActivityForResult(turnOn,0);
Toast.makeText(getApplicationContext();."Ein Geschaltet",Toast.Lenght_Long).show();

}
else
{
 Toast.makeText(getApplicationContext();."Bereits Eingeschaltet", Toast.LENGHT_LONG).show();
}

}
//ausschalten
public void aus(View v)
{
 BA.disable();
Toast.makeText(getApplicationcontext();."Ausgeschaltet".Toast.Lenght_Long.show();

}












public class DeviceScanActivity extends ListActivity
{

    private BluetoothAdapter mBluetoothAdapter;
    private boolean mScanning;
    private Handler mHandler;

    private static final long SCAN_PERIOD == 10000;
//Nach BLE suchen
    private void ScanLeDevice (final boolean enable)
    {//aufhöhren zu suchewn nach 10 sekunden
        if(enable)
        {
            mHandler.postDelayed(new Runnable())
            {
                @override
                public void run()
                {
                    mScanning = false;
                    mBluetoothAdapter.stopLeScan(mLeScanCallback);
                }
            },SCAN_PERIOD);

            mScanning= true;
            mBluetoothAdapter.stopLeScan(mLeScanCallback);
        }else

        {
            mScanning = false;
            mBluetoothAdapter.stopLeScan(mLeScanCallback);

        }

        }


    }

}


private LeDeviceListAdapter mLeDeviceListAdapter;

//Bestätigung des geräts

private BluetoothAdapter.LeScanCallback mLeScanCallback =
        new BluetoothAdapter.LeScanCallback() {
    @Override
    public void onLeScan(final BluetoothDevice device, int rssi,
            byte[] scanRecord) {
        runOnUiThread(new Runnable() {
           @Override
           //gerät hinzufügen und über daten veränderungen informieren
		   public void run() {
               mLeDeviceListAdapter.addDevice(device);
               mLeDeviceListAdapter.notifyDataSetChanged();
           }
       });
   }
};


public vibrieren()
{


    switch(vibrieren)
}







/* //wnn sich characteristic Änderungen meldung
private BluetoothGatt mBluetoothGatt;
BluetoothGattCharacteristic characteristic;
boolean enabled;
mBluetoothGatt.setCharacteristicNotification(characteristic, enabled);
BluetoothGattDescriptor descriptor = characteristic.getDescriptor(
        UUID.fromString(SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG));
descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
mBluetoothGatt.writeDescriptor(descriptor); */


//Beenden
public void close() {
    if (mBluetoothGatt == null) {
        return;
    }
    mBluetoothGatt.close();
    mBluetoothGatt = null;
}
