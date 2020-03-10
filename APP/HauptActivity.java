package weinhardt.notificator;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import android.view.View.OnClickListener;

public class HauptActivity extends AppCompatActivity {

    private BluetoothAdapter BA;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_haupt);
        //Bluetooth Adapter initalisieren
        final BluetoothManager bluetoothManager =
                (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        BA = bluetoothManager.getAdapter();




    }


        //permissions für bluetooth einfügen
    //Bluetooth Adapter einsschalten
    public void BL_an (View v)
    {
        if (BA== null||!BA.isEnabled())
        {
            Intent einschalten = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(einschalten,0);//???
            Toast.makeText(getApplicationContext(), "Bluetooth Eingeschaltet", Toast.LENGTH_LONG).show();
        }
    }



    //Layout wechseln
    public void Wecker_wechseln (View v)
    {
       Intent intent= new Intent(HauptActivity.this,AlarmActivity.class);
        startActivity(intent);
    }
    //Layout wechseln
    public void Kalibration_wechseln (View v)
    {
        Intent intent= new Intent(HauptActivity.this,KalibrierungActivity.class);
        startActivity(intent);
    }

    //Layout wechseln
    public void Einstellungen_wechseln (View v)
    {

        Intent intent= new Intent(HauptActivity.this,EinstellungenActivity.class);
        startActivity(intent);
    }

}
/* onend  public void close() {
if (mBluetoothGatt == null) {
        return;
        }
        mBluetoothGatt.close();
        mBluetoothGatt = null;
        }

        */

//Abfragen ob das Gerät BLE fähig ist
       /* if (!getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH_LE)) {
            Toast.makeText(getApplicationContext(), "Bluetooth Low Energy wird auf diesem Gerät nicht Unterstützt", Toast.LENGTH_LONG).show();
        }


    }
*/