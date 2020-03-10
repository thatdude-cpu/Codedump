package abschluss.abschlussprojekt;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

/**
 * Created by mariu on 18.02.2016.
 */
public class AlarmFragment extends Fragment {

    public AlarmFragment() {
    }

    @Override
    public View
    onCreateView(LayoutInflater inflater, ViewGroup container,
                 Bundle
                         savedInstanceState) {

        return
                inflater.inflate(R.layout.fragment_alarm, container, false);
    }
}