package weinhardt.notificator;

import android.app.ListActivity;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.widget.Toast;

/**
 * Created by mariu on 30.04.2016.
 */
/*
public class KalibrierungListActivity extends ListActivity{



    private BluetoothAdapter BA;
    private boolean mScanning;
    private Handler mHandler;
    private static final long SCAN_PERIOD = 10000;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    //wird app pausiert wird der scan beendet
    @Override
    protected void onPause() {
        super.onPause();
        suchen(false);

    }



    private void suchen(final boolean enable) {

        if (!BA.isEnabled()) {
            Intent einschalten = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(einschalten, REQUEST_ENABLE_BT);
            Toast.makeText(getApplicationContext(), "Bluetooth Eingeschaltet", Toast.LENGTH_LONG).show();


        } else {
            suchen(true);
        }
        //alternativ start lescan
        if (enable) {
            mHandler.postDelayed(new Runnable() {
                @Override
                public void run() {
                    mScanning = false;
                    BA.stopLeScan(mLeScanCallback);
                        }
                    }, SCAN_PERIOD);

                    mScanning = true;
                    BA.startLeScan(mLeScanCallback);
                } else {
                    mScanning = false;
                    BA.stopLeScan(mLeScanCallback);
                }
            }







    public void Menu_wechseln() {
        setContentView(R.layout.activity_haupt);
    }

    public void Vibrieren() {
        //vibrieren
    }

}
*/

/*
// Device scan callback.
private LeDeviceListAdapter mLeDeviceListAdapter;

private BluetoothAdapter.LeScanCallback mLeScanCallback =
        new BluetoothAdapter.LeScanCallback() {
            @Override
            public void onLeScan(final BluetoothDevice device, int rssi,
                                 byte[] scanRecord) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mLeDeviceListAdapter.addDevice(device);
                        mLeDeviceListAdapter.notifyDataSetChanged();
                    }
                });
            }
        };
        */
