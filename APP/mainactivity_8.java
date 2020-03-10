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


//Buutons listview BluetoothAdapter und angeschlossene Geräte Hinzufügen(ertsellen)
public class MainActivity extends Activity
{
 button b1,b2,b3,b4;
 private BluetoothAdapter BA;
private Set<Bluetoothdevice> pairedDevices;
ListView lv;


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
lv=(ListView)findViewById(R.id.ListView);


}
	//einschalten wird mit knopf eingeschaltet
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
//sichtbar für andere geräte eventuell meldung
public void schtbar(view v)
{
 Intent sichtbarwerden = new Intent(<bluetoothAdapter.ACTION_REQUEST_DISCOVERABLE());
startActivityForResults(getvisible,0);

}
//liste von geräten anzeigen
public void liste(View v)
{
 pairedDevices= BA.getBondedDevices();
ArrayList = new ArrayList();

for(BluetoothDevice bt: pairedDevices)

list.add(bt.getName());
Toast.makeText(getApplicationContext()."zeige gekoppelte geräte",Toast.LENGTH_SHORT).show();

final ArrayADapter adapter =new ArrayAdapter(this,android.R.layout.simple_list_item_item_1,list);
lv setAdapter(adapter);
}



@override
public boolean onCreateOptionMenu(Menu menu)
{
//zu action bar hinzufügen
getMenuInflater().inflate(R.menu.menu_main,menu);
return true;

}


@override //optionen ausgewählt
public boolean onOptionsItemSelected(MenuItem item)
{

  int id = item.getItemId();



	if(id==R.id.action_settings)
{

	return true;
}
return super.onOptionsItemSelected(item);

}

}










