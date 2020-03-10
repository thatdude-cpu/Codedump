package abschluss.abschlussprojekt;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.Switch;

/**
 * Created by DellPower2 on 02.02.2016.
 */
public class Einstellungen extends Activity {

    CheckBox c1,c2,c3,c4;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        c1=(CheckBox)findViewById(R.id.vibriren_an);
        c2=(CheckBox)findViewById(R.id.benachrichtigungrn_an);
        c3=(CheckBox)findViewById(R.id.);
       // c4=(CheckBox)findViewById(R.id.checkBox4);
    }




    public void vibrieren_ (View v)
    {
        if(c1.isChecked()) {


            //vibrieren

        }


    }

public void benachrichtigungen (View v)
    {
            if(c2.isChecked()){
    //notify listener implementieren
            }

    }

    public void main(View v){

        setContentView(R.layout.fragment_main);
    }

}
