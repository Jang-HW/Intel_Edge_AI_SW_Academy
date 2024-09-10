package net.kccistc.socketclient;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.ToggleButton;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.text.SimpleDateFormat;
import java.util.Date;

public class MainActivity extends AppCompatActivity {
    static MainHandler mainHandler;
    ClientThread clientThread;
    TextView textView;
    ScrollView scrollViewRecv;
    ToggleButton toggleButtonStart;
    SimpleDateFormat dataFormat = new SimpleDateFormat("yy.MM.dd HH:mm:ss");

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        EditText editTextIp = findViewById(R.id.editTextIp);
        EditText editTextPort = findViewById(R.id.editTextPort);
        toggleButtonStart = findViewById(R.id.toggleButtonStart);
        Button buttonSend = findViewById(R.id.buttonSend);
        EditText editTextSend = findViewById(R.id.editTextSend);
        buttonSend.setEnabled(false);
        textView = findViewById(R.id.textViewRecv);
        scrollViewRecv = findViewById(R.id.scrollViewRecv);
        if(ClientThread.socket != null)
            toggleButtonStart.setEnabled(false);
        toggleButtonStart.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(toggleButtonStart.isChecked()) {
                    String strIp = editTextIp.getText().toString();
                    int intPort = Integer.parseInt(editTextPort.getText().toString());
                    clientThread = new ClientThread(strIp,intPort);
                    clientThread.start();
                    buttonSend.setEnabled(true);
                } else {
                    clientThread.stopClient();
                    buttonSend.setEnabled(false);
                }
            }
        });


        buttonSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String strSend = editTextSend.getText().toString();
                clientThread.sendData(strSend);
                editTextSend.setText("");
            }
        });
        mainHandler = new MainHandler();
    }
    class MainHandler extends Handler {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            Bundle bundle = msg.getData();
            String data = bundle.getString("msg");

            Date date = new Date();
            String strDate = dataFormat.format(date);
            strDate = strDate + " " + data;
            textView.append(strDate);
            scrollViewRecv.fullScroll(View.FOCUS_DOWN);
        }
    }
}