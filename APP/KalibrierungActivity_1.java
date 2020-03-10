package weinhardt.notificator;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Toast;

public class KalibrierungActivity extends AppCompatActivity {

    private BluetoothAdapter BA;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_kalibrierung);
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
            startActivityForResult(einschalten, 0);
            Toast.makeText(getApplicationContext(), "Bluetooth Eingeschaltet", Toast.LENGTH_LONG).show();


        } else {
            suchen(true);
        }

        if (enable) {
        }

    }




                public void Menu_wechseln() {
                    finish();
                }

                public void Vibrieren() {
                    //vibrieren
                }

            }
