package net.kccistc.socketclient;

import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.ToggleButton;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.text.SimpleDateFormat;

public class MainActivity extends AppCompatActivity {
    static MainHandler mainHandler;
    ClientThread clientThread;
    ToggleButton toggleButtonConnect;
    Button buttonSend;
    TextView textViewRecv;
    ScrollView scrollViewRecv;
    SimpleDateFormat dateFormat = new SimpleDateFormat("yy.MM.dd HH:mm:ss");
    ImageButton imageButtonLamp;
    ImageButton imageButtonPlug;
    boolean imageButtonLampFlag = false;
    boolean imageButtonPlugFlag = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        imageButtonLamp = findViewById(R.id.imageButtonLamp);
        imageButtonPlug = findViewById(R.id.imageButtonPlug);
        imageButtonLamp.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(clientThread.socket != null)
                {
                    if(imageButtonLampFlag) {
                        clientThread.sendData(ClientThread.arduinoId + "LAMPOFF");
//                        imageButtonLampFlag = false;
                    }
                    else {
                        clientThread.sendData(ClientThread.arduinoId + "LAMPON");
//                        imageButtonLampFlag = true;
                    }
                }
            }
        });
        imageButtonPlug.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(clientThread.socket != null)
                {
                    if(imageButtonPlugFlag) {
                        clientThread.sendData(ClientThread.arduinoId + "GASOFF");
//                        imageButtonLampFlag = false;
                    }
                    else {
                        clientThread.sendData(ClientThread.arduinoId + "GASON");
//                        imageButtonLampFlag = true;
                    }
                }
            }
        });
        // 각각의 핸들러와 클라이언트는 thread로 생성함
        clientThread = new ClientThread();
        clientThread.start();
        mainHandler = new MainHandler();
    }
    // 메인 핸들러
    // 이너 클래스 - 클래스 내에서 한 번만 쓰고 버릴 클래스
    class MainHandler extends Handler {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);

            // 번들에서 보낸 데이터를 꺼내오는 작업
            Bundle bundle = msg.getData();
            String data = bundle.getString("msg");
//            Log.d("MainHandler",data);
            if(data.indexOf("New connect") != -1) {
                return;
            }
            // 문자열 파싱
            String[] splitLists = data.split("\\[|]|@|\\n") ;
            for(int i=0; i< splitLists.length; i++)
                Log.d("MainHandler"," i: " + i + ", value: " +splitLists[i]);

            // 두 명령어 중에 하나가 오면, img를 업데이트를 하기 위한 func 호출
            if((data.indexOf("LAMP") != -1) ||(data.indexOf("GAS") != -1)) {
                buttonImageUpdate(splitLists[2]);        //LAMPON
            }

        }
        public void buttonImageUpdate(String cmd)
        {
            Log.d("buttonUpdate ", cmd);
            // 각각의 객체를 변경 하도록 처리
            if (cmd.equals("LAMPON")) {
                imageButtonLamp.setImageResource(R.drawable.lamp_on);
                imageButtonLamp.setBackgroundColor(Color.GREEN);
                imageButtonLampFlag = true;
            } else if (cmd.equals("LAMPOFF")) {
                imageButtonLamp.setImageResource(R.drawable.lamp_off);
                imageButtonLamp.setBackgroundColor(Color.LTGRAY);
                imageButtonLampFlag = false;
            } else if (cmd.equals("GASON")) {
                imageButtonPlug.setImageResource(R.drawable.plug_on);
                imageButtonPlug.setBackgroundColor(Color.GREEN);
                imageButtonPlugFlag = true;
            } else if (cmd.equals("GASOFF")) {
                imageButtonPlug.setImageResource(R.drawable.plug_off);
                imageButtonPlug.setBackgroundColor(Color.LTGRAY);
                imageButtonPlugFlag = false;
            }
        }

        public void button_onClick(View v){
            ;
        }
    }
}