package abschluss.abschlussprojekt;

import android.app.Activity;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.CheckBox;

/**
 * Created by mariu on 18.02.2016.
 */
public class Alarrm extends Activity {

    //system uhrzeit bekommen
    CheckBox c1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        c1=(CheckBox)findViewById(R.id.alarm_an);


    }

    //Uhrzeit einstellen auslesen aus feld

    public void alarm_aktiv(View v)
    {

        if(c1.isChecked()) {

        if(alarm_z==Zeit) {

            //(klingeln)

        }

        }
        }



    }


