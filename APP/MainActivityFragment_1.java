package abschluss.abschlussprojekt;

/**
 * Created by DellPower2 on 16.12.2015.
 */

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import android.graphics.Color;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiManager;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.ViewGroup;
import android.view.View;
import android.widget.*;
import android.support.v4.app.Fragment;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
public class MainActivityFragment
extends Fragment{

public MainActivityFragment() {
}

@Override
    public View
    onCreateView(LayoutInflater inflater,ViewGroup container,
                    Bundle
                 savedInstanceState) {

    return
            inflater.inflate(R.layout.fragment_main,container,false);
}
}


