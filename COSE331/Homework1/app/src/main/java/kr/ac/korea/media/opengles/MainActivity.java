package kr.ac.korea.media.opengles;

import android.content.res.AssetManager;
import android.graphics.Point;
import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;



import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;



public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


//    Button buttonW,buttonA,buttonS,buttonD;


    static float previousX=0;
    static float previousY=0;

    GLSurfaceRenderer glSurfaceRenderer;


    @Override
    protected void onCreate(Bundle savedInstanceState) {

        // call superclass method
        super.onCreate(savedInstanceState);

        //setContentView(R.layout.activity_main);
//        buttonW=findViewById(R.id.buttonW);

//        GLSurfaceView glSurfaceView = (GLSurfaceView) findViewById(R.id.surfaceView);
        // create OpenGL ES surface view
        GLSurfaceView glSurfaceView = new GLSurfaceView(this);
        //GLSurfaceView glSurfaceView=findViewById(R.id.surfaceView);

        glSurfaceView.setEGLContextClientVersion(2);
        //glSurfaceView.setRenderer(new GLSurfaceRenderer());
        glSurfaceRenderer = new GLSurfaceRenderer();
        glSurfaceView.setRenderer(glSurfaceRenderer);

        // set content view to OpenGL ES surface view
        setContentView(glSurfaceView);
        Button w = new Button(this);
        Button a = new Button(this);
        Button s = new Button(this);
        Button d = new Button(this);


        w.setText("W");
        a.setText("A");
        s.setText("S");
        d.setText("D");



        Display display = getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getSize(size);
        int width = size.x;
        int height = size.y;
        int buttonWidth  = width/8;
        int buttonHeight = buttonWidth;

        w.setWidth(buttonWidth);
        w.setHeight(buttonHeight);

        a.setWidth(buttonWidth);
        a.setHeight(buttonHeight);

        s.setWidth(buttonWidth);
        s.setHeight(buttonHeight);

        d.setWidth(buttonWidth);
        d.setHeight(buttonHeight);


        s.setX((width-buttonWidth)/2);
        s.setY(7*(height-buttonHeight)/8);

        w.setX((width-buttonWidth)/2);
        w.setY(7*(height-buttonHeight)/8 - buttonHeight);

        a.setX((width-buttonWidth)/2-buttonWidth*2);
        a.setY(7*(height-buttonHeight)/8);

        d.setX((width-buttonWidth)/2 + buttonWidth*2);
        d.setY(7*(height-buttonHeight)/8 );


        w.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                //glSurfaceRenderer.direction = 0;

                switch(event.getAction())
                {
                    case MotionEvent.ACTION_DOWN:
                        setDirection(0);
                        break;
                    case MotionEvent.ACTION_UP:
                        setDirection(-1);
                        break;
                }
                return true;
            }
        });


        a.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                switch(event.getAction())
                {
                    case MotionEvent.ACTION_DOWN:
                        setDirection(1);
                        break;
                    case MotionEvent.ACTION_UP:
                        setDirection(-1);
                        break;
                }
                return true;
            }
        });


        s.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                switch(event.getAction())
                {
                    case MotionEvent.ACTION_DOWN:
                        setDirection(2);
                        break;
                    case MotionEvent.ACTION_UP:
                        setDirection(-1);
                        break;
                }
                return true;
            }
        });


        d.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {

                switch(event.getAction())
                {
                    case MotionEvent.ACTION_DOWN:
                        setDirection(3);
                        break;
                    case MotionEvent.ACTION_UP:
                        setDirection(-1);
                        break;
                }

                return true;
            }
        });

        ViewGroup.LayoutParams lp =new ViewGroup.LayoutParams(ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        this.addContentView(w, lp);
        this.addContentView(a, lp);
        this.addContentView(s, lp);
        this.addContentView(d, lp);

    }

    public void setDirection(int dir){
        glSurfaceRenderer.direction = dir;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {



        float presentX=0;
        float presentY=0;
        float dx=0;
        float dy=0;
        //Log.v("(x,y): ",Integer.toString(x)+" " + Integer.toString(y));

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                previousX=event.getX();
                previousY=event.getY();
//                System.out.println("down");
                break;

            case MotionEvent.ACTION_MOVE:
                presentX=event.getX();
                presentY=event.getY();
                dx = presentX-previousX;
                dy = presentY-previousY;

                previousX=presentX;
                previousY=presentY;
                rotateCamera(dx,dy);
                System.out.println("presentX: " + presentX +" presentY: "+ presentY);
                break;

            case MotionEvent.ACTION_UP:
                previousX=0;
                previousY=0;
                break;

        }
        return false;
    }



    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native void surfaceCreated(AssetManager assetManager);
    public native void surfaceChanged(int width, int height);
    public native void drawFrame(float deltaTime);
    public native void rotateCamera(float dx, float dy);
    public native void translateCamera(int direction);

    private class GLSurfaceRenderer implements GLSurfaceView.Renderer {

        // last time attribute for calculating delta time
        private long lastTime = System.currentTimeMillis();

        int direction = -1;

        @Override
        public void onSurfaceCreated(GL10 gl, EGLConfig config) {

            // call native method
            surfaceCreated(getAssets());
        }

        @Override
        public void onSurfaceChanged(GL10 gl, int width, int height) {

            // call native method
            surfaceChanged(width, height);
        }

        @Override
        public void onDrawFrame(GL10 gl) {

            // calculate delta time
            long currentTime = System.currentTimeMillis();
            float deltaTime = (float) (currentTime - lastTime) / 1000.0f;
            lastTime = currentTime;



            // call native method
            drawFrame(deltaTime);

            if(direction != -1)
                translateCamera(direction);
        }




    }
}
