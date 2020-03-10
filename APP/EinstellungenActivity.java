package weinhardt.notificator;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.CheckBox;
import android.widget.Toast;

public class EinstellungenActivity extends AppCompatActivity {

    CheckBox c1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_einstellungen);
    }


    public void Benachrichtigungen_vibrieren()
    {   //Vielleicht anders intialisieren
        if(c1.isChecked())
        {
            //notification listener implementieren
            Toast.makeText(getApplicationContext(), "Bluetooth Eingeschaltet", Toast.LENGTH_LONG).show();
        }
    }


    public void Menue_wechseln()
    {
        finish();
    }
}