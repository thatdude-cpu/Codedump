package weinhardt.notificator;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.CheckBox;
import android.widget.Toast;

public class AlarmActivity extends AppCompatActivity {

    CheckBox c1;
    int alarmzeit;
    int systemzeit;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_alarm);
    }



//Alarm aktivieren variablen deklarieren //Checkbox eventuell anders imleentieren
public void Alarm_an()
{
    if(c1.isChecked())
    {
      /*  if(alarmzeit==systemzeit)
        {
            vibrieren()
        }
    */

        Toast.makeText(getApplicationContext(), "Bluetooth Eingeschaltet", Toast.LENGTH_LONG).show();
    }




}

    public void Menue_wechseln()
    {
        finish();
    }
}
