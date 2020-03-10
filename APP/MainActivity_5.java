package abschluss.abschlussprojekt;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.net.Uri;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.common.api.GoogleApiClient;

/**
 * Created by DellPower2 on 16.12.2015.
 */


public class MainActivity extends AppCompatActivity {

    Button b1, b2, b3, b4;
    private BluetoothAdapter BA;



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        BluetoothAdapter BA = BluetoothAdapter.getDefaultAdapter();



    }

/*
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        if (id == R.id.action_settings) {

            return true;
        }

        return super.onOptionsItemSelected(item);
    }
*/
//buttons konfigurieren ids festlegen BA als bluetooth adapter festlegen


    public void bl_an(View v) {
        if (!BA.isEnabled())

        {
            Intent einschalten = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(einschalten, 0);
            Toast.makeText(getApplicationContext(), "Ein Geschaltet", Toast.LENGTH_LONG).show();

        } else {
            Toast.makeText(getApplicationContext(), "Bereits Eingeschaltet", Toast.LENGTH_LONG).show();
        }

    }

    //ausschalten
    public void bl_aus(View v) {
        BA.disable();
        Toast.makeText(getApplicationContext(), "Ausgeschaltet", Toast.LENGTH_LONG).show();

    }


    public void scan(View v) {
        //LE scan nach geräten
    }

    public void verbinden(View v) {
        //Verbinden mit gefundenem gerät

    }

    public void alarm(View V) {
        setContentView(R.layout.fragment_alarm);


    }

    public void einstellungen(View v) {
        setContentView(R.layout.fragment_einstellungen);
    }


}




