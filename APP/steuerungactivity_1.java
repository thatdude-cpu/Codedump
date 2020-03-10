public class steuerungactivity extends Acticity

button b1,b2,b3,b4,b5;
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
	b5=(Button) findViewById(R.id.button5);
BA=BluetoothAdapter.getDefault.Adapter();



}

//Characxteristic übergabe
public void vibrationsmuster einsellen {

}


